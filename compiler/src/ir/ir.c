#include "ir.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

IRModule* ir_module_create(const char* name) {
    IRModule* module = (IRModule*)malloc(sizeof(IRModule));
    if (!module) {
        return NULL;
    }

    module->name = name ? strdup(name) : NULL;
    module->functions = NULL;
    module->globals = NULL;
    module->global_count = 0;

    return module;
}

void ir_module_free(IRModule* module) {
    if (!module) return;

    if (module->name) {
        free(module->name);
    }

    IRFunction* func = module->functions;
    while (func) {
        IRFunction* next = func->next;
        ir_function_free(func);
        func = next;
    }

    if (module->globals) {
        for (size_t i = 0; i < module->global_count; i++) {
            if (module->globals[i]) {
                ir_value_free(module->globals[i]);
            }
        }
        free(module->globals);
    }

    free(module);
}

IRFunction* ir_function_create(const char* name) {
    IRFunction* function = (IRFunction*)malloc(sizeof(IRFunction));
    if (!function) {
        return NULL;
    }

    function->name = name ? strdup(name) : NULL;
    function->entry_block = NULL;
    function->blocks = NULL;
    function->parameters = NULL;
    function->parameter_count = 0;
    function->return_value = NULL;
    function->next = NULL;

    return function;
}

void ir_function_free(IRFunction* function) {
    if (!function) return;

    if (function->name) {
        free(function->name);
    }

    IRBasicBlock* block = function->blocks;
    while (block) {
        IRBasicBlock* next = block->next;
        ir_basic_block_free(block);
        block = next;
    }

    if (function->parameters) {
        for (size_t i = 0; i < function->parameter_count; i++) {
            if (function->parameters[i]) {
                ir_value_free(function->parameters[i]);
            }
        }
        free(function->parameters);
    }

    if (function->return_value) {
        ir_value_free(function->return_value);
    }

    free(function);
}

IRBasicBlock* ir_basic_block_create(const char* name) {
    IRBasicBlock* block = (IRBasicBlock*)malloc(sizeof(IRBasicBlock));
    if (!block) {
        return NULL;
    }

    block->name = name ? strdup(name) : NULL;
    block->instructions = NULL;
    block->next = NULL;
    block->predecessors = NULL;
    block->predecessor_count = 0;
    block->successors = NULL;
    block->successor_count = 0;

    return block;
}

void ir_basic_block_free(IRBasicBlock* block) {
    if (!block) return;

    if (block->name) {
        free(block->name);
    }

    IRInstruction* instr = block->instructions;
    while (instr) {
        IRInstruction* next = instr->next;
        ir_instruction_free(instr);
        instr = next;
    }

    if (block->predecessors) {
        free(block->predecessors);
    }

    if (block->successors) {
        free(block->successors);
    }

    free(block);
}

IRInstruction* ir_instruction_create(IRInstructionType type) {
    IRInstruction* instruction = (IRInstruction*)malloc(sizeof(IRInstruction));
    if (!instruction) {
        return NULL;
    }

    instruction->type = type;
    instruction->result = NULL;
    instruction->operand1 = NULL;
    instruction->operand2 = NULL;
    instruction->line = 0;
    instruction->column = 0;
    instruction->next = NULL;

    return instruction;
}

void ir_instruction_free(IRInstruction* instruction) {
    if (!instruction) return;

    if (instruction->result) {
        ir_value_free(instruction->result);
    }
    if (instruction->operand1) {
        ir_value_free(instruction->operand1);
    }
    if (instruction->operand2) {
        ir_value_free(instruction->operand2);
    }

    free(instruction);
}

IRValue* ir_value_create(IRValueType type) {
    IRValue* value = (IRValue*)malloc(sizeof(IRValue));
    if (!value) {
        return NULL;
    }

    value->type = type;
    memset(&value->data, 0, sizeof(value->data));

    return value;
}

void ir_value_free(IRValue* value) {
    if (!value) return;

    if (value->type == IR_VALUE_STRING && value->data.string_value) {
        free(value->data.string_value);
    }
    if (value->type == IR_VALUE_LABEL && value->data.label_name) {
        free(value->data.label_name);
    }

    free(value);
}

