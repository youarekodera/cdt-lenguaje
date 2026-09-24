#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Compiler* compiler_create() {
    Compiler* compiler = (Compiler*)malloc(sizeof(Compiler));
    if (!compiler) {
        return NULL;
    }

    compiler->input_file = NULL;
    compiler->output_file = NULL;
    compiler->source_code = NULL;
    compiler->lexer = NULL;
    compiler->parser = NULL;
    compiler->semantic_analyzer = NULL;
    compiler->ir_module = NULL;
    compiler->codegen = NULL;
    compiler->diagnostics = diagnostics_create();
    compiler->verbose = false;
    compiler->check_only = false;
    compiler->emit_ir = false;

    return compiler;
}

void compiler_free(Compiler* compiler) {
    if (!compiler) return;

    if (compiler->input_file) {
        free(compiler->input_file);
    }
    if (compiler->output_file) {
        free(compiler->output_file);
    }
    if (compiler->source_code) {
        free(compiler->source_code);
    }
    if (compiler->lexer) {
        lexer_free(compiler->lexer);
    }
    if (compiler->parser) {
        parser_free(compiler->parser);
    }
    if (compiler->semantic_analyzer) {
        semantic_analyzer_free(compiler->semantic_analyzer);
    }
    if (compiler->ir_module) {
        ir_module_free(compiler->ir_module);
    }
    if (compiler->codegen) {
        codegen_free(compiler->codegen);
    }
    if (compiler->diagnostics) {
        diagnostics_free(compiler->diagnostics);
    }

    free(compiler);
}

