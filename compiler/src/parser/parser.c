#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef strdup
#define strdup(s) (strcpy((char*)malloc(strlen(s) + 1), s))
#endif

Parser* parser_create(Lexer* lexer) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    if (!parser) {
        return NULL;
    }

    parser->lexer = lexer;
    parser->current_token = NULL;
    parser->previous_token = NULL;
    parser->had_error = false;
    parser->panic_mode = false;
    parser->error_message = NULL;

    // Initialize with first token
    parser_advance(parser);

    return parser;
}

void parser_free(Parser* parser) {
    if (parser) {
        if (parser->current_token) {
            token_free(parser->current_token);
        }
        if (parser->previous_token) {
            token_free(parser->previous_token);
        }
        if (parser->error_message) {
            free(parser->error_message);
        }
        free(parser);
    }
}

void parser_advance(Parser* parser) {
    if (parser->current_token) {
        if (parser->previous_token) {
            token_free(parser->previous_token);
        }
        parser->previous_token = parser->current_token;
    }

    Token* token = lexer_next_token(parser->lexer);
    while (token && token->type == TOKEN_NEWLINE) {
        token_free(token);
        token = lexer_next_token(parser->lexer);
    }

    parser->current_token = token;
}

bool parser_check(Parser* parser, TokenType type) {
    if (!parser->current_token) {
        return false;
    }
    return parser->current_token->type == type;
}

bool parser_match(Parser* parser, TokenType type) {
    if (parser_check(parser, type)) {
        parser_advance(parser);
        return true;
    }
    return false;
}

bool parser_match_multiple(Parser* parser, TokenType* types, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (parser_check(parser, types[i])) {
            parser_advance(parser);
            return true;
        }
    }
    return false;
}

Token* parser_consume(Parser* parser, TokenType type, const char* error_message) {
    if (parser_check(parser, type)) {
        Token* token = parser->current_token;
        parser_advance(parser);
        return token;
    }

    parser_error(parser, error_message);
    return NULL;
}

void parser_error(Parser* parser, const char* message) {
    if (parser->panic_mode) {
        return;
    }

    parser->panic_mode = true;
    parser->had_error = true;

    if (parser->error_message) {
        free(parser->error_message);
    }

    parser->error_message = strdup(message);

    if (parser->current_token) {
        fprintf(stderr, "Error at line %d, column %d: %s\n",
                parser->current_token->line,
                parser->current_token->column,
                message);
    } else {
        fprintf(stderr, "Error: %s\n", message);
    }
}

void parser_error_at_token(Parser* parser, Token* token, const char* message) {
    if (parser->panic_mode) {
        return;
    }

    parser->panic_mode = true;
    parser->had_error = true;

    if (parser->error_message) {
        free(parser->error_message);
    }

    parser->error_message = strdup(message);

    fprintf(stderr, "Error at line %d, column %d: %s\n",
            token->line,
            token->column,
            message);
}

void parser_synchronize(Parser* parser) {
    parser->panic_mode = false;

    while (!parser_is_at_end(parser)) {
        if (parser->previous_token && parser->previous_token->type == TOKEN_NEWLINE) {
            return;
        }

        TokenType types[] = {
            TOKEN_START, TOKEN_LET, TOKEN_CHANGE, TOKEN_FIXED,
            TOKEN_DEFINE, TOKEN_OBJECT, TOKEN_CONTRACT, TOKEN_CHOICE,
            TOKEN_USE, TOKEN_MODULE, TOKEN_WHEN, TOKEN_REPEAT,
            TOKEN_WHILE, TOKEN_FOR, TOKEN_ATTEMPT
        };

        if (parser_match_multiple(parser, types, sizeof(types) / sizeof(TokenType))) {
            return;
        }

        parser_advance(parser);
    }
}

bool parser_is_at_end(Parser* parser) {
    return parser_check(parser, TOKEN_EOF) || parser->current_token == NULL;
}

Token* parser_peek(Parser* parser) {
    return parser->current_token;
}

Token* parser_previous(Parser* parser) {
    return parser->previous_token;
}

Token* parser_previous_token(Parser* parser) {
    return parser->previous_token;
}