IRContext* ir_context_create(IRModule* module) {
    IRContext* context = (IRContext*)malloc(sizeof(IRContext));
    if (!context) {
        return NULL;
    }

    context->module = module;
    context->current_function = NULL;
    context->current_block = NULL;
    context->temp_counter = 0;
    context->label_counter = 0;
    context->register_counter = 0;

    return context;
}

void ir_context_free(IRContext* context) {
    if (context) {
        free(context);
    }
}

char* ir_generate_temp_name(IRContext* context) {
    if (!context) return NULL;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t%d", context->temp_counter++);
    return strdup(buffer);
}

char* ir_generate_label_name(IRContext* context) {
    if (!context) return NULL;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "L%d", context->label_counter++);
    return strdup(buffer);
}

IRValue* ir_create_int_constant(long value) {
    IRValue* ir_value = ir_value_create(IR_VALUE_INT);
    if (!ir_value) {
        return NULL;
    }

    ir_value->data.int_value = value;
    return ir_value;
}

IRValue* ir_create_float_constant(double value) {
    IRValue* ir_value = ir_value_create(IR_VALUE_FLOAT);
    if (!ir_value) {
        return NULL;
    }

    ir_value->data.float_value = value;
    return ir_value;
}

IRValue* ir_create_string_constant(const char* value) {
    IRValue* ir_value = ir_value_create(IR_VALUE_STRING);
    if (!ir_value) {
        return NULL;
    }

    ir_value->data.string_value = value ? strdup(value) : NULL;
    return ir_value;
}

IRValue* ir_create_boolean_constant(bool value) {
    IRValue* ir_value = ir_value_create(IR_VALUE_BOOLEAN);
    if (!ir_value) {
        return NULL;
    }

    ir_value->data.bool_value = value;
    return ir_value;
}

IRValue* ir_create_temp_register(IRContext* context) {
    if (!context) return NULL;

    IRValue* ir_value = ir_value_create(IR_VALUE_REGISTER);
    if (!ir_value) {
        return NULL;
    }

    ir_value->data.register_number = context->register_counter++;
    return ir_value;
}

IRInstruction* ir_emit_instruction(IRContext* context, IRInstructionType type,
                                   IRValue* result, IRValue* operand1, IRValue* operand2) {
    if (!context || !context->current_block) {
        return NULL;
    }

    IRInstruction* instruction = ir_instruction_create(type);
    if (!instruction) {
        return NULL;
    }

    instruction->result = result;
    instruction->operand1 = operand1;
    instruction->operand2 = operand2;

    // Add to current block
    if (context->current_block->instructions == NULL) {
        context->current_block->instructions = instruction;
    } else {
        IRInstruction* last = context->current_block->instructions;
        while (last->next) {
            last = last->next;
        }
        last->next = instruction;
    }

    return instruction;
}

IRInstruction* ir_emit_binary_op(IRContext* context, IRInstructionType type,
                                  IRValue* left, IRValue* right) {
    if (!context) return NULL;

    IRValue* result = ir_create_temp_register(context);
    return ir_emit_instruction(context, type, result, left, right);
}

IRInstruction* ir_emit_unary_op(IRContext* context, IRInstructionType type, IRValue* operand) {
    if (!context) return NULL;

    IRValue* result = ir_create_temp_register(context);
    return ir_emit_instruction(context, type, result, operand, NULL);
}

IRModule* ir_generate_from_ast(ASTProgram* program) {
    if (!program) {
        return NULL;
    }

    IRModule* module = ir_module_create(program->module ? program->module->name : "main");
    if (!module) {
        return NULL;
    }

    IRContext* context = ir_context_create(module);
    if (!context) {
        ir_module_free(module);
        return NULL;
    }

    // Generate IR for each declaration
    for (size_t i = 0; i < program->declaration_count; i++) {
        if (program->declarations[i]) {
            switch (program->declarations[i]->base.type) {
                case NODE_FUNCTION_DECLARATION: {
                    IRFunction* func = ir_generate_function(
                        (ASTFunctionDeclaration*)program->declarations[i], context);
                    if (func) {
                        // Add to module
                        if (module->functions == NULL) {
                            module->functions = func;
                        } else {
                            IRFunction* last = module->functions;
                            while (last->next) {
                                last = last->next;
                            }
                            last->next = func;
                        }
                    }
                    break;
                }
                default:
                    // Handle other declaration types
                    break;
            }
        }
    }

    ir_context_free(context);

    return module;
}

