#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"
#include <stdbool.h>

// Code generator structure
typedef struct {
    IRModule* module;
    FILE* output;
    int indent_level;
    bool had_error;
    char* error_message;
} CodeGenerator;

// Code generator functions
CodeGenerator* codegen_create(IRModule* module, FILE* output);
void codegen_free(CodeGenerator* generator);
bool codegen_generate(CodeGenerator* generator);

// C code generation functions
bool codegen_generate_c(CodeGenerator* generator);
bool codegen_generate_c_header(CodeGenerator* generator);
bool codegen_generate_c_function(CodeGenerator* generator, IRFunction* function);
bool codegen_generate_c_basic_block(CodeGenerator* generator, IRBasicBlock* block);
bool codegen_generate_c_instruction(CodeGenerator* generator, IRInstruction* instruction);
const char* codegen_c_type_from_ir(IRValueType type);

// Helper functions
void codegen_emit_indent(CodeGenerator* generator);
void codegen_emit(CodeGenerator* generator, const char* format, ...);
void codegen_emit_line(CodeGenerator* generator, const char* format, ...);

#endif // CODEGEN_H
