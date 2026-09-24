#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"
#include "ir.h"
#include "codegen.h"
#include "diagnostics.h"
#include <stdbool.h>

// Compiler structure
typedef struct {
    char* input_file;
    char* output_file;
    char* source_code;
    Lexer* lexer;
    Parser* parser;
    SemanticAnalyzer* semantic_analyzer;
    IRModule* ir_module;
    CodeGenerator* codegen;
    DiagnosticsContext* diagnostics;
    bool verbose;
    bool check_only;
    bool emit_ir;
} Compiler;

// Compiler functions
Compiler* compiler_create();
void compiler_free(Compiler* compiler);

bool compiler_load_file(Compiler* compiler, const char* filename);
bool compiler_compile(Compiler* compiler);
bool compiler_check(Compiler* compiler);
bool compiler_build(Compiler* compiler, const char* output_path);
bool compiler_run(Compiler* compiler, const char* filename);

// Compiler options
void compiler_set_verbose(Compiler* compiler, bool verbose);
void compiler_set_check_only(Compiler* compiler, bool check_only);
void compiler_set_emit_ir(Compiler* compiler, bool emit_ir);

// Helper functions
char* compiler_read_file(const char* filename);
char* compiler_get_output_filename(const char* input_filename);
bool compiler_write_file(const char* filename, const char* content);

#endif // COMPILER_H