IRFunction* ir_generate_function(ASTFunctionDeclaration* decl, IRContext* context) {
    if (!decl || !context) {
        return NULL;
    }

    IRFunction* function = ir_function_create(decl->name);
    if (!function) {
        return NULL;
    }

    context->current_function = function;

    // Create entry block
    IRBasicBlock* entry_block = ir_basic_block_create("entry");
    if (!entry_block) {
        ir_function_free(function);
        return NULL;
    }

    function->entry_block = entry_block;
    function->blocks = entry_block;
    context->current_block = entry_block;

    // Generate IR for function body
    if (decl->body) {
        ir_generate_block(decl->body, context);
    }

    context->current_function = NULL;
    context->current_block = NULL;

    return function;
}

IRBasicBlock* ir_generate_block(ASTBlock* block, IRContext* context) {
    if (!block || !context) {
        return NULL;
    }

    for (size_t i = 0; i < block->statement_count; i++) {
        if (block->statements[i]) {
            ir_generate_statement(block->statements[i], context);
        }
    }

    return context->current_block;
}

IRInstruction* ir_generate_expression(ASTExpression* expr, IRContext* context) {
    if (!expr || !context) {
        return NULL;
    }

    switch (expr->base.type) {
        case NODE_LITERAL_EXPRESSION: {
            ASTLiteralExpression* literal = (ASTLiteralExpression*)expr;
            IRValue* value = NULL;

            switch (literal->literal_type) {
                case TOKEN_NUMBER:
                    value = ir_create_int_constant(literal->value.number_value);
                    break;
                case TOKEN_DECIMAL:
                    value = ir_create_float_constant(literal->value.decimal_value);
                    break;
                case TOKEN_TEXT:
                    value = ir_create_string_constant(literal->value.text_value);
                    break;
                case TOKEN_BOOLEAN:
                    value = ir_create_boolean_constant(literal->value.boolean_value);
                    break;
                default:
                    break;
            }

            if (value) {
                return ir_emit_instruction(context, IR_CONST, value, NULL, NULL);
            }
            break;
        }

        case NODE_BINARY_EXPRESSION: {
            ASTBinaryExpression* bin_expr = (ASTBinaryExpression*)expr;
            IRInstruction* left = ir_generate_expression(bin_expr->left, context);
            IRInstruction* right = ir_generate_expression(bin_expr->right, context);

            if (left && right) {
                IRInstructionType op_type;

                switch (bin_expr->operator) {
                    case TOKEN_PLUS:
                        op_type = IR_ADD;
                        break;
                    case TOKEN_MINUS:
                        op_type = IR_SUB;
                        break;
                    case TOKEN_MULTIPLY:
                        op_type = IR_MUL;
                        break;
                    case TOKEN_DIVIDE:
                        op_type = IR_DIV;
                        break;
                    case TOKEN_MODULO:
                        op_type = IR_MOD;
                        break;
                    default:
                        op_type = IR_NOP;
                        break;
                }

                return ir_emit_binary_op(context, op_type, left->result, right->result);
            }
            break;
        }

        case NODE_IDENTIFIER_EXPRESSION: {
            ASTIdentifierExpression* ident = (ASTIdentifierExpression*)expr;
            (void)ident; // Suppress unused variable warning
            // For now, just emit a load instruction
            IRValue* result = ir_create_temp_register(context);
            return ir_emit_instruction(context, IR_LOAD, result, NULL, NULL);
        }

        default:
            break;
    }

    return NULL;
}

IRInstruction* ir_generate_statement(ASTStatement* stmt, IRContext* context) {
    if (!stmt || !context) {
        return NULL;
    }

    switch (stmt->base.type) {
        case NODE_EXPRESSION_STATEMENT: {
            ASTExpressionStatement* expr_stmt = (ASTExpressionStatement*)stmt;
            return ir_generate_expression(expr_stmt->expression, context);
        }

        case NODE_RETURN_STATEMENT: {
            ASTReturnStatement* ret_stmt = (ASTReturnStatement*)stmt;
            IRValue* value = NULL;

            if (ret_stmt->value) {
                IRInstruction* expr_instr = ir_generate_expression(ret_stmt->value, context);
                if (expr_instr) {
                    value = expr_instr->result;
                }
            }

            return ir_emit_instruction(context, IR_RETURN, value, NULL, NULL);
        }

        default:
            break;
    }

    return NULL;
}