ASTProgram* parser_parse(Parser* parser) {
    ASTProgram* program = ast_program_create();
    if (!program) {
        return NULL;
    }

    // Parse module statement if present
    if (parser_match(parser, TOKEN_MODULE)) {
        Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected module name after 'module'");
        if (name_token) {
            program->module = (ASTModuleStatement*)malloc(sizeof(ASTModuleStatement));
            program->module->base.type = NODE_MODULE;
            program->module->base.line = name_token->line;
            program->module->base.column = name_token->column;
            program->module->base.filename = name_token->filename;
            program->module->name = name_token->lexeme ? strdup(name_token->lexeme) : NULL;
        }
    }

    // Parse imports
    while (parser_match(parser, TOKEN_USE)) {
        ASTImportStatement* import = parser_parse_import(parser);
        if (import) {
            // Add to imports array (simplified - should use dynamic array)
            program->import_count++;
        }
    }

    // Parse declarations
    while (!parser_is_at_end(parser)) {
        ASTDeclaration* decl = parser_parse_declaration(parser);
        if (decl) {
            // Add to declarations array (simplified)
            program->declaration_count++;
        }

        if (parser->panic_mode) {
            parser_synchronize(parser);
        }
    }

    return program;
}

ASTModuleStatement* parser_parse_module(Parser* parser) {
    // Already handled in parser_parse
    (void)parser; // Suppress unused parameter warning
    return NULL;
}

ASTImportStatement* parser_parse_import(Parser* parser) {
    ASTImportStatement* import = (ASTImportStatement*)malloc(sizeof(ASTImportStatement));
    if (!import) {
        return NULL;
    }

    Token* module_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected module name after 'use'");
    if (module_token) {
        import->base.type = NODE_IMPORT;
        import->base.line = module_token->line;
        import->base.column = module_token->column;
        import->base.filename = module_token->filename;
        import->module_name = module_token->lexeme ? strdup(module_token->lexeme) : NULL;
        import->alias = NULL;
        import->imports = NULL;
        import->import_count = 0;

        // Check for alias
        if (parser_match(parser, TOKEN_AS)) {
            Token* alias_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected alias after 'as'");
            if (alias_token) {
                import->alias = alias_token->lexeme ? strdup(alias_token->lexeme) : NULL;
            }
        }
    } else {
        free(import);
        return NULL;
    }

    return import;
}

ASTDeclaration* parser_parse_declaration(Parser* parser) {
    if (parser_match(parser, TOKEN_LET) || parser_match(parser, TOKEN_FIXED)) {
        return (ASTDeclaration*)parser_parse_variable_declaration(parser);
    }

    if (parser_match(parser, TOKEN_DEFINE)) {
        return (ASTDeclaration*)parser_parse_function_declaration(parser);
    }

    if (parser_match(parser, TOKEN_OBJECT)) {
        return (ASTDeclaration*)parser_parse_object_declaration(parser);
    }

    if (parser_match(parser, TOKEN_CONTRACT)) {
        return (ASTDeclaration*)parser_parse_contract_declaration(parser);
    }

    if (parser_match(parser, TOKEN_CHOICE)) {
        return (ASTDeclaration*)parser_parse_choice_declaration(parser);
    }

    if (parser_match(parser, TOKEN_START)) {
        // Special entry point - parse as a block
        ASTBlock* block = parser_parse_block(parser);
        if (block) {
            // Create a simple function-like structure for the start block
            ASTFunctionDeclaration* start_func = (ASTFunctionDeclaration*)malloc(sizeof(ASTFunctionDeclaration));
            if (!start_func) {
                ast_block_free(block);
                return NULL;
            }

            start_func->base.type = NODE_FUNCTION_DECLARATION;
            start_func->base.line = parser_previous(parser) ? parser_previous(parser)->line : 0;
            start_func->base.column = parser_previous(parser) ? parser_previous(parser)->column : 0;
            start_func->base.filename = parser_previous(parser) ? parser_previous(parser)->filename : NULL;
            start_func->name = strdup("main");
            start_func->parameters = NULL;
            start_func->parameter_count = 0;
            start_func->return_type = TOKEN_IDENTIFIER;
            start_func->body = block;

            return (ASTDeclaration*)start_func;
        }
    }

    parser_error(parser, "Expected declaration");
    return NULL;
}

