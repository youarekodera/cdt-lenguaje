#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTProgram* ast_program_create() {
    ASTProgram* program = (ASTProgram*)malloc(sizeof(ASTProgram));
    if (!program) {
        return NULL;
    }

    program->base.type = NODE_PROGRAM;
    program->base.line = 0;
    program->base.column = 0;
    program->base.filename = NULL;
    program->module = NULL;
    program->imports = NULL;
    program->import_count = 0;
    program->declarations = NULL;
    program->declaration_count = 0;

    return program;
}

void ast_program_free(ASTProgram* program) {
    if (!program) return;

    if (program->module) {
        free(program->module->name);
        free(program->module);
    }

    for (size_t i = 0; i < program->import_count; i++) {
        if (program->imports[i]) {
            free(program->imports[i]->module_name);
            if (program->imports[i]->alias) {
                free(program->imports[i]->alias);
            }
            for (size_t j = 0; j < program->imports[i]->import_count; j++) {
                free(program->imports[i]->imports[j]);
            }
            free(program->imports[i]->imports);
            free(program->imports[i]);
        }
    }
    free(program->imports);

    // Free declarations (will need more comprehensive handling for different types)
    for (size_t i = 0; i < program->declaration_count; i++) {
        // TODO: Free different declaration types based on their actual type
        if (program->declarations[i]) {
            free(program->declarations[i]);
        }
    }
    free(program->declarations);

    free(program);
}

ASTBlock* ast_block_create() {
    ASTBlock* block = (ASTBlock*)malloc(sizeof(ASTBlock));
    if (!block) {
        return NULL;
    }

    block->base.type = NODE_BLOCK;
    block->base.line = 0;
    block->base.column = 0;
    block->base.filename = NULL;
    block->statements = NULL;
    block->statement_count = 0;

    return block;
}

void ast_block_free(ASTBlock* block) {
    if (!block) return;

    for (size_t i = 0; i < block->statement_count; i++) {
        if (block->statements[i]) {
            // TODO: Free different statement types
            free(block->statements[i]);
        }
    }
    free(block->statements);
    free(block);
}

ASTVariableDeclaration* ast_variable_declaration_create(const char* name, TokenType type, ASTExpression* initializer, bool is_mutable, bool is_constant) {
    ASTVariableDeclaration* decl = (ASTVariableDeclaration*)malloc(sizeof(ASTVariableDeclaration));
    if (!decl) {
        return NULL;
    }

    decl->base.type = NODE_VARIABLE_DECLARATION;
    decl->base.line = 0;
    decl->base.column = 0;
    decl->base.filename = NULL;

    decl->name = name ? strdup(name) : NULL;
    decl->type_annotation = type;
    decl->initializer = initializer;
    decl->is_mutable = is_mutable;
    decl->is_constant = is_constant;

    return decl;
}

void ast_variable_declaration_free(ASTVariableDeclaration* decl) {
    if (!decl) return;

    if (decl->name) {
        free(decl->name);
    }
    if (decl->initializer) {
        ast_expression_free(decl->initializer);
    }
    free(decl);
}

ASTFunctionDeclaration* ast_function_declaration_create(const char* name, ASTParameter** parameters, size_t param_count, TokenType return_type, ASTBlock* body) {
    ASTFunctionDeclaration* decl = (ASTFunctionDeclaration*)malloc(sizeof(ASTFunctionDeclaration));
    if (!decl) {
        return NULL;
    }

    decl->base.type = NODE_FUNCTION_DECLARATION;
    decl->base.line = 0;
    decl->base.column = 0;
    decl->base.filename = NULL;

    decl->name = name ? strdup(name) : NULL;
    decl->parameters = parameters;
    decl->parameter_count = param_count;
    decl->return_type = return_type;
    decl->body = body;

    return decl;
}

void ast_function_declaration_free(ASTFunctionDeclaration* decl) {
    if (!decl) return;

    if (decl->name) {
        free(decl->name);
    }

    for (size_t i = 0; i < decl->parameter_count; i++) {
        if (decl->parameters[i]) {
            if (decl->parameters[i]->name) {
                free(decl->parameters[i]->name);
            }
            free(decl->parameters[i]);
        }
    }
    free(decl->parameters);

    if (decl->body) {
        ast_block_free(decl->body);
    }

    free(decl);
}

ASTExpression* ast_binary_expression_create(ASTExpression* left, TokenType operator, ASTExpression* right) {
    ASTBinaryExpression* expr = (ASTBinaryExpression*)malloc(sizeof(ASTBinaryExpression));
    if (!expr) {
        return NULL;
    }

    expr->base.type = NODE_BINARY_EXPRESSION;
    expr->base.line = 0;
    expr->base.column = 0;
    expr->base.filename = NULL;

    expr->left = left;
    expr->operator = operator;
    expr->right = right;

    return (ASTExpression*)expr;
}

