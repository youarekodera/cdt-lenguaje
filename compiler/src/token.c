#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Token* token_create(TokenType type, const char* lexeme, size_t length, int line, int column, const char* filename) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (!token) {
        return NULL;
    }

    token->type = type;
    token->length = length;
    token->line = line;
    token->column = column;
    token->filename = filename;

    if (lexeme && length > 0) {
        token->lexeme = (char*)malloc(length + 1);
        if (!token->lexeme) {
            free(token);
            return NULL;
        }
        strncpy(token->lexeme, lexeme, length);
        token->lexeme[length] = '\0';
    } else {
        token->lexeme = NULL;
    }

    return token;
}

void token_free(Token* token) {
    if (token) {
        if (token->lexeme) {
            free(token->lexeme);
        }
        free(token);
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        // Keywords
        case TOKEN_START: return "START";
        case TOKEN_LET: return "LET";
        case TOKEN_CHANGE: return "CHANGE";
        case TOKEN_FIXED: return "FIXED";
        case TOKEN_AS: return "AS";
        case TOKEN_TO: return "TO";
        case TOKEN_DEFINE: return "DEFINE";
        case TOKEN_USING: return "USING";
        case TOKEN_GIVE: return "GIVE";
        case TOKEN_WHEN: return "WHEN";
        case TOKEN_OTHERWISE: return "OTHERWISE";
        case TOKEN_REPEAT: return "REPEAT";
        case TOKEN_UNTIL: return "UNTIL";
        case TOKEN_FROM: return "FROM";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_EACH: return "EACH";
        case TOKEN_INSIDE: return "INSIDE";
        case TOKEN_OBJECT: return "OBJECT";
        case TOKEN_PROPERTY: return "PROPERTY";
        case TOKEN_CREATE: return "CREATE";
        case TOKEN_ACTION: return "ACTION";
        case TOKEN_SET: return "SET";
        case TOKEN_SELF: return "SELF";
        case TOKEN_CONTRACT: return "CONTRACT";
        case TOKEN_FOLLOWS: return "FOLLOWS";
        case TOKEN_CHOICE: return "CHOICE";
        case TOKEN_ATTEMPT: return "ATTEMPT";
        case TOKEN_RECOVER: return "RECOVER";
        case TOKEN_RAISE: return "RAISE";
        case TOKEN_USE: return "USE";
        case TOKEN_MODULE: return "MODULE";
        case TOKEN_CASE: return "CASE";
        case TOKEN_CHOOSE: return "CHOOSE";
        case TOKEN_MAYBE: return "MAYBE";
        case TOKEN_EXISTS: return "EXISTS";
        case TOKEN_OF: return "OF";
        case TOKEN_NOTHING: return "NOTHING";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_PRINT: return "PRINT";

        // Literals
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_DECIMAL: return "DECIMAL";
        case TOKEN_TEXT: return "TEXT";
        case TOKEN_CHARACTER: return "CHARACTER";
        case TOKEN_BOOLEAN: return "BOOLEAN";

        // Identifiers
        case TOKEN_IDENTIFIER: return "IDENTIFIER";

        // Operators
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";
        case TOKEN_POWER: return "POWER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";

        // Punctuation
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COLON: return "COLON";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_ARROW: return "ARROW";

        // Special
        case TOKEN_INDENT: return "INDENT";
        case TOKEN_DEDENT: return "DEDENT";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";

        // Module separator
        case TOKEN_AT: return "AT";

        // List/Map operations
        case TOKEN_ADD: return "ADD";
        case TOKEN_REMOVE: return "REMOVE";

        // Type keywords
        case TOKEN_TYPE_NUMBER: return "TYPE_NUMBER";
        case TOKEN_TYPE_DECIMAL: return "TYPE_DECIMAL";
        case TOKEN_TYPE_TEXT: return "TYPE_TEXT";
        case TOKEN_TYPE_CHARACTER: return "TYPE_CHARACTER";
        case TOKEN_TYPE_BOOLEAN: return "TYPE_BOOLEAN";
        case TOKEN_TYPE_LIST: return "TYPE_LIST";
        case TOKEN_TYPE_MAP: return "TYPE_MAP";
        case TOKEN_TYPE_SET: return "TYPE_SET";

        default: return "UNKNOWN";
    }
}

bool token_is_keyword(Token* token) {
    if (!token) return false;
    return token->type >= TOKEN_START && token->type <= TOKEN_PRINT;
}

bool token_is_literal(Token* token) {
    if (!token) return false;
    return token->type >= TOKEN_NUMBER && token->type <= TOKEN_BOOLEAN;
}

bool token_is_operator(Token* token) {
    if (!token) return false;
    return token->type >= TOKEN_PLUS && token->type <= TOKEN_GREATER_EQUAL;
}

bool token_is_type(Token* token) {
    if (!token) return false;
    return token->type >= TOKEN_TYPE_NUMBER && token->type <= TOKEN_TYPE_SET;
}