ASTVariableDeclaration* parser_parse_variable_declaration(Parser* parser) {
    Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected variable name");
    if (!name_token) {
        return NULL;
    }

    TokenType type_annotation = TOKEN_IDENTIFIER; // Default (no type)

    if (parser_match(parser, TOKEN_AS)) {
        Token* type_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected type after 'as'");
        if (type_token) {
            // Convert identifier to type token
            if (strcmp(type_token->lexeme, "number") == 0) {
                type_annotation = TOKEN_TYPE_NUMBER;
            } else if (strcmp(type_token->lexeme, "decimal") == 0) {
                type_annotation = TOKEN_TYPE_DECIMAL;
            } else if (strcmp(type_token->lexeme, "text") == 0) {
                type_annotation = TOKEN_TYPE_TEXT;
            } else if (strcmp(type_token->lexeme, "boolean") == 0) {
                type_annotation = TOKEN_TYPE_BOOLEAN;
            }
        }
    }

    ASTExpression* initializer = NULL;
    if (parser_match(parser, TOKEN_ASSIGN)) {
        initializer = parser_parse_expression(parser);
    }

    // Determine if this is a constant (need to check the LET/FIXED token that was matched before)
    // For simplicity, we'll assume all declarations are mutable unless explicitly marked
    bool is_constant = false;

    return ast_variable_declaration_create(name_token->lexeme, type_annotation, initializer, !is_constant, is_constant);
}

ASTFunctionDeclaration* parser_parse_function_declaration(Parser* parser) {
    Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected function name");
    if (!name_token) {
        return NULL;
    }

    if (!parser_match(parser, TOKEN_USING)) {
        parser_error(parser, "Expected 'using' after function name");
        return NULL;
    }

    // Parse parameters (simplified - should handle multiple parameters)
    ASTParameter** parameters = NULL;
    size_t param_count = 0;

    // First parameter
    Token* param_name = parser_consume(parser, TOKEN_IDENTIFIER, "Expected parameter name");
    if (param_name) {
        if (parser_match(parser, TOKEN_AS)) {
            Token* param_type = parser_consume(parser, TOKEN_IDENTIFIER, "Expected parameter type");
            (void)param_type; // Suppress unused variable warning
            // Store parameter (simplified)
            param_count++;
        }

        // Check for "and" for more parameters
        while (parser_match(parser, TOKEN_AND)) {
            Token* next_param = parser_consume(parser, TOKEN_IDENTIFIER, "Expected parameter name");
            if (next_param && parser_match(parser, TOKEN_AS)) {
                Token* next_type = parser_consume(parser, TOKEN_IDENTIFIER, "Expected parameter type");
                (void)next_type; // Suppress unused variable warning
                param_count++;
            }
        }
    }

    TokenType return_type = TOKEN_IDENTIFIER; // Default return type

    ASTBlock* body = parser_parse_block(parser);

    return ast_function_declaration_create(name_token->lexeme, parameters, param_count, return_type, body);
}

ASTObjectDeclaration* parser_parse_object_declaration(Parser* parser) {
    Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected object name");
    if (!name_token) {
        return NULL;
    }

    ASTObjectDeclaration* obj_decl = (ASTObjectDeclaration*)malloc(sizeof(ASTObjectDeclaration));
    if (!obj_decl) {
        return NULL;
    }

    obj_decl->base.type = NODE_OBJECT_DECLARATION;
    obj_decl->base.line = name_token->line;
    obj_decl->base.column = name_token->column;
    obj_decl->base.filename = name_token->filename;
    obj_decl->name = name_token->lexeme ? strdup(name_token->lexeme) : NULL;
    obj_decl->contracts = NULL;
    obj_decl->contract_count = 0;
    obj_decl->properties = NULL;
    obj_decl->property_count = 0;
    obj_decl->actions = NULL;
    obj_decl->action_count = 0;

    // Check for contract implementation
    if (parser_match(parser, TOKEN_FOLLOWS)) {
        Token* contract_name = parser_consume(parser, TOKEN_IDENTIFIER, "Expected contract name");
        if (contract_name) {
            // Store contract (simplified)
        }
    }

    // Parse body
    ASTBlock* body = parser_parse_block(parser);
    if (body) {
        // Parse properties and actions from block
        // This is simplified - a full implementation would iterate through the block
    }

    return obj_decl;
}

ASTContractDeclaration* parser_parse_contract_declaration(Parser* parser) {
    Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected contract name");
    if (!name_token) {
        return NULL;
    }

    ASTContractDeclaration* contract = (ASTContractDeclaration*)malloc(sizeof(ASTContractDeclaration));
    if (!contract) {
        return NULL;
    }

    contract->base.type = NODE_CONTRACT_DECLARATION;
    contract->base.line = name_token->line;
    contract->base.column = name_token->column;
    contract->base.filename = name_token->filename;
    contract->name = name_token->lexeme ? strdup(name_token->lexeme) : NULL;
    contract->actions = NULL;
    contract->action_count = 0;

    // Parse body
    ASTBlock* body = parser_parse_block(parser);
    if (body) {
        // Parse action declarations from block
    }

    return contract;
}

