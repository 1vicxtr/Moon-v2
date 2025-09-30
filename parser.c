#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TOK_KEYWORD, TOK_IDENTIFIER, TOK_SYMBOL, TOK_TAG, TOK_TEXT, TOK_EOF } TokenType;
typedef struct { TokenType type; char lexeme[512]; } Token;

extern Token tokens[1024];
extern int tokenCount;
extern void lexer(const char *filename);

typedef struct ElementStack {
    char tagName[64];
    char attributes[256];
    int id;
    struct ElementStack *next;
} ElementStack;

void parse_and_generate() {
    FILE *out = fopen("output.js", "w");
    if (!out) { printf("Error al abrir output.js\n"); exit(1); }

    fprintf(out, "const root = document.body;\n");

    ElementStack *stack = NULL;
    int elemId = 0;

    for (int i = 0; i < tokenCount; i++) {
        Token t = tokens[i];

        if (t.type == TOK_KEYWORD && strcmp(t.lexeme, "component") == 0) {
            fprintf(out, "// Componente detectado\n");
        } else if (t.type == TOK_TAG) {
            if (t.lexeme[1] == '/') {
                // Tag de cierre
                if (stack) stack = stack->next;
            } else {
                // Extraer nombre y atributos
                char tagName[64] = "";
                char attrs[256] = "";
                sscanf(t.lexeme, "<%63[^ >]", tagName);

                // Todo lo que sigue hasta > son atributos
                char *space = strchr(t.lexeme, ' ');
                if (space) strcpy(attrs, space + 1);
                int len = strlen(attrs);
                if (len > 0 && attrs[len-1] == '>') attrs[len-1] = '\0';

                elemId++;
                fprintf(out, "let e%d = document.createElement('%s');\n", elemId, tagName);

                // Aplicar atributos
                if (strlen(attrs) > 0) {
                    char *tokenAttr = strtok(attrs, " ");
                    while (tokenAttr) {
                        char key[64], value[128];
                        if (sscanf(tokenAttr, "%63[^=]=\"%127[^\"]\"", key, value) == 2) {
                            fprintf(out, "e%d.setAttribute(\"%s\", \"%s\");\n", elemId, key, value);
                        }
                        tokenAttr = strtok(NULL, " ");
                    }
                }

                if (stack)
                    fprintf(out, "e%d.appendChild(e%d);\n", stack->id, elemId);
                else
                    fprintf(out, "root.appendChild(e%d);\n", elemId);

                ElementStack *newElem = malloc(sizeof(ElementStack));
                strcpy(newElem->tagName, tagName);
                strcpy(newElem->attributes, attrs);
                newElem->id = elemId;
                newElem->next = stack;
                stack = newElem;
            }
        } else if (t.type == TOK_TEXT) {
            if (stack)
                fprintf(out, "e%d.textContent = \"%s\";\n", stack->id, t.lexeme);
        }
    }

    fclose(out);
    printf("✅ output.js generado correctamente\n");
}

int main() {
    lexer("components/app.mn");
    parse_and_generate();
    return 0;
}