void ast_expression_free(ASTExpression* expr) {
    if (!expr) return;

    switch (expr->base.type) {
        case NODE_BINARY_EXPRESSION: {
            ASTBinaryExpression* bin_expr = (ASTBinaryExpression*)expr;
            ast_expression_free(bin_expr->left);
            ast_expression_free(bin_expr->right);
            free(bin_expr);
            break;
        }
        case NODE_UNARY_EXPRESSION: {
            ASTUnaryExpression* unary_expr = (ASTUnaryExpression*)expr;
            ast_expression_free(unary_expr->operand);
            free(unary_expr);
            break;
        }
        case NODE_LITERAL_EXPRESSION: {
            ast_literal_expression_free((ASTLiteralExpression*)expr);
            break;
        }
        case NODE_IDENTIFIER_EXPRESSION: {
            ast_identifier_expression_free((ASTIdentifierExpression*)expr);
            break;
        }
        default:
            free(expr);
            break;
    }
}

ASTLiteralExpression* ast_literal_expression_create(TokenType type, const char* value) {
    ASTLiteralExpression* expr = (ASTLiteralExpression*)malloc(sizeof(ASTLiteralExpression));
    if (!expr) {
        return NULL;
    }

    expr->base.type = NODE_LITERAL_EXPRESSION;
    expr->base.line = 0;
    expr->base.column = 0;
    expr->base.filename = NULL;

    expr->literal_type = type;

    switch (type) {
        case TOKEN_NUMBER:
            expr->value.number_value = atol(value);
            break;
        case TOKEN_DECIMAL:
            expr->value.decimal_value = atof(value);
            break;
        case TOKEN_TEXT:
            expr->value.text_value = value ? strdup(value) : NULL;
            break;
        case TOKEN_CHARACTER:
            expr->value.character_value = value ? value[0] : '\0';
            break;
        case TOKEN_BOOLEAN:
            expr->value.boolean_value = (strcmp(value, "true") == 0);
            break;
        default:
            expr->value.text_value = value ? strdup(value) : NULL;
            break;
    }

    return expr;
}

void ast_literal_expression_free(ASTLiteralExpression* expr) {
    if (!expr) return;

    if (expr->literal_type == TOKEN_TEXT && expr->value.text_value) {
        free(expr->value.text_value);
    }
    free(expr);
}

ASTIdentifierExpression* ast_identifier_expression_create(const char* name) {
    ASTIdentifierExpression* expr = (ASTIdentifierExpression*)malloc(sizeof(ASTIdentifierExpression));
    if (!expr) {
        return NULL;
    }

    expr->base.type = NODE_IDENTIFIER_EXPRESSION;
    expr->base.line = 0;
    expr->base.column = 0;
    expr->base.filename = NULL;

    expr->name = name ? strdup(name) : NULL;

    return expr;
}

void ast_identifier_expression_free(ASTIdentifierExpression* expr) {
    if (!expr) return;

    if (expr->name) {
        free(expr->name);
    }
    free(expr);
}

const char* ast_node_type_name(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "Program";
        case NODE_MODULE: return "Module";
        case NODE_IMPORT: return "Import";
        case NODE_VARIABLE_DECLARATION: return "VariableDeclaration";
        case NODE_FUNCTION_DECLARATION: return "FunctionDeclaration";
        case NODE_OBJECT_DECLARATION: return "ObjectDeclaration";
        case NODE_PROPERTY_DECLARATION: return "PropertyDeclaration";
        case NODE_ACTION_DECLARATION: return "ActionDeclaration";
        case NODE_CONTRACT_DECLARATION: return "ContractDeclaration";
        case NODE_CHOICE_DECLARATION: return "ChoiceDeclaration";
        case NODE_BLOCK: return "Block";
        case NODE_EXPRESSION_STATEMENT: return "ExpressionStatement";
        case NODE_ASSIGNMENT_STATEMENT: return "AssignmentStatement";
        case NODE_CHANGE_STATEMENT: return "ChangeStatement";
        case NODE_CONDITIONAL_STATEMENT: return "ConditionalStatement";
        case NODE_LOOP_STATEMENT: return "LoopStatement";
        case NODE_REPEAT_STATEMENT: return "RepeatStatement";
        case NODE_FOR_STATEMENT: return "ForStatement";
        case NODE_WHILE_STATEMENT: return "WhileStatement";
        case NODE_RETURN_STATEMENT: return "ReturnStatement";
        case NODE_TRY_STATEMENT: return "TryStatement";
        case NODE_RAISE_STATEMENT: return "RaiseStatement";
        case NODE_MATCH_STATEMENT: return "MatchStatement";
        case NODE_BINARY_EXPRESSION: return "BinaryExpression";
        case NODE_UNARY_EXPRESSION: return "UnaryExpression";
        case NODE_CALL_EXPRESSION: return "CallExpression";
        case NODE_MEMBER_EXPRESSION: return "MemberExpression";
        case NODE_INDEX_EXPRESSION: return "IndexExpression";
        case NODE_LITERAL_EXPRESSION: return "LiteralExpression";
        case NODE_IDENTIFIER_EXPRESSION: return "IdentifierExpression";
        case NODE_LIST_EXPRESSION: return "ListExpression";
        case NODE_MAP_EXPRESSION: return "MapExpression";
        case NODE_LAMBDA_EXPRESSION: return "LambdaExpression";
        case NODE_IF_EXPRESSION: return "IfExpression";
        case NODE_CHOOSE_EXPRESSION: return "ChooseExpression";
        default: return "Unknown";
    }
}
