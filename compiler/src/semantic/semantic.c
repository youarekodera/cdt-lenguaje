#include "semantic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SYMBOL_TABLE_INITIAL_CAPACITY 64

SemanticAnalyzer* semantic_analyzer_create(ASTProgram* program) {
    SemanticAnalyzer* analyzer = (SemanticAnalyzer*)malloc(sizeof(SemanticAnalyzer));
    if (!analyzer) {
        return NULL;
    }

    analyzer->program = program;
    analyzer->had_error = false;
    analyzer->error_message = NULL;
    analyzer->symbol_table = symbol_table_create(NULL);

    return analyzer;
}

void semantic_analyzer_free(SemanticAnalyzer* analyzer) {
    if (analyzer) {
        if (analyzer->symbol_table) {
            symbol_table_free(analyzer->symbol_table);
        }
        if (analyzer->error_message) {
            free(analyzer->error_message);
        }
        free(analyzer);
    }
}

SymbolTable* symbol_table_create(SymbolTable* parent) {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!table) {
        return NULL;
    }

    table->parent = parent;
    table->symbols = (Symbol**)malloc(sizeof(Symbol*) * SYMBOL_TABLE_INITIAL_CAPACITY);
    if (!table->symbols) {
        free(table);
        return NULL;
    }
    table->symbol_count = 0;
    table->symbol_capacity = SYMBOL_TABLE_INITIAL_CAPACITY;

    return table;
}

void symbol_table_free(SymbolTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->symbol_count; i++) {
        if (table->symbols[i]) {
            if (table->symbols[i]->name) {
                free(table->symbols[i]->name);
            }
            free(table->symbols[i]);
        }
    }
    free(table->symbols);

    if (table->parent) {
        symbol_table_free(table->parent);
    }

    free(table);
}

Symbol* symbol_table_add(SymbolTable* table, const char* name, TokenType type, bool is_mutable, bool is_constant, int line, int column) {
    if (!table || !name) {
        return NULL;
    }

    // Check if symbol already exists in current scope
    Symbol* existing = symbol_table_lookup_local(table, name);
    if (existing) {
        return NULL; // Symbol already defined
    }

    // Resize if needed
    if (table->symbol_count >= table->symbol_capacity) {
        size_t new_capacity = table->symbol_capacity * 2;
        Symbol** new_symbols = (Symbol**)realloc(table->symbols, sizeof(Symbol*) * new_capacity);
        if (!new_symbols) {
            return NULL;
        }
        table->symbols = new_symbols;
        table->symbol_capacity = new_capacity;
    }

    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!symbol) {
        return NULL;
    }

    symbol->name = strdup(name);
    symbol->type = type;
    symbol->is_mutable = is_mutable;
    symbol->is_constant = is_constant;
    symbol->is_function = false;
    symbol->is_defined = true;
    symbol->line = line;
    symbol->column = column;

    table->symbols[table->symbol_count++] = symbol;

    return symbol;
}

Symbol* symbol_table_lookup(SymbolTable* table, const char* name) {
    if (!table || !name) {
        return NULL;
    }

    // Check current scope
    Symbol* symbol = symbol_table_lookup_local(table, name);
    if (symbol) {
        return symbol;
    }

    // Check parent scopes
    if (table->parent) {
        return symbol_table_lookup(table->parent, name);
    }

    return NULL;
}

Symbol* symbol_table_lookup_local(SymbolTable* table, const char* name) {
    if (!table || !name) {
        return NULL;
    }

    for (size_t i = 0; i < table->symbol_count; i++) {
        if (table->symbols[i] && strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i];
        }
    }

    return NULL;
}

void symbol_table_push_scope(SemanticAnalyzer* analyzer) {
    if (!analyzer) return;
    analyzer->symbol_table = symbol_table_create(analyzer->symbol_table);
}

void symbol_table_pop_scope(SemanticAnalyzer* analyzer) {
    if (!analyzer || !analyzer->symbol_table) return;

    SymbolTable* current = analyzer->symbol_table;
    analyzer->symbol_table = current->parent;
    current->parent = NULL; // Prevent recursive free
    symbol_table_free(current);
}

TypeInfo* type_info_create(TokenType base_type) {
    TypeInfo* type = (TypeInfo*)malloc(sizeof(TypeInfo));
    if (!type) {
        return NULL;
    }

    type->base_type = base_type;
    type->is_optional = false;
    type->element_type = NULL;
    type->type_parameters = NULL;
    type->type_parameter_count = 0;

    return type;
}

void type_info_free(TypeInfo* type) {
    if (!type) return;

    if (type->element_type) {
        type_info_free(type->element_type);
    }

    if (type->type_parameters) {
        for (size_t i = 0; i < type->type_parameter_count; i++) {
            type_info_free(type->type_parameters[i]);
        }
        free(type->type_parameters);
    }

    free(type);
}

