#ifndef AST_H
#define AST_H

#include "token.h"
#include <stdbool.h>

// Forward declarations
typedef struct ASTNode ASTNode;
typedef struct ASTProgram ASTProgram;
typedef struct ASTBlock ASTBlock;
typedef struct ASTVariableDeclaration ASTVariableDeclaration;
typedef struct ASTFunctionDeclaration ASTFunctionDeclaration;
typedef struct ASTObjectDeclaration ASTObjectDeclaration;
typedef struct ASTPropertyDeclaration ASTPropertyDeclaration;
typedef struct ASTActionDeclaration ASTActionDeclaration;
typedef struct ASTContractDeclaration ASTContractDeclaration;
typedef struct ASTChoiceDeclaration ASTChoiceDeclaration;
typedef struct ASTImportStatement ASTImportStatement;
typedef struct ASTModuleStatement ASTModuleStatement;

// Base declaration type (for all declarations)
typedef struct ASTDeclaration ASTDeclaration;

// Helper structure forward declarations
typedef struct {
    char* name;
    TokenType type;
} ASTParameter;

typedef struct {
    char* key;
    struct ASTExpression* value;
} ASTMapEntry;

typedef struct {
    struct ASTExpression* pattern;
    struct ASTBlock* body;
} ASTMatchCase;

typedef struct {
    struct ASTExpression* pattern;
    struct ASTExpression* body;
} ASTChooseCase;

// Expression nodes (forward declarations)
typedef struct ASTExpression ASTExpression;
typedef struct ASTBinaryExpression ASTBinaryExpression;
typedef struct ASTUnaryExpression ASTUnaryExpression;
typedef struct ASTCallExpression ASTCallExpression;
typedef struct ASTMemberExpression ASTMemberExpression;
typedef struct ASTIndexExpression ASTIndexExpression;
typedef struct ASTLiteralExpression ASTLiteralExpression;
typedef struct ASTIdentifierExpression ASTIdentifierExpression;
typedef struct ASTListExpression ASTListExpression;
typedef struct ASTMapExpression ASTMapExpression;
typedef struct ASTLambdaExpression ASTLambdaExpression;
typedef struct ASTIfExpression ASTIfExpression;
typedef struct ASTChooseExpression ASTChooseExpression;

// Statement nodes (forward declarations)
typedef struct ASTStatement ASTStatement;
typedef struct ASTExpressionStatement ASTExpressionStatement;
typedef struct ASTAssignmentStatement ASTAssignmentStatement;
typedef struct ASTChangeStatement ASTChangeStatement;
typedef struct ASTConditionalStatement ASTConditionalStatement;
typedef struct ASTLoopStatement ASTLoopStatement;
typedef struct ASTRepeatStatement ASTRepeatStatement;
typedef struct ASTForStatement ASTForStatement;
typedef struct ASTWhileStatement ASTWhileStatement;
typedef struct ASTReturnStatement ASTReturnStatement;
typedef struct ATSTryStatement ATSTryStatement;
typedef struct ASTRaiseStatement ASTRaiseStatement;
typedef struct ASTMatchStatement ASTMatchStatement;

