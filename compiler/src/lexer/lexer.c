#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define INDENT_STACK_INITIAL_CAPACITY 16

Lexer* lexer_create(const char* source, const char* filename) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    if (!lexer) {
        return NULL;
    }

    lexer->source = source;
    lexer->source_length = strlen(source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->filename = filename;
    lexer->had_error = false;
    lexer->error_message = NULL;
    lexer->current_indent = 0;

    lexer->indent_stack = (int*)malloc(sizeof(int) * INDENT_STACK_INITIAL_CAPACITY);
    if (!lexer->indent_stack) {
        free(lexer);
        return NULL;
    }
    lexer->indent_stack[0] = 0; // Base indentation level
    lexer->indent_stack_size = 1;
    lexer->indent_stack_capacity = INDENT_STACK_INITIAL_CAPACITY;

    return lexer;
}

void lexer_free(Lexer* lexer) {
    if (lexer) {
        if (lexer->indent_stack) {
            free(lexer->indent_stack);
        }
        if (lexer->error_message) {
            free(lexer->error_message);
        }
        free(lexer);
    }
}

bool lexer_is_at_end(Lexer* lexer) {
    return lexer->position >= lexer->source_length;
}

char lexer_advance(Lexer* lexer) {
    if (lexer_is_at_end(lexer)) {
        return '\0';
    }
    char c = lexer->source[lexer->position++];
    lexer->column++;
    return c;
}

char lexer_peek(Lexer* lexer) {
    if (lexer_is_at_end(lexer)) {
        return '\0';
    }
    return lexer->source[lexer->position];
}

char lexer_peek_next(Lexer* lexer) {
    if (lexer->position + 1 >= lexer->source_length) {
        return '\0';
    }
    return lexer->source[lexer->position + 1];
}

bool lexer_match(Lexer* lexer, char expected) {
    if (lexer_is_at_end(lexer)) {
        return false;
    }
    if (lexer->source[lexer->position] != expected) {
        return false;
    }
    lexer->position++;
    lexer->column++;
    return true;
}

void lexer_skip_whitespace(Lexer* lexer) {
    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r') {
            lexer_advance(lexer);
        } else {
            break;
        }
    }
}

void lexer_skip_comment(Lexer* lexer) {
    // Single-line comment
    if (lexer_match(lexer, '#')) {
        while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != '\n') {
            lexer_advance(lexer);
        }
        return;
    }

    // Multi-line comment
    if (lexer_match(lexer, '#') && lexer_match(lexer, '#') && lexer_match(lexer, '#')) {
        while (!lexer_is_at_end(lexer)) {
            if (lexer_match(lexer, '#') && lexer_match(lexer, '#') && lexer_match(lexer, '#')) {
                return;
            }
            lexer_advance(lexer);
        }
    }
}

void lexer_push_indent(Lexer* lexer, int indent) {
    if (lexer->indent_stack_size >= lexer->indent_stack_capacity) {
        size_t new_capacity = lexer->indent_stack_capacity * 2;
        int* new_stack = (int*)realloc(lexer->indent_stack, sizeof(int) * new_capacity);
        if (!new_stack) {
            return;
        }
        lexer->indent_stack = new_stack;
        lexer->indent_stack_capacity = new_capacity;
    }
    lexer->indent_stack[lexer->indent_stack_size++] = indent;
}

int lexer_pop_indent(Lexer* lexer) {
    if (lexer->indent_stack_size > 1) {
        return lexer->indent_stack[--lexer->indent_stack_size];
    }
    return 0;
}