ASTChoiceDeclaration* parser_parse_choice_declaration(Parser* parser) {
    Token* name_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected choice name");
    if (!name_token) {
        return NULL;
    }

    ASTChoiceDeclaration* choice = (ASTChoiceDeclaration*)malloc(sizeof(ASTChoiceDeclaration));
    if (!choice) {
        return NULL;
    }

    choice->base.type = NODE_CHOICE_DECLARATION;
    choice->base.line = name_token->line;
    choice->base.column = name_token->column;
    choice->base.filename = name_token->filename;
    choice->name = name_token->lexeme ? strdup(name_token->lexeme) : NULL;
    choice->variants = NULL;
    choice->variant_count = 0;

    // Parse variants
    while (!parser_is_at_end(parser) && !parser_check(parser, TOKEN_DEDENT)) {
        Token* variant_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected variant name");
        if (variant_token) {
            // Add variant (simplified)
            choice->variant_count++;
        }
    }

    return choice;
}

ASTStatement* parser_parse_statement(Parser* parser) {
    if (parser_match(parser, TOKEN_LET) || parser_match(parser, TOKEN_FIXED)) {
        return (ASTStatement*)parser_parse_variable_declaration(parser);
    }

    if (parser_match(parser, TOKEN_CHANGE)) {
        return (ASTStatement*)parser_parse_change(parser);
    }

    if (parser_match(parser, TOKEN_WHEN)) {
        return (ASTStatement*)parser_parse_conditional(parser);
    }

    if (parser_match(parser, TOKEN_REPEAT)) {
        return (ASTStatement*)parser_parse_repeat(parser);
    }

    if (parser_match(parser, TOKEN_WHILE)) {
        return (ASTStatement*)parser_parse_while(parser);
    }

    if (parser_match(parser, TOKEN_FOR)) {
        return (ASTStatement*)parser_parse_for(parser);
    }

    if (parser_match(parser, TOKEN_GIVE)) {
        return (ASTStatement*)parser_parse_return(parser);
    }

    if (parser_match(parser, TOKEN_ATTEMPT)) {
        return (ASTStatement*)parser_parse_try(parser);
    }

    if (parser_match(parser, TOKEN_CHOOSE)) {
        return (ASTStatement*)parser_parse_match(parser);
    }

    // Default to expression statement
    return (ASTStatement*)parser_parse_expression_statement(parser);
}

ASTBlock* parser_parse_block(Parser* parser) {
    ASTBlock* block = ast_block_create();
    if (!block) {
        return NULL;
    }

    // Expect indentation
    if (!parser_match(parser, TOKEN_INDENT)) {
        parser_error(parser, "Expected indented block");
        return block;
    }

    // Parse statements until dedent
    while (!parser_is_at_end(parser) && !parser_check(parser, TOKEN_DEDENT)) {
        ASTStatement* stmt = parser_parse_statement(parser);
        if (stmt) {
            // Add to block (simplified)
            block->statement_count++;
        }

        if (parser->panic_mode) {
            parser_synchronize(parser);
        }
    }

    // Consume dedent
    parser_match(parser, TOKEN_DEDENT);

    return block;
}

ASTChangeStatement* parser_parse_change(Parser* parser) {
    Token* var_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected variable name after 'change'");
    if (!var_token) {
        return NULL;
    }

    if (!parser_match(parser, TOKEN_TO)) {
        parser_error(parser, "Expected 'to' after variable name");
        return NULL;
    }

    ASTExpression* value = parser_parse_expression(parser);
    if (!value) {
        return NULL;
    }

    ASTChangeStatement* stmt = (ASTChangeStatement*)malloc(sizeof(ASTChangeStatement));
    if (!stmt) {
        if (value) ast_expression_free(value);
        return NULL;
    }

    stmt->base.type = NODE_CHANGE_STATEMENT;
    stmt->base.line = var_token->line;
    stmt->base.column = var_token->column;
    stmt->base.filename = var_token->filename;
    stmt->variable_name = var_token->lexeme ? strdup(var_token->lexeme) : NULL;
    stmt->value = value;

    return stmt;
}

ASTExpressionStatement* parser_parse_expression_statement(Parser* parser) {
    ASTExpression* expr = parser_parse_expression(parser);
    if (!expr) {
        return NULL;
    }

    ASTExpressionStatement* stmt = (ASTExpressionStatement*)malloc(sizeof(ASTExpressionStatement));
    if (!stmt) {
        ast_expression_free(expr);
        return NULL;
    }

    stmt->base.type = NODE_EXPRESSION_STATEMENT;
    stmt->base.line = expr->base.line;
    stmt->base.column = expr->base.column;
    stmt->base.filename = expr->base.filename;
    stmt->expression = expr;

    return stmt;
}

