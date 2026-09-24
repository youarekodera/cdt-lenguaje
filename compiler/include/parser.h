#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <stdbool.h>

// Parser structure
typedef struct {
    Lexer* lexer;
    Token* current_token;
    Token* previous_token;
    bool had_error;
    bool panic_mode;
    char* error_message;
} Parser;

// Parser functions
Parser* parser_create(Lexer* lexer);
void parser_free(Parser* parser);
ASTProgram* parser_parse(Parser* parser);

// Parsing helpers
void parser_advance(Parser* parser);
bool parser_check(Parser* parser, TokenType type);
bool parser_match(Parser* parser, TokenType type);
bool parser_match_multiple(Parser* parser, TokenType* types, size_t count);
Token* parser_consume(Parser* parser, TokenType type, const char* error_message);
void parser_error(Parser* parser, const char* message);
void parser_error_at_token(Parser* parser, Token* token, const char* message);
void parser_synchronize(Parser* parser);

// Declaration parsing
ASTDeclaration* parser_parse_declaration(Parser* parser);
ASTModuleStatement* parser_parse_module(Parser* parser);
ASTImportStatement* parser_parse_import(Parser* parser);
ASTVariableDeclaration* parser_parse_variable_declaration(Parser* parser);
ASTFunctionDeclaration* parser_parse_function_declaration(Parser* parser);
ASTObjectDeclaration* parser_parse_object_declaration(Parser* parser);
ASTContractDeclaration* parser_parse_contract_declaration(Parser* parser);
ASTChoiceDeclaration* parser_parse_choice_declaration(Parser* parser);

// Statement parsing
ASTStatement* parser_parse_statement(Parser* parser);
ASTBlock* parser_parse_block(Parser* parser);
ASTExpressionStatement* parser_parse_expression_statement(Parser* parser);
ASTConditionalStatement* parser_parse_conditional(Parser* parser);
ASTLoopStatement* parser_parse_loop(Parser* parser);
ASTChangeStatement* parser_parse_change(Parser* parser);
ASTRepeatStatement* parser_parse_repeat(Parser* parser);
ASTForStatement* parser_parse_for(Parser* parser);
ASTWhileStatement* parser_parse_while(Parser* parser);
ASTReturnStatement* parser_parse_return(Parser* parser);
ATSTryStatement* parser_parse_try(Parser* parser);
ASTMatchStatement* parser_parse_match(Parser* parser);
ASTRaiseStatement* parser_parse_raise(Parser* parser);

// Expression parsing (precedence climbing)
ASTExpression* parser_parse_expression(Parser* parser);
ASTExpression* parser_parse_assignment(Parser* parser);
ASTExpression* parser_parse_logical_or(Parser* parser);
ASTExpression* parser_parse_logical_and(Parser* parser);
ASTExpression* parser_parse_equality(Parser* parser);
ASTExpression* parser_parse_comparison(Parser* parser);
ASTExpression* parser_parse_term(Parser* parser);
ASTExpression* parser_parse_factor(Parser* parser);
ASTExpression* parser_parse_unary(Parser* parser);
ASTExpression* parser_parse_power(Parser* parser);
ASTExpression* parser_parse_call(Parser* parser);
ASTExpression* parser_parse_primary(Parser* parser);

// Specific expression parsers
ASTExpression* parser_parse_list(Parser* parser);
ASTExpression* parser_parse_map(Parser* parser);
ASTExpression* parser_parse_lambda(Parser* parser);
ASTExpression* parser_parse_if_expression(Parser* parser);
ASTExpression* parser_parse_choose(Parser* parser);

// Utility functions
bool parser_is_at_end(Parser* parser);
Token* parser_peek(Parser* parser);
Token* parser_previous(Parser* parser);
Token* parser_previous_token(Parser* parser);

#endif // PARSER_H
