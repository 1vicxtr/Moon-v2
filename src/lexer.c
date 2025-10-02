#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOK_KEYWORD,
    TOK_IDENTIFIER,
    TOK_SYMBOL,
    TOK_TAG,
    TOK_TEXT,
    TOK_JSBLOCK,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[1024];
} Token;

Token tokens[2048];
int tokenCount = 0;

void addToken(TokenType type, const char *lexeme) {
    if (tokenCount < 2048) {
        tokens[tokenCount].type = type;
        strncpy(tokens[tokenCount].lexeme, lexeme, sizeof(tokens[tokenCount].lexeme) - 1);
        tokens[tokenCount].lexeme[sizeof(tokens[tokenCount].lexeme) - 1] = '\0';
        tokenCount++;
    }
}

void lexer(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("❌ Error al abrir %s\n", filename);
        exit(1);
    }

    int c;
    char buffer[1024];
    int idx = 0;
    int insideTag = 0;
    int insideHTML = 0;

    while ((c = fgetc(f)) != EOF) {
        // ----------- Bloque JS (js(...)) ----------------
        if (!insideHTML && c == 'j') {
            long pos = ftell(f);
            char la1 = fgetc(f);
            char la2 = fgetc(f);
            if (la1 == 's' && la2 == '(') {
                // leer todo hasta el ) de cierre
                idx = 0;
                int parens = 1;
                while ((c = fgetc(f)) != EOF && parens > 0) {
                    if (c == '(') parens++;
                    else if (c == ')') parens--;
                    if (parens > 0) buffer[idx++] = c;
                }
                buffer[idx] = '\0';
                addToken(TOK_JSBLOCK, buffer);
                continue;
            } else {
                fseek(f, pos, SEEK_SET);
            }
        }

        // ----------- Tags <html> o normales ----------------
        if (c == '<') {
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

            if (strcmp(buffer, "<html>") == 0) insideHTML = 1;
            if (strcmp(buffer, "</html>") == 0) insideHTML = 0;

            idx = 0;
            insideTag = 0;
        }
        else {
            buffer[idx++] = c;
        }
    }

    // Guardar último texto
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