ASTConditionalStatement* parser_parse_conditional(Parser* parser) {
    ASTExpression* condition = parser_parse_expression(parser);
    if (!condition) {
        return NULL;
    }

    ASTBlock* then_block = parser_parse_block(parser);

    ASTConditionalStatement* stmt = (ASTConditionalStatement*)malloc(sizeof(ASTConditionalStatement));
    if (!stmt) {
        if (condition) ast_expression_free(condition);
        if (then_block) ast_block_free(then_block);
        return NULL;
    }

    stmt->base.type = NODE_CONDITIONAL_STATEMENT;
    stmt->base.line = condition->base.line;
    stmt->base.column = condition->base.column;
    stmt->base.filename = condition->base.filename;
    stmt->condition = condition;
    stmt->then_block = then_block;
    stmt->elif_branch = NULL;
    stmt->else_block = NULL;

    // Check for elif/otherwise
    if (parser_match(parser, TOKEN_OTHERWISE)) {
        if (parser_match(parser, TOKEN_WHEN)) {
            // Elif branch
            stmt->elif_branch = parser_parse_conditional(parser);
        } else {
            // Else block
            stmt->else_block = parser_parse_block(parser);
        }
    }

    return stmt;
}

ASTRepeatStatement* parser_parse_repeat(Parser* parser) {
    Token* var_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected variable name after 'repeat'");
    if (!var_token) {
        return NULL;
    }

    if (!parser_match(parser, TOKEN_FROM)) {
        parser_error(parser, "Expected 'from' after variable name");
        return NULL;
    }

    ASTExpression* start = parser_parse_expression(parser);
    if (!start) {
        return NULL;
    }

    if (!parser_match(parser, TOKEN_UNTIL)) {
        parser_error(parser, "Expected 'until' after start value");
        return NULL;
    }

    ASTExpression* end = parser_parse_expression(parser);
    if (!end) {
        return NULL;
    }

    ASTBlock* body = parser_parse_block(parser);

    ASTRepeatStatement* stmt = (ASTRepeatStatement*)malloc(sizeof(ASTRepeatStatement));
    if (!stmt) {
        if (start) ast_expression_free(start);
        if (end) ast_expression_free(end);
        if (body) ast_block_free(body);
        return NULL;
    }

    stmt->base.type = NODE_REPEAT_STATEMENT;
    stmt->base.line = var_token->line;
    stmt->base.column = var_token->column;
    stmt->base.filename = var_token->filename;
    stmt->variable_name = var_token->lexeme ? strdup(var_token->lexeme) : NULL;
    stmt->start = start;
    stmt->end = end;
    stmt->body = body;

    return stmt;
}

ASTForStatement* parser_parse_for(Parser* parser) {
    if (!parser_match(parser, TOKEN_EACH)) {
        parser_error(parser, "Expected 'each' after 'for'");
        return NULL;
    }

    Token* var_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected variable name after 'each'");
    if (!var_token) {
        return NULL;
    }

    if (!parser_match(parser, TOKEN_INSIDE)) {
        parser_error(parser, "Expected 'inside' after variable name");
        return NULL;
    }

    ASTExpression* collection = parser_parse_expression(parser);
    if (!collection) {
        return NULL;
    }

    ASTBlock* body = parser_parse_block(parser);

    ASTForStatement* stmt = (ASTForStatement*)malloc(sizeof(ASTForStatement));
    if (!stmt) {
        if (collection) ast_expression_free(collection);
        if (body) ast_block_free(body);
        return NULL;
    }

    stmt->base.type = NODE_FOR_STATEMENT;
    stmt->base.line = var_token->line;
    stmt->base.column = var_token->column;
    stmt->base.filename = var_token->filename;
    stmt->variable_name = var_token->lexeme ? strdup(var_token->lexeme) : NULL;
    stmt->collection = collection;
    stmt->body = body;

    return stmt;
}

ASTWhileStatement* parser_parse_while(Parser* parser) {
    ASTExpression* condition = parser_parse_expression(parser);
    if (!condition) {
        return NULL;
    }

    ASTBlock* body = parser_parse_block(parser);

    ASTWhileStatement* stmt = (ASTWhileStatement*)malloc(sizeof(ASTWhileStatement));
    if (!stmt) {
        if (condition) ast_expression_free(condition);
        if (body) ast_block_free(body);
        return NULL;
    }

    stmt->base.type = NODE_WHILE_STATEMENT;
    stmt->base.line = condition->base.line;
    stmt->base.column = condition->base.column;
    stmt->base.filename = condition->base.filename;
    stmt->condition = condition;
    stmt->body = body;

    return stmt;
}