bool types_are_compatible(TypeInfo* type1, TypeInfo* type2) {
    if (!type1 || !type2) {
        return false;
    }

    // For now, simple type equality check
    // A full implementation would handle subtyping, optional types, etc.
    return type1->base_type == type2->base_type;
}

void semantic_error(SemanticAnalyzer* analyzer, int line, int column, const char* message) {
    if (!analyzer) return;

    analyzer->had_error = true;

    if (analyzer->error_message) {
        free(analyzer->error_message);
    }

    char buffer[512];
    snprintf(buffer, sizeof(buffer), "Line %d, Column %d: %s", line, column, message);
    analyzer->error_message = strdup(buffer);

    fprintf(stderr, "Semantic Error: %s\n", buffer);
}

void semantic_error_at_node(SemanticAnalyzer* analyzer, ASTNode* node, const char* message) {
    if (!analyzer || !node) return;

    semantic_error(analyzer, node->line, node->column, message);
}

bool validate_variable_declaration(ASTVariableDeclaration* decl, SymbolTable* table) {
    if (!decl || !table) {
        return false;
    }

    // Check if variable already exists
    Symbol* existing = symbol_table_lookup_local(table, decl->name);
    if (existing) {
        fprintf(stderr, "Error: Variable '%s' already declared at line %d\n",
                decl->name, existing->line);
        return false;
    }

    // Add to symbol table
    Symbol* symbol = symbol_table_add(table, decl->name, decl->type_annotation,
                                      decl->is_mutable, decl->is_constant,
                                      decl->base.line, decl->base.column);
    if (!symbol) {
        fprintf(stderr, "Error: Failed to add symbol '%s' to symbol table\n", decl->name);
        return false;
    }

    // Type check initializer if present
    if (decl->initializer) {
        // For now, we'll skip full type checking of the initializer
        // In a full implementation, this would call type_check_expression
    }

    return true;
}

bool validate_function_declaration(ASTFunctionDeclaration* decl, SymbolTable* table) {
    if (!decl || !table) {
        return false;
    }

    // Check if function already exists
    Symbol* existing = symbol_table_lookup_local(table, decl->name);
    if (existing) {
        fprintf(stderr, "Error: Function '%s' already declared at line %d\n",
                decl->name, existing->line);
        return false;
    }

    // Add function to symbol table
    Symbol* symbol = symbol_table_add(table, decl->name, decl->return_type,
                                      false, false, decl->base.line, decl->base.column);
    if (!symbol) {
        fprintf(stderr, "Error: Failed to add function '%s' to symbol table\n", decl->name);
        return false;
    }
    symbol->is_function = true;

    // Create new scope for function body
    SymbolTable* function_scope = symbol_table_create(table);

    // Add parameters to function scope
    for (size_t i = 0; i < decl->parameter_count; i++) {
        if (decl->parameters[i]) {
            Symbol* param_symbol = symbol_table_add(function_scope,
                                                   decl->parameters[i]->name,
                                                   decl->parameters[i]->type,
                                                   false, false,
                                                   decl->base.line, decl->base.column);
            if (!param_symbol) {
                fprintf(stderr, "Error: Failed to add parameter '%s'\n",
                        decl->parameters[i]->name);
                symbol_table_free(function_scope);
                return false;
            }
        }
    }

    // Validate function body
    if (decl->body) {
        for (size_t i = 0; i < decl->body->statement_count; i++) {
            if (decl->body->statements[i]) {
                // Validate each statement
                // For now, we'll skip full validation
            }
        }
    }

    symbol_table_free(function_scope);

    return true;
}

bool validate_object_declaration(ASTObjectDeclaration* decl, SymbolTable* table) {
    if (!decl || !table) {
        return false;
    }

    // Check if object already exists
    Symbol* existing = symbol_table_lookup_local(table, decl->name);
    if (existing) {
        fprintf(stderr, "Error: Object '%s' already declared at line %d\n",
                decl->name, existing->line);
        return false;
    }

    // Add object to symbol table
    Symbol* symbol = symbol_table_add(table, decl->name, TOKEN_IDENTIFIER,
                                      false, false, decl->base.line, decl->base.column);
    if (!symbol) {
        fprintf(stderr, "Error: Failed to add object '%s' to symbol table\n", decl->name);
        return false;
    }

    // Validate properties
    for (size_t i = 0; i < decl->property_count; i++) {
        if (decl->properties[i]) {
            // Validate property
        }
    }

    // Validate actions
    for (size_t i = 0; i < decl->action_count; i++) {
        if (decl->actions[i]) {
            // Validate action
        }
    }

    return true;
}

