#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1.0.0"

void print_usage(const char* program_name) {
    printf("CodeTime Compiler v%s\n", VERSION);
    printf("\n");
    printf("Usage: %s <command> [options] <file>\n", program_name);
    printf("\n");
    printf("Commands:\n");
    printf("  build   Compile a .cdt file to executable\n");
    printf("  run     Compile and run a .cdt file\n");
    printf("  check   Check a .cdt file for errors without compiling\n");
    printf("  format  Format a .cdt file (not yet implemented)\n");
    printf("  version Print version information\n");
    printf("  help    Show this help message\n");
    printf("\n");
    printf("Options:\n");
    printf("  -v, --verbose   Enable verbose output\n");
    printf("  -o <file>       Specify output file\n");
    printf("  --emit-ir       Emit intermediate representation\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s build program.cdt\n", program_name);
    printf("  %s run program.cdt\n", program_name);
    printf("  %s check program.cdt\n", program_name);
    printf("  %s build -o myapp program.cdt\n", program_name);
}

void print_version() {
    printf("CodeTime Compiler v%s\n", VERSION);
    printf("A modern high-level programming language\n");
    printf("Copyright (c) 2026 CodeTime Contributors\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* command = argv[1];
    const char* input_file = NULL;
    const char* output_file = NULL;
    bool verbose = false;
    bool check_only = false;
    bool emit_ir = false;

    // Parse arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--emit-ir") == 0) {
            emit_ir = true;
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        }
    }

    // Handle commands
    if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }

    if (strcmp(command, "version") == 0 || strcmp(command, "--version") == 0) {
        print_version();
        return 0;
    }

    if (strcmp(command, "build") == 0) {
        if (!input_file) {
            fprintf(stderr, "Error: No input file specified\n");
            print_usage(argv[0]);
            return 1;
        }

        Compiler* compiler = compiler_create();
        if (!compiler) {
            fprintf(stderr, "Error: Failed to create compiler\n");
            return 1;
        }

        compiler_set_verbose(compiler, verbose);
        compiler_set_emit_ir(compiler, emit_ir);

        if (!compiler_load_file(compiler, input_file)) {
            compiler_free(compiler);
            return 1;
        }

        bool success = compiler_build(compiler, output_file);

        if (compiler->diagnostics->had_error) {
            diagnostics_print(compiler->diagnostics, stderr);
        }

        compiler_free(compiler);

        return success ? 0 : 1;
    }

    if (strcmp(command, "run") == 0) {
        if (!input_file) {
            fprintf(stderr, "Error: No input file specified\n");
            print_usage(argv[0]);
            return 1;
        }

        Compiler* compiler = compiler_create();
        if (!compiler) {
            fprintf(stderr, "Error: Failed to create compiler\n");
            return 1;
        }

        compiler_set_verbose(compiler, verbose);

        bool success = compiler_run(compiler, input_file);

        if (compiler->diagnostics->had_error) {
            diagnostics_print(compiler->diagnostics, stderr);
        }

        compiler_free(compiler);

        return success ? 0 : 1;
    }

    if (strcmp(command, "check") == 0) {
        if (!input_file) {
            fprintf(stderr, "Error: No input file specified\n");
            print_usage(argv[0]);
            return 1;
        }

        Compiler* compiler = compiler_create();
        if (!compiler) {
            fprintf(stderr, "Error: Failed to create compiler\n");
            return 1;
        }

        compiler_set_verbose(compiler, verbose);
        (void)check_only; // Suppress unused variable warning
        compiler_set_check_only(compiler, true);

        if (!compiler_load_file(compiler, input_file)) {
            compiler_free(compiler);
            return 1;
        }

        bool success = compiler_check(compiler);

        if (compiler->diagnostics->had_error) {
            diagnostics_print(compiler->diagnostics, stderr);
        } else {
            printf("No errors found.\n");
        }

        compiler_free(compiler);

        return success ? 0 : 1;
    }

    if (strcmp(command, "format") == 0) {
        fprintf(stderr, "Error: Format command not yet implemented\n");
        return 1;
    }

    fprintf(stderr, "Error: Unknown command '%s'\n", command);
    print_usage(argv[0]);
    return 1;
}
