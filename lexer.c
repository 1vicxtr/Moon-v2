#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { TOK_KEYWORD, TOK_IDENTIFIER, TOK_SYMBOL, TOK_TAG, TOK_TEXT, TOK_EOF } TokenType;

typedef struct {
    TokenType type;
    char lexeme[512];
} Token;

Token tokens[1024];
int tokenCount = 0;

// ---------------- Función para agregar tokens ----------------
void addToken(TokenType type, const char *lexeme) {
    if (tokenCount < 1024) {
        tokens[tokenCount].type = type;
        strncpy(tokens[tokenCount].lexeme, lexeme, sizeof(tokens[tokenCount].lexeme) - 1);
        tokens[tokenCount].lexeme[sizeof(tokens[tokenCount].lexeme) - 1] = '\0';
        tokenCount++;
    }
}

// ---------------- Lexer ----------------
void lexer(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("❌ Error al abrir %s\n", filename);
        exit(1);
    }

    int c;
    char buffer[512];
    int idx = 0;
    int insideTag = 0;

    while ((c = fgetc(f)) != EOF) {
        if (c == '<') {
            // Guardar texto previo antes del tag
            if (idx > 0 && !insideTag) {
                buffer[idx] = '\0';
                int start = 0;
                while (isspace(buffer[start])) start++;
                int end = strlen(buffer) - 1;
                while (end >= start && isspace(buffer[end])) end--;
                if (end >= start) {
                    buffer[end + 1] = '\0';
                    addToken(TOK_TEXT, buffer + start);
                }
                idx = 0;
            }
            insideTag = 1;
            buffer[idx++] = c;
        } 
        else if (c == '>') {
            buffer[idx++] = c;
            buffer[idx] = '\0';
            addToken(TOK_TAG, buffer);
            idx = 0;
            insideTag = 0;
        } 
        else {
            buffer[idx++] = c;
        }
    }

    // Guardar último texto si hay
    if (idx > 0 && !insideTag) {
        buffer[idx] = '\0';
        int start = 0;
        while (isspace(buffer[start])) start++;
        int end = strlen(buffer) - 1;
        while (end >= start && isspace(buffer[end])) end--;
        if (end >= start) {
            buffer[end + 1] = '\0';
            addToken(TOK_TEXT, buffer + start);
        }
    }

    addToken(TOK_EOF, "EOF");
    fclose(f);
}