void ir_value_print(IRValue* value, FILE* output) {
    if (!value || !output) return;

    switch (value->type) {
        case IR_VALUE_INT:
            fprintf(output, "%ld", value->data.int_value);
            break;
        case IR_VALUE_FLOAT:
            fprintf(output, "%f", value->data.float_value);
            break;
        case IR_VALUE_STRING:
            fprintf(output, "\"%s\"", value->data.string_value ? value->data.string_value : "");
            break;
        case IR_VALUE_BOOLEAN:
            fprintf(output, "%s", value->data.bool_value ? "true" : "false");
            break;
        case IR_VALUE_REGISTER:
            fprintf(output, "r%d", value->data.register_number);
            break;
        case IR_VALUE_LABEL:
            fprintf(output, "%s", value->data.label_name ? value->data.label_name : "");
            break;
        case IR_VALUE_TEMP:
            fprintf(output, "t%ld", value->data.int_value);
            break;
    }
}

void ir_instruction_print(IRInstruction* instruction, FILE* output) {
    if (!instruction || !output) return;

    switch (instruction->type) {
        case IR_NOP:
            fprintf(output, "nop");
            break;
        case IR_CONST:
            fprintf(output, "const ");
            ir_value_print(instruction->result, output);
            fprintf(output, " = ");
            ir_value_print(instruction->operand1, output);
            break;
        case IR_LOAD:
            fprintf(output, "load ");
            ir_value_print(instruction->result, output);
            break;
        case IR_STORE:
            fprintf(output, "store ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->result, output);
            break;
        case IR_ADD:
            fprintf(output, "add ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_SUB:
            fprintf(output, "sub ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_MUL:
            fprintf(output, "mul ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_DIV:
            fprintf(output, "div ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_MOD:
            fprintf(output, "mod ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_NEG:
            fprintf(output, "neg ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            break;
        case IR_CALL:
            fprintf(output, "call ");
            ir_value_print(instruction->result, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand1, output);
            break;
        case IR_RETURN:
            fprintf(output, "return ");
            if (instruction->result) {
                ir_value_print(instruction->result, output);
            }
            break;
        case IR_JUMP:
            fprintf(output, "jump ");
            ir_value_print(instruction->operand1, output);
            break;
        case IR_JUMP_IF:
            fprintf(output, "jump_if ");
            ir_value_print(instruction->operand1, output);
            fprintf(output, ", ");
            ir_value_print(instruction->operand2, output);
            break;
        case IR_LABEL:
            fprintf(output, "label ");
            ir_value_print(instruction->result, output);
            break;
        case IR_PARAM:
            fprintf(output, "param ");
            ir_value_print(instruction->result, output);
            break;
        case IR_PHI:
            fprintf(output, "phi ");
            ir_value_print(instruction->result, output);
            break;
    }
}

void ir_basic_block_print(IRBasicBlock* block, FILE* output) {
    if (!block || !output) return;

    fprintf(output, "%s:\n", block->name ? block->name : "unnamed");

    IRInstruction* instr = block->instructions;
    while (instr) {
        fprintf(output, "  ");
        ir_instruction_print(instr, output);
        fprintf(output, "\n");
        instr = instr->next;
    }
}

void ir_function_print(IRFunction* function, FILE* output) {
    if (!function || !output) return;

    fprintf(output, "function %s\n", function->name ? function->name : "unnamed");

    IRBasicBlock* block = function->blocks;
    while (block) {
        ir_basic_block_print(block, output);
        block = block->next;
    }

    fprintf(output, "\n");
}

void ir_module_print(IRModule* module, FILE* output) {
    if (!module || !output) return;

    fprintf(output, "module %s\n\n", module->name ? module->name : "unnamed");

    IRFunction* func = module->functions;
    while (func) {
        ir_function_print(func, output);
        func = func->next;
    }
}