char* compiler_read_file(const char* filename) {
    if (!filename) {
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';

    fclose(file);

    return content;
}

bool compiler_write_file(const char* filename, const char* content) {
    if (!filename || !content) {
        return false;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n", filename);
        return false;
    }

    fprintf(file, "%s", content);
    fclose(file);

    return true;
}

char* compiler_get_output_filename(const char* input_filename) {
    if (!input_filename) {
        return NULL;
    }

    // Check if input has .cdt extension
    size_t len = strlen(input_filename);
    if (len > 4 && strcmp(input_filename + len - 4, ".cdt") == 0) {
        char* output = (char*)malloc(len - 3 + 2); // Replace .cdt with .c
        if (!output) {
            return NULL;
        }
        strncpy(output, input_filename, len - 4);
        output[len - 4] = '\0';
        strcat(output, ".c");
        return output;
    }

    // If no .cdt extension, append .c
    char* output = (char*)malloc(len + 3);
    if (!output) {
        return NULL;
    }
    strcpy(output, input_filename);
    strcat(output, ".c");
    return output;
}

bool compiler_load_file(Compiler* compiler, const char* filename) {
    if (!compiler || !filename) {
        return false;
    }

    compiler->input_file = strdup(filename);
    if (!compiler->input_file) {
        return false;
    }

    compiler->source_code = compiler_read_file(filename);
    if (!compiler->source_code) {
        return false;
    }

    return true;
}

void compiler_set_verbose(Compiler* compiler, bool verbose) {
    if (compiler) {
        compiler->verbose = verbose;
    }
}

void compiler_set_check_only(Compiler* compiler, bool check_only) {
    if (compiler) {
        compiler->check_only = check_only;
    }
}

void compiler_set_emit_ir(Compiler* compiler, bool emit_ir) {
    if (compiler) {
        compiler->emit_ir = emit_ir;
    }
}

bool compiler_check(Compiler* compiler) {
    if (!compiler || !compiler->source_code) {
        return false;
    }

    if (compiler->verbose) {
        printf("Lexing...\n");
    }

    // Create lexer
    compiler->lexer = lexer_create(compiler->source_code, compiler->input_file);
    if (!compiler->lexer) {
        diagnostics_emit_error(compiler->diagnostics, E001_SYNTAX_ERROR,
                             "Failed to create lexer", NULL,
                             compiler->input_file, 0, 0);
        return false;
    }

    if (compiler->verbose) {
        printf("Parsing...\n");
    }

    // Create parser
    compiler->parser = parser_create(compiler->lexer);
    if (!compiler->parser) {
        diagnostics_emit_error(compiler->diagnostics, E001_SYNTAX_ERROR,
                             "Failed to create parser", NULL,
                             compiler->input_file, 0, 0);
        return false;
    }

    // Parse
    ASTProgram* program = parser_parse(compiler->parser);
    if (!program || compiler->parser->had_error) {
        diagnostics_emit_error(compiler->diagnostics, E002_UNEXPECTED_TOKEN,
                             "Parsing failed", "Check your syntax",
                             compiler->input_file, 0, 0);
        return false;
    }

    if (compiler->verbose) {
        printf("Semantic analysis...\n");
    }

    // Semantic analysis
    compiler->semantic_analyzer = semantic_analyzer_create(program);
    if (!compiler->semantic_analyzer) {
        diagnostics_emit_error(compiler->diagnostics, E001_SYNTAX_ERROR,
                             "Failed to create semantic analyzer", NULL,
                             compiler->input_file, 0, 0);
        ast_program_free(program);
        return false;
    }

    bool semantic_ok = semantic_analyze(compiler->semantic_analyzer);
    if (!semantic_ok) {
        diagnostics_emit_error(compiler->diagnostics, E103_TYPE_MISMATCH,
                             "Semantic analysis failed", "Check variable declarations and types",
                             compiler->input_file, 0, 0);
        ast_program_free(program);
        return false;
    }

    ast_program_free(program);

    return true;
}

bool compiler_compile(Compiler* compiler) {
    if (!compiler || !compiler->source_code) {
        return false;
    }

    // Perform checking first
    if (!compiler_check(compiler)) {
        return false;
    }

    if (compiler->check_only) {
        return true;
    }

    if (compiler->verbose) {
        printf("Generating IR...\n");
    }

    // Re-parse for IR generation (in a real compiler, we'd reuse the AST)
    compiler->parser = parser_create(compiler->lexer);
    if (!compiler->parser) {
        return false;
    }

    ASTProgram* program = parser_parse(compiler->parser);
    if (!program) {
        return false;
    }

    // Generate IR
    compiler->ir_module = ir_generate_from_ast(program);
    if (!compiler->ir_module) {
        ast_program_free(program);
        return false;
    }

    if (compiler->emit_ir) {
        if (compiler->verbose) {
            printf("Emitting IR...\n");
        }
        ir_module_print(compiler->ir_module, stdout);
    }

    if (compiler->verbose) {
        printf("Generating code...\n");
    }

    // Generate output filename
    compiler->output_file = compiler_get_output_filename(compiler->input_file);
    if (!compiler->output_file) {
        ast_program_free(program);
        return false;
    }

    // Open output file
    FILE* output = fopen(compiler->output_file, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file '%s'\n", compiler->output_file);
        ast_program_free(program);
        return false;
    }

    // Generate code
    compiler->codegen = codegen_create(compiler->ir_module, output);
    if (!compiler->codegen) {
        fclose(output);
        ast_program_free(program);
        return false;
    }

    bool codegen_ok = codegen_generate(compiler->codegen);
    fclose(output);

    ast_program_free(program);

    if (!codegen_ok) {
        return false;
    }

    if (compiler->verbose) {
        printf("Compilation successful: %s\n", compiler->output_file);
    }

    return true;
}

bool compiler_build(Compiler* compiler, const char* output_path) {
    if (!compiler) {
        return false;
    }

    // Compile to C
    if (!compiler_compile(compiler)) {
        return false;
    }

    // Compile C code with gcc
    char command[512];
    if (output_path) {
        snprintf(command, sizeof(command), "gcc -o %s %s", output_path, compiler->output_file);
    } else {
        // Generate executable name from input
        char exe_name[256];
        strncpy(exe_name, compiler->input_file, sizeof(exe_name) - 1);
        exe_name[sizeof(exe_name) - 1] = '\0';

        // Remove .cdt extension
        size_t len = strlen(exe_name);
        if (len > 4 && strcmp(exe_name + len - 4, ".cdt") == 0) {
            exe_name[len - 4] = '\0';
        }

        snprintf(command, sizeof(command), "gcc -o %s %s", exe_name, compiler->output_file);
    }

    if (compiler->verbose) {
        printf("Running: %s\n", command);
    }

    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error: C compilation failed\n");
        return false;
    }

    if (compiler->verbose) {
        printf("Build successful\n");
    }

    return true;
}

bool compiler_run(Compiler* compiler, const char* filename) {
    if (!compiler) {
        return false;
    }

    if (!compiler_load_file(compiler, filename)) {
        return false;
    }

    if (!compiler_build(compiler, NULL)) {
        return false;
    }

    // Generate executable name
    char exe_name[256];
    strncpy(exe_name, filename, sizeof(exe_name) - 1);
    exe_name[sizeof(exe_name) - 1] = '\0';

    size_t len = strlen(exe_name);
    if (len > 4 && strcmp(exe_name + len - 4, ".cdt") == 0) {
        exe_name[len - 4] = '\0';
    }

    if (compiler->verbose) {
        printf("Running: %s\n", exe_name);
    }

    int result = system(exe_name);
    return result == 0;
}