// Node types
typedef enum {
    // Program
    NODE_PROGRAM,

    // Declarations
    NODE_MODULE,
    NODE_IMPORT,
    NODE_VARIABLE_DECLARATION,
    NODE_FUNCTION_DECLARATION,
    NODE_OBJECT_DECLARATION,
    NODE_PROPERTY_DECLARATION,
    NODE_ACTION_DECLARATION,
    NODE_CONTRACT_DECLARATION,
    NODE_CHOICE_DECLARATION,

    // Statements
    NODE_BLOCK,
    NODE_EXPRESSION_STATEMENT,
    NODE_ASSIGNMENT_STATEMENT,
    NODE_CHANGE_STATEMENT,
    NODE_CONDITIONAL_STATEMENT,
    NODE_LOOP_STATEMENT,
    NODE_REPEAT_STATEMENT,
    NODE_FOR_STATEMENT,
    NODE_WHILE_STATEMENT,
    NODE_RETURN_STATEMENT,
    NODE_TRY_STATEMENT,
    NODE_RAISE_STATEMENT,
    NODE_MATCH_STATEMENT,

    // Expressions
    NODE_BINARY_EXPRESSION,
    NODE_UNARY_EXPRESSION,
    NODE_CALL_EXPRESSION,
    NODE_MEMBER_EXPRESSION,
    NODE_INDEX_EXPRESSION,
    NODE_LITERAL_EXPRESSION,
    NODE_IDENTIFIER_EXPRESSION,
    NODE_LIST_EXPRESSION,
    NODE_MAP_EXPRESSION,
    NODE_LAMBDA_EXPRESSION,
    NODE_IF_EXPRESSION,
    NODE_CHOOSE_EXPRESSION
} NodeType;

// Base node structure
struct ASTNode {
    NodeType type;
    int line;
    int column;
    const char* filename;
};

// Base declaration structure
struct ASTDeclaration {
    ASTNode base;
};

// Base statement structure
struct ASTStatement {
    ASTNode base;
};

// Base expression structure
struct ASTExpression {
    ASTNode base;
};

// Program node
struct ASTProgram {
    ASTNode base;
    ASTModuleStatement* module;
    ASTImportStatement** imports;
    size_t import_count;
    ASTDeclaration** declarations;
    size_t declaration_count;
};

// Block node
struct ASTBlock {
    ASTNode base;
    ASTStatement** statements;
    size_t statement_count;
};

// Module statement
struct ASTModuleStatement {
    ASTNode base;
    char* name;
};

// Import statement
struct ASTImportStatement {
    ASTNode base;
    char* module_name;
    char* alias;
    char** imports;
    size_t import_count;
};

// Variable declaration
struct ASTVariableDeclaration {
    ASTNode base;
    char* name;
    TokenType type_annotation;
    ASTExpression* initializer;
    bool is_mutable;
    bool is_constant;
};

// Function declaration
struct ASTFunctionDeclaration {
    ASTNode base;
    char* name;
    ASTParameter** parameters;
    size_t parameter_count;
    TokenType return_type;
    ASTBlock* body;
};

// Object declaration
struct ASTObjectDeclaration {
    ASTNode base;
    char* name;
    char** contracts;
    size_t contract_count;
    ASTPropertyDeclaration** properties;
    size_t property_count;
    ASTActionDeclaration** actions;
    size_t action_count;
};

// Property declaration
struct ASTPropertyDeclaration {
    ASTNode base;
    char* name;
    TokenType type_annotation;
    ASTExpression* default_value;
};

// Action declaration (method)
struct ASTActionDeclaration {
    ASTNode base;
    char* name;
    ASTParameter** parameters;
    size_t parameter_count;
    TokenType return_type;
    ASTBlock* body;
};

// Contract declaration
struct ASTContractDeclaration {
    ASTNode base;
    char* name;
    ASTActionDeclaration** actions;
    size_t action_count;
};

// Choice declaration (enum)
struct ASTChoiceDeclaration {
    ASTNode base;
    char* name;
    char** variants;
    size_t variant_count;
};

// Expression statement
struct ASTExpressionStatement {
    ASTNode base;
    ASTExpression* expression;
};

// Assignment statement
struct ASTAssignmentStatement {
    ASTNode base;
    ASTExpression* target;
    ASTExpression* value;
};

// Change statement
struct ASTChangeStatement {
    ASTNode base;
    char* variable_name;
    ASTExpression* value;
};

// Conditional statement
struct ASTConditionalStatement {
    ASTNode base;
    ASTExpression* condition;
    ASTBlock* then_block;
    ASTConditionalStatement* elif_branch;
    ASTBlock* else_block;
};

// Loop statement (base)
struct ASTLoopStatement {
    ASTNode base;
};

// Repeat statement
struct ASTRepeatStatement {
    ASTNode base;
    char* variable_name;
    ASTExpression* start;
    ASTExpression* end;
    ASTBlock* body;
};

