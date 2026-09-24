#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>

// Lexer structure
typedef struct {
    const char* source;
    size_t source_length;
    size_t position;
    int line;
    int column;
    const char* filename;

    // Indentation tracking
    int* indent_stack;
    size_t indent_stack_size;
    size_t indent_stack_capacity;
    int current_indent;

    // Error handling
    bool had_error;
    char* error_message;
} Lexer;

// Lexer functions
Lexer* lexer_create(const char* source, const char* filename);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
Token* lexer_peek_token(Lexer* lexer, size_t offset);

// Helper functions
bool lexer_is_at_end(Lexer* lexer);
char lexer_advance(Lexer* lexer);
char lexer_peek(Lexer* lexer);
char lexer_peek_next(Lexer* lexer);
bool lexer_match(Lexer* lexer, char expected);
void lexer_skip_whitespace(Lexer* lexer);
void lexer_skip_comment(Lexer* lexer);
Token* lexer_scan_token(Lexer* lexer);
Token* lexer_scan_identifier(Lexer* lexer);
Token* lexer_scan_number(Lexer* lexer);
Token* lexer_scan_string(Lexer* lexer);
Token* lexer_scan_character(Lexer* lexer);
Token* lexer_handle_indentation(Lexer* lexer);
void lexer_push_indent(Lexer* lexer, int indent);
int lexer_pop_indent(Lexer* lexer);
TokenType lexer_identifier_type(const char* lexeme, size_t length);

#endif // LEXER_H