bool validate_function_call(ASTCallExpression* call, SymbolTable* table) {
    if (!call || !table) {
        return false;
    }

    // Check if function exists
    if (call->callee->base.type == NODE_IDENTIFIER_EXPRESSION) {
        ASTIdentifierExpression* ident = (ASTIdentifierExpression*)call->callee;
        Symbol* symbol = symbol_table_lookup(table, ident->name);
        if (!symbol) {
            fprintf(stderr, "Error: Unknown function '%s'\n", ident->name);
            return false;
        }

        if (!symbol->is_function) {
            fprintf(stderr, "Error: '%s' is not a function\n", ident->name);
            return false;
        }
    }

    return true;
}

bool validate_assignment(ASTExpression* target, ASTExpression* value, SymbolTable* table) {
    if (!target || !value || !table) {
        return false;
    }

    // Check if target is an identifier
    if (target->base.type == NODE_IDENTIFIER_EXPRESSION) {
        ASTIdentifierExpression* ident = (ASTIdentifierExpression*)target;
        Symbol* symbol = symbol_table_lookup(table, ident->name);
        if (!symbol) {
            fprintf(stderr, "Error: Unknown variable '%s'\n", ident->name);
            return false;
        }

        if (symbol->is_constant) {
            fprintf(stderr, "Error: Cannot assign to constant '%s'\n", ident->name);
            return false;
        }

        if (!symbol->is_mutable) {
            fprintf(stderr, "Error: Cannot assign to immutable variable '%s'\n", ident->name);
            return false;
        }
    }

    return true;
}

bool type_check_expression(ASTExpression* expr, SymbolTable* table) {
    if (!expr) {
        return false;
    }

    switch (expr->base.type) {
        case NODE_LITERAL_EXPRESSION:
            return true;

        case NODE_IDENTIFIER_EXPRESSION: {
            ASTIdentifierExpression* ident = (ASTIdentifierExpression*)expr;
            Symbol* symbol = symbol_table_lookup(table, ident->name);
            if (!symbol) {
                fprintf(stderr, "Error: Unknown variable '%s'\n", ident->name);
                return false;
            }
            return true;
        }

        case NODE_BINARY_EXPRESSION: {
            ASTBinaryExpression* bin_expr = (ASTBinaryExpression*)expr;
            return type_check_expression(bin_expr->left, table) &&
                   type_check_expression(bin_expr->right, table);
        }

        case NODE_UNARY_EXPRESSION: {
            ASTUnaryExpression* unary_expr = (ASTUnaryExpression*)expr;
            return type_check_expression(unary_expr->operand, table);
        }

        case NODE_CALL_EXPRESSION: {
            return validate_function_call((ASTCallExpression*)expr, table);
        }

        case NODE_LIST_EXPRESSION:
        case NODE_MAP_EXPRESSION:
            return true;

        default:
            return true; // Placeholder for other expression types
    }
}

bool type_check_statement(ASTStatement* stmt, SymbolTable* table) {
    if (!stmt) {
        return false;
    }

    switch (stmt->base.type) {
        case NODE_EXPRESSION_STATEMENT: {
            ASTExpressionStatement* expr_stmt = (ASTExpressionStatement*)stmt;
            return type_check_expression(expr_stmt->expression, table);
        }

        case NODE_VARIABLE_DECLARATION: {
            return validate_variable_declaration((ASTVariableDeclaration*)stmt, table);
        }

        case NODE_ASSIGNMENT_STATEMENT: {
            ASTAssignmentStatement* assign_stmt = (ASTAssignmentStatement*)stmt;
            return validate_assignment(assign_stmt->target, assign_stmt->value, table);
        }

        case NODE_CONDITIONAL_STATEMENT: {
            ASTConditionalStatement* cond_stmt = (ASTConditionalStatement*)stmt;
            return type_check_expression(cond_stmt->condition, table);
        }

        case NODE_REPEAT_STATEMENT:
        case NODE_FOR_STATEMENT:
        case NODE_WHILE_STATEMENT:
            return true; // Placeholder

        default:
            return true; // Placeholder for other statement types
    }
}

bool type_check_declaration(ASTDeclaration* decl, SymbolTable* table) {
    if (!decl) {
        return false;
    }

    switch (decl->base.type) {
        case NODE_VARIABLE_DECLARATION:
            return validate_variable_declaration((ASTVariableDeclaration*)decl, table);

        case NODE_FUNCTION_DECLARATION:
            return validate_function_declaration((ASTFunctionDeclaration*)decl, table);

        case NODE_OBJECT_DECLARATION:
            return validate_object_declaration((ASTObjectDeclaration*)decl, table);

        default:
            return true; // Placeholder for other declaration types
    }
}

bool semantic_analyze(SemanticAnalyzer* analyzer) {
    if (!analyzer || !analyzer->program) {
        return false;
    }

    // Analyze declarations
    for (size_t i = 0; i < analyzer->program->declaration_count; i++) {
        if (analyzer->program->declarations[i]) {
            if (!type_check_declaration(analyzer->program->declarations[i],
                                       analyzer->symbol_table)) {
                analyzer->had_error = true;
            }
        }
    }

    return !analyzer->had_error;
}
