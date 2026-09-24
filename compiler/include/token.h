#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdbool.h>

// Token types for CodeTime language
typedef enum {
    // Keywords
    TOKEN_START,
    TOKEN_LET,
    TOKEN_CHANGE,
    TOKEN_FIXED,
    TOKEN_AS,
    TOKEN_TO,
    TOKEN_DEFINE,
    TOKEN_USING,
    TOKEN_GIVE,
    TOKEN_WHEN,
    TOKEN_OTHERWISE,
    TOKEN_REPEAT,
    TOKEN_UNTIL,
    TOKEN_FROM,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_EACH,
    TOKEN_INSIDE,
    TOKEN_OBJECT,
    TOKEN_PROPERTY,
    TOKEN_CREATE,
    TOKEN_ACTION,
    TOKEN_SET,
    TOKEN_SELF,
    TOKEN_CONTRACT,
    TOKEN_FOLLOWS,
    TOKEN_CHOICE,
    TOKEN_ATTEMPT,
    TOKEN_RECOVER,
    TOKEN_RAISE,
    TOKEN_USE,
    TOKEN_MODULE,
    TOKEN_CASE,
    TOKEN_CHOOSE,
    TOKEN_MAYBE,
    TOKEN_EXISTS,
    TOKEN_OF,
    TOKEN_NOTHING,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_PRINT,

    // Literals
    TOKEN_NUMBER,
    TOKEN_DECIMAL,
    TOKEN_TEXT,
    TOKEN_CHARACTER,
    TOKEN_BOOLEAN,

    // Identifiers
    TOKEN_IDENTIFIER,

    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_POWER,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,

    // Punctuation
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_COLON,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_ARROW,

    // Special
    TOKEN_INDENT,
    TOKEN_DEDENT,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_ERROR,

    // Module separator
    TOKEN_AT,

    // List/Map operations
    TOKEN_ADD,
    TOKEN_REMOVE,

    // Type keywords
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_DECIMAL,
    TOKEN_TYPE_TEXT,
    TOKEN_TYPE_CHARACTER,
    TOKEN_TYPE_BOOLEAN,
    TOKEN_TYPE_LIST,
    TOKEN_TYPE_MAP,
    TOKEN_TYPE_SET,
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* lexeme;
    size_t length;
    int line;
    int column;
    const char* filename;
} Token;

// Token management functions
Token* token_create(TokenType type, const char* lexeme, size_t length, int line, int column, const char* filename);
void token_free(Token* token);
const char* token_type_name(TokenType type);
bool token_is_keyword(Token* token);
bool token_is_literal(Token* token);
bool token_is_operator(Token* token);
bool token_is_type(Token* token);

#endif // TOKEN_H