Token* lexer_handle_indentation(Lexer* lexer) {
    // Count leading spaces/tabs for current line
    int indent = 0;
    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (c == ' ') {
            indent++;
            lexer_advance(lexer);
        } else if (c == '\t') {
            indent += 4; // Assume 4 spaces per tab
            lexer_advance(lexer);
        } else {
            break;
        }
    }

    lexer->current_indent = indent;

    // Skip comments after indentation
    if (lexer_peek(lexer) == '#') {
        lexer_skip_comment(lexer);
        // Skip to next line if we only had a comment
        while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != '\n') {
            lexer_advance(lexer);
        }
        if (lexer_is_at_end(lexer) || lexer_peek(lexer) == '\n') {
            // Empty line with only comment, skip it
            if (lexer_match(lexer, '\n')) {
                lexer->line++;
                lexer->column = 1;
            }
            return NULL;
        }
    }

    int top_indent = lexer->indent_stack[lexer->indent_stack_size - 1];

    if (indent > top_indent) {
        // Increase indentation
        lexer_push_indent(lexer, indent);
        return token_create(TOKEN_INDENT, "", 0, lexer->line, lexer->column, lexer->filename);
    } else if (indent < top_indent) {
        // Decrease indentation (may need multiple DEDENT tokens)
        while (lexer->indent_stack_size > 1 && indent < lexer->indent_stack[lexer->indent_stack_size - 1]) {
            lexer_pop_indent(lexer);
            // Note: We'll need to handle multiple DEDENTs in the main loop
        }
        return token_create(TOKEN_DEDENT, "", 0, lexer->line, lexer->column, lexer->filename);
    }

    // Same indentation level, no token needed
    return NULL;
}

TokenType lexer_identifier_type(const char* lexeme, size_t length) {
    (void)length; // Suppress unused parameter warning
    // Keywords
    if (strcmp(lexeme, "start") == 0) return TOKEN_START;
    if (strcmp(lexeme, "let") == 0) return TOKEN_LET;
    if (strcmp(lexeme, "change") == 0) return TOKEN_CHANGE;
    if (strcmp(lexeme, "fixed") == 0) return TOKEN_FIXED;
    if (strcmp(lexeme, "as") == 0) return TOKEN_AS;
    if (strcmp(lexeme, "to") == 0) return TOKEN_TO;
    if (strcmp(lexeme, "define") == 0) return TOKEN_DEFINE;
    if (strcmp(lexeme, "using") == 0) return TOKEN_USING;
    if (strcmp(lexeme, "give") == 0) return TOKEN_GIVE;
    if (strcmp(lexeme, "when") == 0) return TOKEN_WHEN;
    if (strcmp(lexeme, "otherwise") == 0) return TOKEN_OTHERWISE;
    if (strcmp(lexeme, "repeat") == 0) return TOKEN_REPEAT;
    if (strcmp(lexeme, "until") == 0) return TOKEN_UNTIL;
    if (strcmp(lexeme, "while") == 0) return TOKEN_WHILE;
    if (strcmp(lexeme, "for") == 0) return TOKEN_FOR;
    if (strcmp(lexeme, "each") == 0) return TOKEN_EACH;
    if (strcmp(lexeme, "inside") == 0) return TOKEN_INSIDE;
    if (strcmp(lexeme, "object") == 0) return TOKEN_OBJECT;
    if (strcmp(lexeme, "property") == 0) return TOKEN_PROPERTY;
    if (strcmp(lexeme, "create") == 0) return TOKEN_CREATE;
    if (strcmp(lexeme, "action") == 0) return TOKEN_ACTION;
    if (strcmp(lexeme, "set") == 0) return TOKEN_SET;
    if (strcmp(lexeme, "self") == 0) return TOKEN_SELF;
    if (strcmp(lexeme, "contract") == 0) return TOKEN_CONTRACT;
    if (strcmp(lexeme, "follows") == 0) return TOKEN_FOLLOWS;
    if (strcmp(lexeme, "choice") == 0) return TOKEN_CHOICE;
    if (strcmp(lexeme, "attempt") == 0) return TOKEN_ATTEMPT;
    if (strcmp(lexeme, "recover") == 0) return TOKEN_RECOVER;
    if (strcmp(lexeme, "raise") == 0) return TOKEN_RAISE;
    if (strcmp(lexeme, "use") == 0) return TOKEN_USE;
    if (strcmp(lexeme, "module") == 0) return TOKEN_MODULE;
    if (strcmp(lexeme, "case") == 0) return TOKEN_CASE;
    if (strcmp(lexeme, "choose") == 0) return TOKEN_CHOOSE;
    if (strcmp(lexeme, "maybe") == 0) return TOKEN_MAYBE;
    if (strcmp(lexeme, "exists") == 0) return TOKEN_EXISTS;
    if (strcmp(lexeme, "of") == 0) return TOKEN_OF;
    if (strcmp(lexeme, "nothing") == 0) return TOKEN_NOTHING;
    if (strcmp(lexeme, "true") == 0) return TOKEN_TRUE;
    if (strcmp(lexeme, "false") == 0) return TOKEN_FALSE;
    if (strcmp(lexeme, "and") == 0) return TOKEN_AND;
    if (strcmp(lexeme, "or") == 0) return TOKEN_OR;
    if (strcmp(lexeme, "not") == 0) return TOKEN_NOT;
    if (strcmp(lexeme, "print") == 0) return TOKEN_PRINT;

    // Type keywords
    if (strcmp(lexeme, "number") == 0) return TOKEN_TYPE_NUMBER;
    if (strcmp(lexeme, "decimal") == 0) return TOKEN_TYPE_DECIMAL;
    if (strcmp(lexeme, "text") == 0) return TOKEN_TYPE_TEXT;
    if (strcmp(lexeme, "character") == 0) return TOKEN_TYPE_CHARACTER;
    if (strcmp(lexeme, "boolean") == 0) return TOKEN_TYPE_BOOLEAN;
    if (strcmp(lexeme, "list") == 0) return TOKEN_TYPE_LIST;
    if (strcmp(lexeme, "map") == 0) return TOKEN_TYPE_MAP;
    if (strcmp(lexeme, "set") == 0) return TOKEN_TYPE_SET;

    return TOKEN_IDENTIFIER;
}

