#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100
#define OUTPUT_BUFFER_SIZE 5000

char output_buffer[OUTPUT_BUFFER_SIZE]; // Store results for Flutter
int buffer_index = 0;

const char *keywords[] = {"if", "else", "while", "int", "float", "return", "for", "void"};
const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

int isKeyword(char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void append_to_output(const char *format, ...) {
    va_list args;                                                                                                                                                                                          
    va_start(args, format);
    buffer_index += vsnprintf(output_buffer + buffer_index, OUTPUT_BUFFER_SIZE - buffer_index, format, args);
    va_end(args);
}

void lexer(const char *code) {
    char token[MAX_TOKEN_LEN];
    int token_index = 0;
    buffer_index = 0;

    int i = 0;
    while (code[i] != '\0') {
        char ch = code[i];

        if (isspace(ch)) {
            i++;
            continue;
        }

        if (isalpha(ch)) {
            token_index = 0;
            while (isalnum(ch) || ch == '_') {
                token[token_index++] = ch;
                ch = code[++i];
            }
            token[token_index] = '\0';

            if (isKeyword(token))
                append_to_output("<KEYWORD, %s>\n", token);
            else
                append_to_output("<IDENTIFIER, %s>\n", token);
        } else if (isdigit(ch)) {
            token_index = 0;
            while (isdigit(ch)) {
                token[token_index++] = ch;
                ch = code[++i];
            }
            token[token_index] = '\0';
            append_to_output("<NUMBER, %s>\n", token);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '>' || ch == '<') {
            append_to_output("<OPERATOR, %c>\n", ch);
            i++;
        } else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}') {
            append_to_output("<SYMBOL, %c>\n", ch);
            i++;
        } else {
            append_to_output("<UNKNOWN, %c>\n", ch);
            i++;
        }
    }
}

// Exported function for Flutter to call
char* analyze_code(const char *code) {
    lexer(code);
    return output_buffer;
}
