#include "diagnostics.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DIAGNOSTICS_INITIAL_CAPACITY 16

DiagnosticsContext* diagnostics_create() {
    DiagnosticsContext* context = (DiagnosticsContext*)malloc(sizeof(DiagnosticsContext));
    if (!context) {
        return NULL;
    }

    context->diagnostics = (Diagnostic**)malloc(sizeof(Diagnostic*) * DIAGNOSTICS_INITIAL_CAPACITY);
    if (!context->diagnostics) {
        free(context);
        return NULL;
    }

    context->diagnostic_count = 0;
    context->diagnostic_capacity = DIAGNOSTICS_INITIAL_CAPACITY;
    context->had_error = false;
    context->had_warning = false;

    return context;
}

void diagnostics_free(DiagnosticsContext* context) {
    if (!context) return;

    for (size_t i = 0; i < context->diagnostic_count; i++) {
        if (context->diagnostics[i]) {
            diagnostic_free(context->diagnostics[i]);
        }
    }

    free(context->diagnostics);
    free(context);
}

void diagnostics_clear(DiagnosticsContext* context) {
    if (!context) return;

    for (size_t i = 0; i < context->diagnostic_count; i++) {
        if (context->diagnostics[i]) {
            diagnostic_free(context->diagnostics[i]);
        }
    }

    context->diagnostic_count = 0;
    context->had_error = false;
    context->had_warning = false;
}

Diagnostic* diagnostic_create(DiagnosticSeverity severity, int error_code,
                               const char* message, const char* suggestion,
                               const char* filename, int line, int column) {
    Diagnostic* diagnostic = (Diagnostic*)malloc(sizeof(Diagnostic));
    if (!diagnostic) {
        return NULL;
    }

    diagnostic->severity = severity;
    diagnostic->error_code = error_code;
    diagnostic->message = message ? strdup(message) : NULL;
    diagnostic->suggestion = suggestion ? strdup(suggestion) : NULL;
    diagnostic->filename = filename;
    diagnostic->line = line;
    diagnostic->column = column;
    diagnostic->source_line = NULL;
    diagnostic->source_line_length = 0;

    return diagnostic;
}

void diagnostic_free(Diagnostic* diagnostic) {
    if (!diagnostic) return;

    if (diagnostic->message) {
        free(diagnostic->message);
    }
    if (diagnostic->suggestion) {
        free(diagnostic->suggestion);
    }
    if (diagnostic->source_line) {
        free(diagnostic->source_line);
    }

    free(diagnostic);
}

void diagnostics_emit(DiagnosticsContext* context, Diagnostic* diagnostic) {
    if (!context || !diagnostic) return;

    // Resize if needed
    if (context->diagnostic_count >= context->diagnostic_capacity) {
        size_t new_capacity = context->diagnostic_capacity * 2;
        Diagnostic** new_diagnostics = (Diagnostic**)realloc(context->diagnostics,
                                                             sizeof(Diagnostic*) * new_capacity);
        if (!new_diagnostics) {
            diagnostic_free(diagnostic);
            return;
        }
        context->diagnostics = new_diagnostics;
        context->diagnostic_capacity = new_capacity;
    }

    context->diagnostics[context->diagnostic_count++] = diagnostic;

    if (diagnostic->severity == DIAGNOSTIC_ERROR || diagnostic->severity == DIAGNOSTIC_FATAL) {
        context->had_error = true;
    } else if (diagnostic->severity == DIAGNOSTIC_WARNING) {
        context->had_warning = true;
    }
}

void diagnostics_emit_error(DiagnosticsContext* context, int error_code,
                            const char* message, const char* suggestion,
                            const char* filename, int line, int column) {
    Diagnostic* diagnostic = diagnostic_create(DIAGNOSTIC_ERROR, error_code, message,
                                            suggestion, filename, line, column);
    if (diagnostic) {
        diagnostics_emit(context, diagnostic);
    }
}

void diagnostics_emit_warning(DiagnosticsContext* context, int warning_code,
                             const char* message, const char* suggestion,
                             const char* filename, int line, int column) {
    Diagnostic* diagnostic = diagnostic_create(DIAGNOSTIC_WARNING, warning_code, message,
                                            suggestion, filename, line, column);
    if (diagnostic) {
        diagnostics_emit(context, diagnostic);
    }
}

void diagnostics_emit_info(DiagnosticsContext* context, const char* message,
                         const char* filename, int line, int column) {
    Diagnostic* diagnostic = diagnostic_create(DIAGNOSTIC_INFO, 0, message,
                                            NULL, filename, line, column);
    if (diagnostic) {
        diagnostics_emit(context, diagnostic);
    }
}

void diagnostic_print(Diagnostic* diagnostic, FILE* output) {
    if (!diagnostic || !output) return;

    const char* severity_str;
    switch (diagnostic->severity) {
        case DIAGNOSTIC_INFO:
            severity_str = "Info";
            break;
        case DIAGNOSTIC_WARNING:
            severity_str = "Warning";
            break;
        case DIAGNOSTIC_ERROR:
            severity_str = "Error";
            break;
        case DIAGNOSTIC_FATAL:
            severity_str = "Fatal";
            break;
        default:
            severity_str = "Unknown";
            break;
    }

    fprintf(output, "CodeTime %s", severity_str);

    if (diagnostic->error_code > 0) {
        fprintf(output, " [E%03d]", diagnostic->error_code);
    }

    fprintf(output, "\n\n");

    if (diagnostic->message) {
        fprintf(output, "%s\n\n", diagnostic->message);
    }

    if (diagnostic->filename) {
        fprintf(output, "Location:\n");
        fprintf(output, "  %s:%d:%d\n", diagnostic->filename, diagnostic->line, diagnostic->column);
    }

    if (diagnostic->suggestion) {
        fprintf(output, "\nSuggestion:\n");
        fprintf(output, "  %s\n", diagnostic->suggestion);
    }

    fprintf(output, "\n");
}

void diagnostics_print(DiagnosticsContext* context, FILE* output) {
    if (!context || !output) return;

    for (size_t i = 0; i < context->diagnostic_count; i++) {
        if (context->diagnostics[i]) {
            diagnostic_print(context->diagnostics[i], output);
        }
    }

    diagnostics_print_summary(context, output);
}

void diagnostics_print_summary(DiagnosticsContext* context, FILE* output) {
    if (!context || !output) return;

    size_t error_count = 0;
    size_t warning_count = 0;

    for (size_t i = 0; i < context->diagnostic_count; i++) {
        if (context->diagnostics[i]) {
            if (context->diagnostics[i]->severity == DIAGNOSTIC_ERROR ||
                context->diagnostics[i]->severity == DIAGNOSTIC_FATAL) {
                error_count++;
            } else if (context->diagnostics[i]->severity == DIAGNOSTIC_WARNING) {
                warning_count++;
            }
        }
    }

    if (error_count > 0 || warning_count > 0) {
        fprintf(output, "Summary: ");
        if (error_count > 0) {
            fprintf(output, "%zu error%s", error_count, error_count == 1 ? "" : "s");
        }
        if (error_count > 0 && warning_count > 0) {
            fprintf(output, ", ");
        }
        if (warning_count > 0) {
            fprintf(output, "%zu warning%s", warning_count, warning_count == 1 ? "" : "s");
        }
        fprintf(output, "\n");
    }
}
