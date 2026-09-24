#include "../../compiler/include/lexer.h"
#include "../../compiler/include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_simple_tokens() {
    const char* source = "let x = 42";
    Lexer* lexer = lexer_create(source, "test.cdt");
    assert(lexer != NULL);

    Token* token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_LET);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_IDENTIFIER);
    assert(strcmp(token->lexeme, "x") == 0);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_ASSIGN);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_NUMBER);
    assert(strcmp(token->lexeme, "42") == 0);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_EOF);
    token_free(token);

    lexer_free(lexer);
    printf("test_simple_tokens: PASSED\n");
}

void test_keywords() {
    const char* source = "start define give when otherwise";
    Lexer* lexer = lexer_create(source, "test.cdt");
    assert(lexer != NULL);

    Token* token = lexer_next_token(lexer);
    assert(token->type == TOKEN_START);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token->type == TOKEN_DEFINE);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token->type == TOKEN_GIVE);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token->type == TOKEN_WHEN);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token->type == TOKEN_OTHERWISE);
    token_free(token);

    lexer_free(lexer);
    printf("test_keywords: PASSED\n");
}

void test_strings() {
    const char* source = "\"Hello, World!\"";
    Lexer* lexer = lexer_create(source, "test.cdt");
    assert(lexer != NULL);

    Token* token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_TEXT);
    assert(strncmp(token->lexeme, "\"Hello, World!\"", 15) == 0);
    token_free(token);

    lexer_free(lexer);
    printf("test_strings: PASSED\n");
}

void test_numbers() {
    const char* source = "42 3.14 -10";
    Lexer* lexer = lexer_create(source, "test.cdt");
    assert(lexer != NULL);

    Token* token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_NUMBER);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_DECIMAL);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_MINUS);
    token_free(token);

    token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_NUMBER);
    token_free(token);

    lexer_free(lexer);
    printf("test_numbers: PASSED\n");
}

void test_comments() {
    const char* source = "# This is a comment\nlet x = 42";
    Lexer* lexer = lexer_create(source, "test.cdt");
    assert(lexer != NULL);

    Token* token = lexer_next_token(lexer);
    assert(token != NULL);
    assert(token->type == TOKEN_LET);
    token_free(token);

    lexer_free(lexer);
    printf("test_comments: PASSED\n");
}

int main() {
    printf("Running lexer tests...\n");

    test_simple_tokens();
    test_keywords();
    test_strings();
    test_numbers();
    test_comments();

    printf("All lexer tests passed!\n");
    return 0;
}
