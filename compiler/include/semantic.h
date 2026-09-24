#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <stdbool.h>

// Forward declarations
typedef struct SymbolTable SymbolTable;
typedef struct Symbol Symbol;
typedef struct TypeInfo TypeInfo;

// Semantic analyzer structure
typedef struct {
    ASTProgram* program;
    bool had_error;
    char* error_message;
    SymbolTable* symbol_table;
} SemanticAnalyzer;

// Symbol table for tracking variables, functions, etc.
struct SymbolTable {
    struct SymbolTable* parent;
    Symbol** symbols;
    size_t symbol_count;
    size_t symbol_capacity;
};

// Symbol representation
struct Symbol {
    char* name;
    TokenType type;
    bool is_mutable;
    bool is_constant;
    bool is_function;
    bool is_defined;
    int line;
    int column;
};

// Type information
struct TypeInfo {
    TokenType base_type;
    bool is_optional;
    TypeInfo* element_type; // For collections
    TypeInfo** type_parameters; // For generics
    size_t type_parameter_count;
};

// Semantic analyzer functions
SemanticAnalyzer* semantic_analyzer_create(ASTProgram* program);
void semantic_analyzer_free(SemanticAnalyzer* analyzer);
bool semantic_analyze(SemanticAnalyzer* analyzer);

// Symbol table functions
SymbolTable* symbol_table_create(SymbolTable* parent);
void symbol_table_free(SymbolTable* table);
Symbol* symbol_table_add(SymbolTable* table, const char* name, TokenType type, bool is_mutable, bool is_constant, int line, int column);
Symbol* symbol_table_lookup(SymbolTable* table, const char* name);
Symbol* symbol_table_lookup_local(SymbolTable* table, const char* name);
void symbol_table_push_scope(SemanticAnalyzer* analyzer);
void symbol_table_pop_scope(SemanticAnalyzer* analyzer);

// Type checking functions
bool type_check_expression(ASTExpression* expr, SymbolTable* table);
bool type_check_statement(ASTStatement* stmt, SymbolTable* table);
bool type_check_declaration(ASTDeclaration* decl, SymbolTable* table);
TypeInfo* type_infer_expression(ASTExpression* expr, SymbolTable* table);
bool types_are_compatible(TypeInfo* type1, TypeInfo* type2);
TypeInfo* type_info_create(TokenType base_type);
void type_info_free(TypeInfo* type);

// Error reporting
void semantic_error(SemanticAnalyzer* analyzer, int line, int column, const char* message);
void semantic_error_at_node(SemanticAnalyzer* analyzer, ASTNode* node, const char* message);

// Validation functions
bool validate_variable_declaration(ASTVariableDeclaration* decl, SymbolTable* table);
bool validate_function_declaration(ASTFunctionDeclaration* decl, SymbolTable* table);
bool validate_object_declaration(ASTObjectDeclaration* decl, SymbolTable* table);
bool validate_function_call(ASTCallExpression* call, SymbolTable* table);
bool validate_assignment(ASTExpression* target, ASTExpression* value, SymbolTable* table);

#endif // SEMANTIC_H