Token* lexer_scan_identifier(Lexer* lexer) {
    size_t start = lexer->position - 1;

    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (isalnum(c) || c == '_') {
            lexer_advance(lexer);
        } else {
            break;
        }
    }

    size_t length = lexer->position - start;
    const char* lexeme = &lexer->source[start];

    TokenType type = lexer_identifier_type(lexeme, length);

    return token_create(type, lexeme, length, lexer->line, lexer->column - length, lexer->filename);
}

Token* lexer_scan_number(Lexer* lexer) {
    size_t start = lexer->position - 1;
    bool is_decimal = false;

    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (isdigit(c)) {
            lexer_advance(lexer);
        } else if (c == '.' && !is_decimal) {
            is_decimal = true;
            lexer_advance(lexer);
        } else {
            break;
        }
    }

    size_t length = lexer->position - start;
    const char* lexeme = &lexer->source[start];

    TokenType type = is_decimal ? TOKEN_DECIMAL : TOKEN_NUMBER;
    return token_create(type, lexeme, length, lexer->line, lexer->column - length, lexer->filename);
}

Token* lexer_scan_string(Lexer* lexer) {
    size_t start = lexer->position;
    bool is_multiline = false;

    // Check for multiline string (""")
    if (lexer_match(lexer, '"') && lexer_match(lexer, '"') && lexer_match(lexer, '"')) {
        is_multiline = true;
    }

    while (!lexer_is_at_end(lexer)) {
        if (is_multiline) {
            if (lexer_match(lexer, '"') && lexer_match(lexer, '"') && lexer_match(lexer, '"')) {
                break;
            }
        } else {
            if (lexer_match(lexer, '"')) {
                break;
            }
        }

        char c = lexer_advance(lexer);
        if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        }
    }

    size_t length = lexer->position - start;
    const char* lexeme = &lexer->source[start];

    return token_create(TOKEN_TEXT, lexeme, length, lexer->line, lexer->column - length, lexer->filename);
}

Token* lexer_scan_character(Lexer* lexer) {
    size_t start = lexer->position;
    lexer_advance(lexer); // Skip opening quote

    if (!lexer_is_at_end(lexer)) {
        char c = lexer_advance(lexer);
        if (c == '\\') {
            // Escape sequence
            lexer_advance(lexer);
        }
    }

    lexer_match(lexer, '\''); // Skip closing quote

    size_t length = lexer->position - start;
    const char* lexeme = &lexer->source[start];

    return token_create(TOKEN_CHARACTER, lexeme, length, lexer->line, lexer->column - length, lexer->filename);
}