ASTReturnStatement* parser_parse_return(Parser* parser) {
    ASTExpression* value = parser_parse_expression(parser);

    ASTReturnStatement* stmt = (ASTReturnStatement*)malloc(sizeof(ASTReturnStatement));
    if (!stmt) {
        if (value) ast_expression_free(value);
        return NULL;
    }

    stmt->base.type = NODE_RETURN_STATEMENT;
    Token* prev = parser_previous(parser);
    stmt->base.line = prev ? prev->line : 0;
    stmt->base.column = prev ? prev->column : 0;
    stmt->base.filename = prev ? prev->filename : NULL;
    stmt->value = value;

    return stmt;
}

ATSTryStatement* parser_parse_try(Parser* parser) {
    ASTBlock* try_block = parser_parse_block(parser);

    if (!parser_match(parser, TOKEN_RECOVER)) {
        parser_error(parser, "Expected 'recover' after try block");
        if (try_block) ast_block_free(try_block);
        return NULL;
    }

    Token* error_token = parser_consume(parser, TOKEN_IDENTIFIER, "Expected error variable name");
    char* error_var = error_token ? strdup(error_token->lexeme) : NULL;

    ASTBlock* catch_block = parser_parse_block(parser);

    ATSTryStatement* stmt = (ATSTryStatement*)malloc(sizeof(ATSTryStatement));
    if (!stmt) {
        if (try_block) ast_block_free(try_block);
        if (error_var) free(error_var);
        if (catch_block) ast_block_free(catch_block);
        return NULL;
    }

    stmt->base.type = NODE_TRY_STATEMENT;
    stmt->base.line = try_block->base.line;
    stmt->base.column = try_block->base.column;
    stmt->base.filename = try_block->base.filename;
    stmt->try_block = try_block;
    stmt->error_variable = error_var;
    stmt->catch_block = catch_block;

    return stmt;
}

ASTMatchStatement* parser_parse_match(Parser* parser) {
    ASTExpression* value = parser_parse_expression(parser);
    if (!value) {
        return NULL;
    }

    ASTMatchStatement* stmt = (ASTMatchStatement*)malloc(sizeof(ASTMatchStatement));
    if (!stmt) {
        if (value) ast_expression_free(value);
        return NULL;
    }

    stmt->base.type = NODE_MATCH_STATEMENT;
    stmt->base.line = value->base.line;
    stmt->base.column = value->base.column;
    stmt->base.filename = value->base.filename;
    stmt->value = value;
    stmt->cases = NULL;
    stmt->case_count = 0;
    stmt->default_case = NULL;

    // Parse cases
    while (parser_match(parser, TOKEN_CASE)) {
        ASTExpression* pattern = parser_parse_expression(parser);
        ASTBlock* body = parser_parse_block(parser);
        (void)pattern; // Suppress unused variable warning
        (void)body; // Suppress unused variable warning
        // Add case (simplified)
        stmt->case_count++;
    }

    // Check for default case
    if (parser_match(parser, TOKEN_OTHERWISE)) {
        stmt->default_case = parser_parse_block(parser);
    }

    return stmt;
}

ASTRaiseStatement* parser_parse_raise(Parser* parser) {
    ASTExpression* error = parser_parse_expression(parser);
    if (!error) {
        return NULL;
    }

    ASTRaiseStatement* stmt = (ASTRaiseStatement*)malloc(sizeof(ASTRaiseStatement));
    if (!stmt) {
        if (error) ast_expression_free(error);
        return NULL;
    }

    stmt->base.type = NODE_RAISE_STATEMENT;
    stmt->base.line = error->base.line;
    stmt->base.column = error->base.column;
    stmt->base.filename = error->base.filename;
    stmt->error = error;

    return stmt;
}

ASTExpression* parser_parse_expression(Parser* parser) {
    return parser_parse_assignment(parser);
}

ASTExpression* parser_parse_assignment(Parser* parser) {
    ASTExpression* expr = parser_parse_logical_or(parser);

    if (parser_match(parser, TOKEN_ASSIGN)) {
        ASTExpression* value = parser_parse_assignment(parser);
        (void)value; // Suppress unused variable warning
        // Create assignment expression (simplified)
        return expr;
    }

    return expr;
}

