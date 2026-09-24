#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Diagnostic severity levels
typedef enum {
    DIAGNOSTIC_INFO,
    DIAGNOSTIC_WARNING,
    DIAGNOSTIC_ERROR,
    DIAGNOSTIC_FATAL
} DiagnosticSeverity;

// Diagnostic structure
typedef struct {
    DiagnosticSeverity severity;
    int error_code;
    char* message;
    char* suggestion;
    const char* filename;
    int line;
    int column;
    size_t source_line_length;
    char* source_line;
} Diagnostic;

// Diagnostics context
typedef struct {
    Diagnostic** diagnostics;
    size_t diagnostic_count;
    size_t diagnostic_capacity;
    bool had_error;
    bool had_warning;
} DiagnosticsContext;

// Diagnostics functions
DiagnosticsContext* diagnostics_create();
void diagnostics_free(DiagnosticsContext* context);
void diagnostics_clear(DiagnosticsContext* context);

Diagnostic* diagnostic_create(DiagnosticSeverity severity, int error_code,
                               const char* message, const char* suggestion,
                               const char* filename, int line, int column);
void diagnostic_free(Diagnostic* diagnostic);

void diagnostics_emit(DiagnosticsContext* context, Diagnostic* diagnostic);
void diagnostics_emit_error(DiagnosticsContext* context, int error_code,
                            const char* message, const char* suggestion,
                            const char* filename, int line, int column);
void diagnostics_emit_warning(DiagnosticsContext* context, int warning_code,
                             const char* message, const char* suggestion,
                             const char* filename, int line, int column);
void diagnostics_emit_info(DiagnosticsContext* context, const char* message,
                         const char* filename, int line, int column);

// Diagnostic printing
void diagnostics_print(DiagnosticsContext* context, FILE* output);
void diagnostic_print(Diagnostic* diagnostic, FILE* output);
void diagnostics_print_summary(DiagnosticsContext* context, FILE* output);

// Common error codes
#define E001_SYNTAX_ERROR 1
#define E002_UNEXPECTED_TOKEN 2
#define E003_MISSING_TOKEN 3
#define E004_INVALID_SYNTAX 4
#define E101_UNKNOWN_VARIABLE 101
#define E102_UNKNOWN_FUNCTION 102
#define E103_TYPE_MISMATCH 103
#define E104_REDECLARATION 104
#define E105_IMMUTABLE_ASSIGNMENT 105
#define E106_CONSTANT_REASSIGNMENT 106
#define E201_UNDEFINED_SYMBOL 201
#define E202_INVALID_OPERATION 202
#define E203_ARGUMENT_COUNT_MISMATCH 203

#endif // DIAGNOSTICS_H
