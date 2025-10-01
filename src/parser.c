#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef enum
{
    TOK_KEYWORD,
    TOK_IDENTIFIER,
    TOK_SYMBOL,
    TOK_TAG,
    TOK_TEXT,
    TOK_EOF
} TokenType;
typedef struct
{
    TokenType type;
    char lexeme[512];
} Token;

extern Token tokens[1024];
extern int tokenCount;
extern void lexer(const char *filename);

typedef struct ElementStack
{
    char tagName[64];
    char attributes[256];
    int id;
    struct ElementStack *prev;
} ElementStack;

void parse_and_generate()
{
    FILE *out = fopen("output.js", "w");
    DIR *index = opendir("components");
    if (!index) {
        perror("Error al abrir el directorio");
        return 1;
    }

    struct dirent *archivo;
    char name_archivo[256];
    char base_name[256];
    FILE *indexJS = fopen("index.js", "w"); // Abrir archivo una sola vez
    if (!out) {
        perror("Error al abrir index.js");
        closedir(index);
        return 1;
    }
    fprintf(indexJS, "function init(){\n");
    fprintf(indexJS, "const root = document.body;\n");
     while ((archivo = readdir(index))) {
        if (strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0)
            continue; // Ignorar . y ..

        // Copiar nombre a variable
        strcpy(name_archivo, archivo->d_name);

        // Buscar extensión .mn
        char *ext = strrchr(name_archivo, '.'); // Encuentra el último "."
        if (ext && strcmp(ext, ".mn") == 0) {
            *ext = '\0'; // Eliminar extensión, ahora name_archivo es solo el nombre base
        } else {
            continue; // Saltar archivos que no sean .mn
        }

        // Copiar a base_name
        strcpy(base_name, name_archivo);
        fprintf(indexJS, "%s(root);\n", base_name);

        // Escribir en index.js
        fprintf(out, "function %s(root) {\n", base_name);
    }
    fprintf(indexJS, "}\n");
    closedir(index);

    if (!out)
    {
        printf("Error al abrir output.js\n");
        exit(1);
    }

    ElementStack *stack = NULL;
    int elemId = 0;

    for (int i = 0; i < tokenCount; i++)
    {
        Token t = tokens[i];

        if (t.type == TOK_KEYWORD && strcmp(t.lexeme, "component") == 0)
        {
            continue;
        }
        else if (t.type == TOK_IDENTIFIER)
        {
            // nombre del componente, ignorar por ahora
            continue;
        }
        else if (t.type == TOK_TAG)
        {
            if (t.lexeme[1] == '/')
            {
                // cerrar tag
                if (stack)
                {
                    ElementStack *old = stack;
                    stack = stack->prev;
                    free(old);
                }
            }
            else
            {
                // abrir tag
                char tagName[64] = "";
                char attrs[256] = "";
                sscanf(t.lexeme, "<%63[^ >]", tagName);

                // atributos
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
    }
    fprintf(indexJS, "init();\n");
    fclose(indexJS);
    fprintf(out, "}\n");
    fclose(out);
    printf("✅ output.js generado correctamente\n");
}

int main()
{
    DIR *dir = opendir("components");
    if (!dir) {
        perror("❌ Error al abrir carpeta components");
        return 1;
    }

    struct dirent *archivo;
    char ruta[512];

    while ((archivo = readdir(dir))) {
        // Ignorar . y ..
        if (strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0)
            continue;

        // Verificar extensión .mn
        char *ext = strrchr(archivo->d_name, '.');
        if (ext && strcmp(ext, ".mn") == 0) {
            snprintf(ruta, sizeof(ruta), "components/%s", archivo->d_name);

            printf("📄 Procesando: %s\n", ruta);

            // Resetear tokens antes de cada archivo
            tokenCount = 0;

            // Analizar archivo
            lexer(ruta);

            // Generar output con los tokens cargados
            parse_and_generate();
        }
    }

    closedir(dir);
    return 0;
}