ASTExpression* parser_parse_logical_or(Parser* parser) {
    ASTExpression* expr = parser_parse_logical_and(parser);

    while (parser_match(parser, TOKEN_OR)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_OR;
        ASTExpression* right = parser_parse_logical_and(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_logical_and(Parser* parser) {
    ASTExpression* expr = parser_parse_equality(parser);

    while (parser_match(parser, TOKEN_AND)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_AND;
        ASTExpression* right = parser_parse_equality(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_equality(Parser* parser) {
    ASTExpression* expr = parser_parse_comparison(parser);

    while (parser_match(parser, TOKEN_EQUAL) || parser_match(parser, TOKEN_NOT_EQUAL)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_EQUAL;
        ASTExpression* right = parser_parse_comparison(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_comparison(Parser* parser) {
    ASTExpression* expr = parser_parse_term(parser);

    while (parser_match(parser, TOKEN_LESS) || parser_match(parser, TOKEN_LESS_EQUAL) ||
           parser_match(parser, TOKEN_GREATER) || parser_match(parser, TOKEN_GREATER_EQUAL)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_LESS;
        ASTExpression* right = parser_parse_term(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_term(Parser* parser) {
    ASTExpression* expr = parser_parse_factor(parser);

    while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_PLUS;
        ASTExpression* right = parser_parse_factor(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_factor(Parser* parser) {
    ASTExpression* expr = parser_parse_unary(parser);

    while (parser_match(parser, TOKEN_MULTIPLY) || parser_match(parser, TOKEN_DIVIDE) ||
           parser_match(parser, TOKEN_MODULO)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_MULTIPLY;
        ASTExpression* right = parser_parse_unary(parser);
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_unary(Parser* parser) {
    if (parser_match(parser, TOKEN_NOT) || parser_match(parser, TOKEN_MINUS)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_NOT;
        ASTExpression* operand = parser_parse_unary(parser);

        ASTUnaryExpression* expr = (ASTUnaryExpression*)malloc(sizeof(ASTUnaryExpression));
        if (!expr) {
            ast_expression_free(operand);
            return NULL;
        }

        expr->base.type = NODE_UNARY_EXPRESSION;
        expr->base.line = prev ? prev->line : 0;
        expr->base.column = prev ? prev->column : 0;
        expr->base.filename = prev ? prev->filename : NULL;
        expr->operator = op;
        expr->operand = operand;

        return (ASTExpression*)expr;
    }

    return parser_parse_power(parser);
}

ASTExpression* parser_parse_power(Parser* parser) {
    ASTExpression* expr = parser_parse_call(parser);

    if (parser_match(parser, TOKEN_POWER)) {
        Token* prev = parser_previous(parser);
        TokenType op = prev ? prev->type : TOKEN_POWER;
        ASTExpression* right = parser_parse_unary(parser); // Power is right-associative
        expr = ast_binary_expression_create(expr, op, right);
    }

    return expr;
}

ASTExpression* parser_parse_call(Parser* parser) {
    ASTExpression* expr = parser_parse_primary(parser);

    while (true) {
        if (parser_match(parser, TOKEN_LEFT_PAREN)) {
            // Function call
            ASTExpression** arguments = NULL;
            size_t arg_count = 0;

            if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
                do {
                    ASTExpression* arg = parser_parse_expression(parser);
                    if (arg) {
                        arg_count++;
                    }
                } while (parser_match(parser, TOKEN_COMMA));
            }

            parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after arguments");

            ASTCallExpression* call = (ASTCallExpression*)malloc(sizeof(ASTCallExpression));
            if (!call) {
                ast_expression_free(expr);
                return NULL;
            }

            call->base.type = NODE_CALL_EXPRESSION;
            call->base.line = expr->base.line;
            call->base.column = expr->base.column;
            call->base.filename = expr->base.filename;
            call->callee = expr;
            call->arguments = arguments;
            call->argument_count = arg_count;

            expr = (ASTExpression*)call;
        } else if (parser_match(parser, TOKEN_LEFT_BRACKET)) {
            // Index access
            ASTExpression* index = parser_parse_expression(parser);
            parser_consume(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after index");

            ASTIndexExpression* idx_expr = (ASTIndexExpression*)malloc(sizeof(ASTIndexExpression));
            if (!idx_expr) {
                ast_expression_free(expr);
                if (index) ast_expression_free(index);
                return NULL;
            }

            idx_expr->base.type = NODE_INDEX_EXPRESSION;
            idx_expr->base.line = expr->base.line;
            idx_expr->base.column = expr->base.column;
            idx_expr->base.filename = expr->base.filename;
            idx_expr->object = expr;
            idx_expr->index = index;

            expr = (ASTExpression*)idx_expr;
        } else if (parser_match(parser, TOKEN_DOT)) {
            // Member access
            Token* member = parser_consume(parser, TOKEN_IDENTIFIER, "Expected property name after '.'");
            if (!member) {
                ast_expression_free(expr);
                return NULL;
            }

            ASTMemberExpression* member_expr = (ASTMemberExpression*)malloc(sizeof(ASTMemberExpression));
            if (!member_expr) {
                ast_expression_free(expr);
                return NULL;
            }

            member_expr->base.type = NODE_MEMBER_EXPRESSION;
            member_expr->base.line = expr->base.line;
            member_expr->base.column = expr->base.column;
            member_expr->base.filename = expr->base.filename;
            member_expr->object = expr;
            member_expr->property_name = member->lexeme ? strdup(member->lexeme) : NULL;

            expr = (ASTExpression*)member_expr;
        } else {
            break;
        }
    }

    return expr;
}

ASTExpression* parser_parse_primary(Parser* parser) {
    if (parser_match(parser, TOKEN_NUMBER) || parser_match(parser, TOKEN_DECIMAL) ||
        parser_match(parser, TOKEN_TEXT) || parser_match(parser, TOKEN_BOOLEAN)) {
        Token* literal = parser_previous(parser);
        return (ASTExpression*)ast_literal_expression_create(literal->type, literal->lexeme);
    }

    if (parser_match(parser, TOKEN_IDENTIFIER)) {
        Token* identifier = parser_previous(parser);
        return (ASTExpression*)ast_identifier_expression_create(identifier->lexeme);
    }

    if (parser_match(parser, TOKEN_LEFT_PAREN)) {
        ASTExpression* expr = parser_parse_expression(parser);
        parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    if (parser_match(parser, TOKEN_LEFT_BRACKET)) {
        return parser_parse_list(parser);
    }

    if (parser_match(parser, TOKEN_LEFT_BRACE)) {
        return parser_parse_map(parser);
    }

    parser_error(parser, "Expected expression");
    return NULL;
}

ASTExpression* parser_parse_list(Parser* parser) {
    ASTListExpression* list_expr = (ASTListExpression*)malloc(sizeof(ASTListExpression));
    if (!list_expr) {
        return NULL;
    }

    Token* prev = parser_previous(parser);
    list_expr->base.type = NODE_LIST_EXPRESSION;
    list_expr->base.line = prev ? prev->line : 0;
    list_expr->base.column = prev ? prev->column : 0;
    list_expr->base.filename = prev ? prev->filename : NULL;
    list_expr->elements = NULL;
    list_expr->element_count = 0;

    if (!parser_check(parser, TOKEN_RIGHT_BRACKET)) {
        do {
            ASTExpression* elem = parser_parse_expression(parser);
            if (elem) {
                list_expr->element_count++;
            }
        } while (parser_match(parser, TOKEN_COMMA));
    }

    parser_consume(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after list elements");

    return (ASTExpression*)list_expr;
}

ASTExpression* parser_parse_map(Parser* parser) {
    ASTMapExpression* map_expr = (ASTMapExpression*)malloc(sizeof(ASTMapExpression));
    if (!map_expr) {
        return NULL;
    }

    Token* prev = parser_previous(parser);
    map_expr->base.type = NODE_MAP_EXPRESSION;
    map_expr->base.line = prev ? prev->line : 0;
    map_expr->base.column = prev ? prev->column : 0;
    map_expr->base.filename = prev ? prev->filename : NULL;
    map_expr->entries = NULL;
    map_expr->entry_count = 0;

    if (!parser_check(parser, TOKEN_RIGHT_BRACE)) {
        do {
            Token* key = parser_consume(parser, TOKEN_IDENTIFIER, "Expected map key");
            parser_consume(parser, TOKEN_COLON, "Expected ':' after map key");
            ASTExpression* value = parser_parse_expression(parser);
            if (key && value) {
                map_expr->entry_count++;
            }
        } while (parser_match(parser, TOKEN_COMMA));
    }

    parser_consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after map entries");

    return (ASTExpression*)map_expr;
}

ASTExpression* parser_parse_lambda(Parser* parser) {
    // Lambda syntax: param => body
    // This is a placeholder for future implementation
    parser_error(parser, "Lambda expressions not yet implemented");
    return NULL;
}

ASTExpression* parser_parse_if_expression(Parser* parser) {
    // If expression (ternary-like)
    // This is a placeholder for future implementation
    parser_error(parser, "If expressions not yet implemented");
    return NULL;
}

ASTExpression* parser_parse_choose(Parser* parser) {
    // Pattern matching expression
    // This is a placeholder for future implementation
    parser_error(parser, "Choose expressions not yet implemented");
    return NULL;
}