// For statement
struct ASTForStatement {
    ASTNode base;
    char* variable_name;
    ASTExpression* collection;
    ASTBlock* body;
};

// While statement
struct ASTWhileStatement {
    ASTNode base;
    ASTExpression* condition;
    ASTBlock* body;
};

// Return statement
struct ASTReturnStatement {
    ASTNode base;
    ASTExpression* value;
};

// Try statement
struct ATSTryStatement {
    ASTNode base;
    ASTBlock* try_block;
    char* error_variable;
    ASTBlock* catch_block;
};

// Raise statement
struct ASTRaiseStatement {
    ASTNode base;
    ASTExpression* error;
};

// Match statement
struct ASTMatchStatement {
    ASTNode base;
    ASTExpression* value;
    ASTMatchCase** cases;
    size_t case_count;
    ASTBlock* default_case;
};

// Binary expression
struct ASTBinaryExpression {
    ASTNode base;
    ASTExpression* left;
    TokenType operator;
    ASTExpression* right;
};

// Unary expression
struct ASTUnaryExpression {
    ASTNode base;
    TokenType operator;
    ASTExpression* operand;
};

// Call expression
struct ASTCallExpression {
    ASTNode base;
    ASTExpression* callee;
    ASTExpression** arguments;
    size_t argument_count;
};

// Member expression
struct ASTMemberExpression {
    ASTNode base;
    ASTExpression* object;
    char* property_name;
};

// Index expression
struct ASTIndexExpression {
    ASTNode base;
    ASTExpression* object;
    ASTExpression* index;
};

// Literal expression
struct ASTLiteralExpression {
    ASTNode base;
    TokenType literal_type;
    union {
        long number_value;
        double decimal_value;
        char* text_value;
        char character_value;
        bool boolean_value;
    } value;
};

// Identifier expression
struct ASTIdentifierExpression {
    ASTNode base;
    char* name;
};

// List expression
struct ASTListExpression {
    ASTNode base;
    ASTExpression** elements;
    size_t element_count;
};

// Map expression
struct ASTMapExpression {
    ASTNode base;
    ASTMapEntry** entries;
    size_t entry_count;
};

// Lambda expression
struct ASTLambdaExpression {
    ASTNode base;
    ASTParameter** parameters;
    size_t parameter_count;
    ASTExpression* body;
};

// If expression
struct ASTIfExpression {
    ASTNode base;
    ASTExpression* condition;
    ASTExpression* then_expr;
    ASTExpression* else_expr;
};

// Choose expression (pattern matching)
struct ASTChooseExpression {
    ASTNode base;
    ASTExpression* value;
    ASTChooseCase** cases;
    size_t case_count;
    ASTExpression* default_case;
};

// AST management functions
ASTProgram* ast_program_create();
void ast_program_free(ASTProgram* program);

ASTBlock* ast_block_create();
void ast_block_free(ASTBlock* block);

ASTVariableDeclaration* ast_variable_declaration_create(const char* name, TokenType type, ASTExpression* initializer, bool is_mutable, bool is_constant);
void ast_variable_declaration_free(ASTVariableDeclaration* decl);

ASTFunctionDeclaration* ast_function_declaration_create(const char* name, ASTParameter** parameters, size_t param_count, TokenType return_type, ASTBlock* body);
void ast_function_declaration_free(ASTFunctionDeclaration* decl);

ASTExpression* ast_binary_expression_create(ASTExpression* left, TokenType operator, ASTExpression* right);
void ast_expression_free(ASTExpression* expr);

ASTLiteralExpression* ast_literal_expression_create(TokenType type, const char* value);
void ast_literal_expression_free(ASTLiteralExpression* expr);

ASTIdentifierExpression* ast_identifier_expression_create(const char* name);
void ast_identifier_expression_free(ASTIdentifierExpression* expr);

const char* ast_node_type_name(NodeType type);

#endif // AST_H
