#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define COMPONENTS_DIR "../components/"

typedef enum
{
    TOK_KEYWORD,
    TOK_IDENTIFIER,
    TOK_SYMBOL,
    TOK_TAG,
    TOK_TEXT,
    TOK_JSBLOCK,
    TOK_EOF
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[1024];
} Token;

extern Token tokens[2048];
extern int tokenCount;
extern void lexer(const char *filename);

typedef struct ElementStack
{
    char tagName[64];
    char attributes[256];
    int id;
    struct ElementStack *prev;
} ElementStack;

// ----------------- Generar un componente JS -----------------
void generate_component(FILE *out, const char *componentName)
{
    fprintf(out, "function %s(root) { \n", componentName);

    ElementStack *stack = NULL;
    int elemId = 0;

    for (int i = 0; i < tokenCount; i++)
    {
        Token t = tokens[i];

        if (t.type == TOK_KEYWORD && strcmp(t.lexeme, "component") == 0)
            continue;
        else if (t.type == TOK_IDENTIFIER)
            continue;
        else if (t.type == TOK_TAG)
        {
            if (t.lexeme[1] == '/')
            {
                if (stack)
                {
                    ElementStack *old = stack;
                    stack = stack->prev;
                    free(old);
                }
            }
            else
            {
                char tagName[64] = "";
                char attrs[256] = "";
                sscanf(t.lexeme, "<%63[^ >]", tagName);

                if (strcmp(tagName, "html") == 0 || strcmp(tagName, "/html") == 0)
                    continue;

                char *space = strchr(t.lexeme, ' ');
                if (space)
                {
                    strcpy(attrs, space + 1);
                    int len = strlen(attrs);
                    if (len > 0 && attrs[len - 1] == '>')
                        attrs[len - 1] = '\0';
                }

                elemId++;
                fprintf(out, "  let e%d = document.createElement('%s');\n", elemId, tagName);

                if (strlen(attrs) > 0)
                {
                    char *tok = strtok(attrs, " ");
                    while (tok)
                    {
                        char key[64], value[128];
                        if (sscanf(tok, "%63[^=]=\"%127[^\"]\"", key, value) == 2)
                        {
                            fprintf(out, "  e%d.setAttribute(\"%s\", \"%s\");\n", elemId, key, value);
                        }
                        tok = strtok(NULL, " ");
                    }
                }

                if (stack)
                    fprintf(out, "  e%d.appendChild(e%d);\n", stack->id, elemId);
                else
                    fprintf(out, "  root.appendChild(e%d);\n", elemId);

                ElementStack *newElem = malloc(sizeof(ElementStack));
                strcpy(newElem->tagName, tagName);
                strcpy(newElem->attributes, attrs);
                newElem->id = elemId;
                newElem->prev = stack;
                stack = newElem;
            }
        }
        else if (t.type == TOK_TEXT)
        {
            if (stack)
                fprintf(out, "  e%d.textContent = \"%s\";\n", stack->id, t.lexeme);
        }
        else if (t.type == TOK_JSBLOCK)
        {
            fprintf(out, "  // Bloque JS crudo\n");
            size_t len = strlen(t.lexeme);
            if (len > 0 && t.lexeme[len-1] != ';' && t.lexeme[len-1] != '\n')
                fprintf(out, "  %s;\n", t.lexeme);
            else
                fprintf(out, "  %s\n", t.lexeme);
        }
    }

    fprintf(out, "}\n\n");
}

// ----------------- Main -----------------
int main()
{
    DIR *dir = opendir(COMPONENTS_DIR);
    if (!dir)
    {
        perror("❌ Error al abrir carpeta de componentes");
        return 1;
    }

    FILE *out = fopen("../src/output.js", "w");
    FILE *indexJS = fopen("../src/index.js", "w");
    if (!out || !indexJS)
    {
        perror("❌ Error al abrir archivos JS");
        closedir(dir);
        return 1;
    }

    fprintf(indexJS, "function init(){\n");
    fprintf(indexJS, "const root = document.body;\n");

    struct dirent *archivo;
    char ruta[512];
    char base_name[256];

    while ((archivo = readdir(dir)))
    {
        if (strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0)
            continue;

        char *ext = strrchr(archivo->d_name, '.');
        if (!ext || strcmp(ext, ".mn") != 0)
            continue;

        strncpy(base_name, archivo->d_name, ext - archivo->d_name);
        base_name[ext - archivo->d_name] = '\0';

        snprintf(ruta, sizeof(ruta), "%s/%s", COMPONENTS_DIR, archivo->d_name);
        printf("📄 Procesando: %s\n", ruta);

        tokenCount = 0;
        lexer(ruta);
        generate_component(out, base_name);

        fprintf(indexJS, "%s(root);\n", base_name);
    }

    fprintf(indexJS, "}\n");
    fprintf(indexJS, "\ninit();\n");

    fclose(out);
    fclose(indexJS);
    closedir(dir);

    printf("✅ output.js y index.js generados correctamente\n");
    return 0;
}
