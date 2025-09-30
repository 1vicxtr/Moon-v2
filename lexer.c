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
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[512]; // más grande para tags con atributos
} Token;

Token tokens[1024];
int tokenCount = 0;

int is_keyword(const char *word) {
    return strcmp(word, "component") == 0;
}

void add_token(TokenType type, const char *lexeme) {
    tokens[tokenCount].type = type;
    strcpy(tokens[tokenCount].lexeme, lexeme);
    tokenCount++;
}

void lexer(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("No se pudo abrir %s\n", filename); exit(1); }

    int c;
    while ((c = fgetc(f)) != EOF) {
        if (isalpha(c)) {
            char buffer[256];
            int idx = 0;
            buffer[idx++] = c;

            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_'))
                buffer[idx++] = c;
            buffer[idx] = '\0';

            if (is_keyword(buffer))
                add_token(TOK_KEYWORD, buffer);
            else
                add_token(TOK_IDENTIFIER, buffer);

            if (c != EOF) ungetc(c, f);
        } 
        else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ';') {
            char sym[2] = {c, '\0'};
            add_token(TOK_SYMBOL, sym);
        } 
        else if (c == '<') {
            char tag[512];
            int j = 0;
            tag[j++] = c;
            while ((c = fgetc(f)) != EOF && c != '>') tag[j++] = c;
            tag[j++] = '>';
            tag[j] = '\0';
            add_token(TOK_TAG, tag);

            // Capturar texto inmediatamente después del tag
            long pos = ftell(f);
            c = fgetc(f);
            if (c != '<' && c != EOF && c != '{' && c != '}') {
                char text[256];
                int k = 0;
                text[k++] = c;
                while ((c = fgetc(f)) != EOF && c != '<' && c != '{' && c != '}' && c != '\n')
                    text[k++] = c;
                text[k] = '\0';

                // eliminar espacios al inicio y final
                int start = 0, end = k-1;
                while (isspace(text[start])) start++;
                while (end >= start && isspace(text[end])) end--;
                text[end+1] = '\0';

                if (start <= end) add_token(TOK_TEXT, text + start);
            } else if (c != EOF) {
                fseek(f, pos, SEEK_SET);
            }
        }
    }

    add_token(TOK_EOF, "EOF");
    fclose(f);
}