Token* lexer_scan_token(Lexer* lexer) {
    char c = lexer_advance(lexer);

    switch (c) {
        // Operators
        case '+':
            if (lexer_match(lexer, '=')) {
                return token_create(TOKEN_ADD, "+=", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_PLUS, "+", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '-':
            if (lexer_match(lexer, '>')) {
                return token_create(TOKEN_ARROW, "->", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_MINUS, "-", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '*':
            if (lexer_match(lexer, '*')) {
                return token_create(TOKEN_POWER, "**", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_MULTIPLY, "*", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '/':
            return token_create(TOKEN_DIVIDE, "/", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '%':
            return token_create(TOKEN_MODULO, "%", 1, lexer->line, lexer->column - 1, lexer->filename);

        // Comparison
        case '=':
            if (lexer_match(lexer, '=')) {
                return token_create(TOKEN_EQUAL, "==", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_ASSIGN, "=", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '!':
            if (lexer_match(lexer, '=')) {
                return token_create(TOKEN_NOT_EQUAL, "!=", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_NOT, "!", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '<':
            if (lexer_match(lexer, '=')) {
                return token_create(TOKEN_LESS_EQUAL, "<=", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_LESS, "<", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '>':
            if (lexer_match(lexer, '=')) {
                return token_create(TOKEN_GREATER_EQUAL, ">=", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_GREATER, ">", 1, lexer->line, lexer->column - 1, lexer->filename);

        // Punctuation
        case ',':
            return token_create(TOKEN_COMMA, ",", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '.':
            if (lexer_match(lexer, '.')) {
                return token_create(TOKEN_DOT, "..", 2, lexer->line, lexer->column - 2, lexer->filename);
            }
            return token_create(TOKEN_DOT, ".", 1, lexer->line, lexer->column - 1, lexer->filename);
        case ':':
            return token_create(TOKEN_COLON, ":", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '@':
            return token_create(TOKEN_AT, "@", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '[':
            return token_create(TOKEN_LEFT_BRACKET, "[", 1, lexer->line, lexer->column - 1, lexer->filename);
        case ']':
            return token_create(TOKEN_RIGHT_BRACKET, "]", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '{':
            return token_create(TOKEN_LEFT_BRACE, "{", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '}':
            return token_create(TOKEN_RIGHT_BRACE, "}", 1, lexer->line, lexer->column - 1, lexer->filename);
        case '(':
            return token_create(TOKEN_LEFT_PAREN, "(", 1, lexer->line, lexer->column - 1, lexer->filename);
        case ')':
            return token_create(TOKEN_RIGHT_PAREN, ")", 1, lexer->line, lexer->column - 1, lexer->filename);

        // Strings and characters
        case '"':
            lexer->position--; // Go back to handle in scan_string
            return lexer_scan_string(lexer);
        case '\'':
            return lexer_scan_character(lexer);

        // Newline
        case '\n':
            lexer->line++;
            lexer->column = 1;
            return NULL; // Don't emit newline tokens, use for indentation tracking

        // Skip other whitespace
        case ' ':
        case '\t':
        case '\r':
            return NULL;

        default:
            if (isdigit(c)) {
                lexer->position--;
                return lexer_scan_number(lexer);
            }
            if (isalpha(c) || c == '_') {
                lexer->position--;
                return lexer_scan_identifier(lexer);
            }
            return token_create(TOKEN_ERROR, &c, 1, lexer->line, lexer->column - 1, lexer->filename);
    }
}

Token* lexer_next_token(Lexer* lexer) {
    // Skip whitespace and comments first
    while (!lexer_is_at_end(lexer)) {
        lexer_skip_whitespace(lexer);
        if (lexer_peek(lexer) == '#') {
            lexer_skip_comment(lexer);
        } else {
            break;
        }
    }

    // Handle indentation at the start of lines
    if (lexer->column == 1 || (lexer->column > 1 && lexer->position > 0 && lexer->source[lexer->position - 1] == '\n')) {
        Token* indent_token = lexer_handle_indentation(lexer);
        if (indent_token) {
            return indent_token;
        }
    }

    if (lexer_is_at_end(lexer)) {
        // Emit remaining DEDENT tokens
        while (lexer->indent_stack_size > 1) {
            lexer_pop_indent(lexer);
            return token_create(TOKEN_DEDENT, "", 0, lexer->line, lexer->column, lexer->filename);
        }
        return token_create(TOKEN_EOF, "", 0, lexer->line, lexer->column, lexer->filename);
    }

    return lexer_scan_token(lexer);
}
