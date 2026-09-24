#ifndef IR_H
#define IR_H

#include "ast.h"
#include <stdbool.h>
#include <stdio.h>

// IR instruction types
typedef enum {
    IR_NOP,
    IR_CONST,
    IR_LOAD,
    IR_STORE,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_MOD,
    IR_NEG,
    IR_CALL,
    IR_RETURN,
    IR_JUMP,
    IR_JUMP_IF,
    IR_LABEL,
    IR_PARAM,
    IR_PHI
} IRInstructionType;

// IR value types
typedef enum {
    IR_VALUE_INT,
    IR_VALUE_FLOAT,
    IR_VALUE_STRING,
    IR_VALUE_BOOLEAN,
    IR_VALUE_REGISTER,
    IR_VALUE_LABEL,
    IR_VALUE_TEMP
} IRValueType;

// IR value representation
typedef struct IRValue {
    IRValueType type;
    union {
        long int_value;
        double float_value;
        char* string_value;
        bool bool_value;
        int register_number;
        char* label_name;
    } data;
} IRValue;

// IR instruction
typedef struct IRInstruction {
    IRInstructionType type;
    IRValue* result;
    IRValue* operand1;
    IRValue* operand2;
    int line;
    int column;
    struct IRInstruction* next;
} IRInstruction;

// IR basic block
typedef struct IRBasicBlock {
    char* name;
    IRInstruction* instructions;
    struct IRBasicBlock* next;
    struct IRBasicBlock** predecessors;
    size_t predecessor_count;
    struct IRBasicBlock** successors;
    size_t successor_count;
} IRBasicBlock;

// IR function
typedef struct IRFunction {
    char* name;
    IRBasicBlock* entry_block;
    IRBasicBlock* blocks;
    IRValue** parameters;
    size_t parameter_count;
    IRValue* return_value;
    struct IRFunction* next;
} IRFunction;

// IR module (represents entire program)
typedef struct IRModule {
    char* name;
    IRFunction* functions;
    IRValue** globals;
    size_t global_count;
} IRModule;

// IR generation context
typedef struct {
    IRModule* module;
    IRFunction* current_function;
    IRBasicBlock* current_block;
    int temp_counter;
    int label_counter;
    int register_counter;
} IRContext;

// IR management functions
IRModule* ir_module_create(const char* name);
void ir_module_free(IRModule* module);

IRFunction* ir_function_create(const char* name);
void ir_function_free(IRFunction* function);

IRBasicBlock* ir_basic_block_create(const char* name);
void ir_basic_block_free(IRBasicBlock* block);

IRInstruction* ir_instruction_create(IRInstructionType type);
void ir_instruction_free(IRInstruction* instruction);

IRValue* ir_value_create(IRValueType type);
void ir_value_free(IRValue* value);

IRContext* ir_context_create(IRModule* module);
void ir_context_free(IRContext* context);

// IR generation from AST
IRModule* ir_generate_from_ast(ASTProgram* program);
IRFunction* ir_generate_function(ASTFunctionDeclaration* decl, IRContext* context);
IRBasicBlock* ir_generate_block(ASTBlock* block, IRContext* context);
IRInstruction* ir_generate_expression(ASTExpression* expr, IRContext* context);
IRInstruction* ir_generate_statement(ASTStatement* stmt, IRContext* context);

// Helper functions
char* ir_generate_temp_name(IRContext* context);
char* ir_generate_label_name(IRContext* context);
IRValue* ir_create_int_constant(long value);
IRValue* ir_create_float_constant(double value);
IRValue* ir_create_string_constant(const char* value);
IRValue* ir_create_boolean_constant(bool value);
IRValue* ir_create_temp_register(IRContext* context);

IRInstruction* ir_emit_instruction(IRContext* context, IRInstructionType type,
                                   IRValue* result, IRValue* operand1, IRValue* operand2);
IRInstruction* ir_emit_binary_op(IRContext* context, IRInstructionType type,
                                  IRValue* left, IRValue* right);
IRInstruction* ir_emit_unary_op(IRContext* context, IRInstructionType type, IRValue* operand);

// IR printing (for debugging)
void ir_module_print(IRModule* module, FILE* output);
void ir_function_print(IRFunction* function, FILE* output);
void ir_basic_block_print(IRBasicBlock* block, FILE* output);
void ir_instruction_print(IRInstruction* instruction, FILE* output);
void ir_value_print(IRValue* value, FILE* output);

#endif // IR_H
