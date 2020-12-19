#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

typedef struct {
    const char * name;
    void       * pointer;
} symbol_t;

typedef enum {
    UNDEFINED,
    SPECIAL_SYMBOL,
    CONSTANT,
    VARIABLE,
    OPERATION,
    FUNCTION,
    END_OF_EXPRESSION
} token_types_e;

typedef struct {
    int value;
    char operation;
    char* name;
    void* function_ptr;
    int function_valence;
    char special_symbol;
    char is_end;
    token_types_e type;
} token_t;

void dereference_value(token_t* token,
                       const char* current_string,
                       token_types_e token_type,
                       const symbol_t* externs) {
    int i = 0;
    while (externs[i].name != 0 && externs[i].pointer != 0) {
        if (!(strcmp(externs[i].name, current_string))) {
            if (token_type == VARIABLE) {
                token->value = (*((int*)externs[i].pointer));
            } else if (token_type == FUNCTION) {
                token->function_ptr = externs[i].pointer;
                token->name = malloc(strlen(externs[i].name));
                strcpy(token->name, externs[i].name);
            }
        }
        ++i;
    }
}

void process_string(char* current_string,
                    int* string_index,
                    size_t expression_len,
                    token_types_e* current_token_type,
                    token_t* tokenized_str,
                    int* index,
                    const symbol_t* externs) {
    current_string[*string_index] = '\0';
    
    if ((*current_token_type) == CONSTANT) {
        tokenized_str[*index].value = atoi(current_string);
        
    } else if ((*current_token_type) == VARIABLE || (*current_token_type) == FUNCTION) {
        dereference_value(&tokenized_str[*index], current_string, *current_token_type, externs);
        
    } else if ((*current_token_type) == OPERATION) {
        tokenized_str[*index].operation = current_string[0];
        
    } else if ((*current_token_type) == SPECIAL_SYMBOL) {
        tokenized_str[*index].special_symbol = current_string[0];
        
    }

    tokenized_str[*index].type = (*current_token_type);

    (*current_token_type) = UNDEFINED;
    current_string[0] = '\0';
    (*string_index) = 0;
    ++(*index);
}

void tokenize_expr (const char* expresion, token_t* tokenized_expr, const symbol_t* externs) {
    int tokenized_str_index = 0;
    size_t expression_len = strlen(expresion);

    token_types_e current_token_type = UNDEFINED;

    char* current_string = malloc(expression_len * sizeof(char));
    int string_index = 0;

    for (int i = 0; i < expression_len; ++i) {

        if (expresion[i] >= '0' && expresion[i] <= '9') {
            if (current_token_type == UNDEFINED) {
                current_token_type = CONSTANT;
            } else if (current_token_type != CONSTANT && current_token_type != VARIABLE) {
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = CONSTANT;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if ((expresion[i] >= 'a' && expresion[i] <= 'z') || (expresion[i] >= 'A' && expresion[i] <= 'Z')) {
            if (current_token_type == UNDEFINED) {
                current_token_type = VARIABLE;
            } else if (current_token_type != VARIABLE) {
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = VARIABLE;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if (expresion[i] == '+' || expresion[i] == '-' || expresion[i] == '*') {
            if (expresion[i] == '-') {
                if (current_string[string_index - 1] == ',' || current_string[string_index - 1] == '(') {
                    ++string_index;
                    process_string(current_string, &string_index, expression_len, &current_token_type,
                                   tokenized_expr, &tokenized_str_index, externs);
                    current_token_type = UNDEFINED;
                }

                if (current_token_type == UNDEFINED) {
                    current_string[string_index] = '0';
                    ++string_index;
                    
                    current_token_type = CONSTANT;
                    process_string(current_string, &string_index, expression_len, &current_token_type,
                                   tokenized_expr, &tokenized_str_index, externs);
                    current_token_type = UNDEFINED;
                }
            }
            if (current_token_type == UNDEFINED) {
                current_token_type = OPERATION;
            }
            if (current_token_type != OPERATION) {
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = OPERATION;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if (expresion[i] == '(' || expresion[i] == ')' || expresion[i] == ',') {
            if (current_token_type == SPECIAL_SYMBOL) {
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = UNDEFINED;
            }

            if (current_token_type == UNDEFINED) {
                current_token_type = SPECIAL_SYMBOL;
            }
            if (expresion[i] == '(' && current_token_type == VARIABLE) {
                current_token_type = FUNCTION;
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = SPECIAL_SYMBOL;
            } else if (current_token_type != SPECIAL_SYMBOL) {
                process_string(current_string, &string_index, expression_len, &current_token_type,
                               tokenized_expr, &tokenized_str_index, externs);
                current_token_type = SPECIAL_SYMBOL;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if (expresion[i] == '_') {
            if (current_token_type == UNDEFINED) {
                current_token_type = VARIABLE;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        }
    }
    if (!(current_token_type == UNDEFINED)) {
        process_string(current_string, &string_index, expression_len, &current_token_type,
                       tokenized_expr, &tokenized_str_index, externs);
    }

    tokenized_expr[tokenized_str_index].type = END_OF_EXPRESSION;
    free(current_string);
}

void tokenized_expr_to_str (char** tokenized_str, token_t* tokenized_expr) {
    int cnt = 0;
    for (size_t i = 0; tokenized_expr[i].type != END_OF_EXPRESSION; ++i) {
        tokenized_str[i] = malloc(100 * sizeof(char));
        if (tokenized_expr[i].type == CONSTANT || tokenized_expr[i].type == VARIABLE) {
            sprintf(tokenized_str[i], "%d", tokenized_expr[i].value);
        } else if (tokenized_expr[i].type == FUNCTION) {
            strcpy(tokenized_str[i], tokenized_expr[i].name);
        } else if (tokenized_expr[i].type == OPERATION) {
            strcpy(tokenized_str[i], &tokenized_expr[i].operation);
        } else if (tokenized_expr[i].type == SPECIAL_SYMBOL) {
            sprintf(tokenized_str[i], "%c", tokenized_expr[i].special_symbol);
        }
        ++cnt;
    }
    for (int i = 0; i < cnt; ++i) {
        printf("%s ", tokenized_str[i]);
    }
}

int define_operation_priority (token_t token) {
    if (token.type == SPECIAL_SYMBOL) {
        if (token.special_symbol == '(' || token.special_symbol == ')') {
            return 0;
        }
    } else if (token.type == OPERATION) {
        if (token.operation == '+' || token.operation == '-') {
            return 1;
        }
        if (token.operation == '*') {
            return 2;
        }
    } else if (token.type == FUNCTION) {
        return 3;
    }
    return 4;
};

void append_function_token(token_t* to, int* index, char* name, void* function_ptr, int function_valence) {
    ++(*index);
    to[*index].name = malloc(strlen(name));
    strcpy(to[*index].name, name);
    to[*index].type = FUNCTION;
    to[*index].function_ptr = function_ptr;
    if (function_valence != -1) {
        to[*index].function_valence = function_valence;
    }
}
    
void append_operation_token(token_t* to, int* index, char operation) {
    ++(*index);
    to[*index].operation = operation;
    to[*index].type = OPERATION;
}

void append_special_token(token_t* to, int* index, char symbol) {
    ++(*index);
    to[*index].special_symbol = symbol;
    to[*index].type = SPECIAL_SYMBOL;
}

void append_constant_token(token_t* to, int* index, int constant) {
    ++(*index);
    to[*index].value = constant;
    to[*index].type = CONSTANT;
}

void make_poliz (token_t* poliz, const token_t* tokenized_expr, token_t* stack) {
    int i = 0;

    int poliz_index = -1;
    int stack_index = -1;

    int* valence_stack = calloc(100, sizeof(int));
    int valence_index = -1;

    while (tokenized_expr[i].type != END_OF_EXPRESSION) {
        if (tokenized_expr[i].type == CONSTANT || tokenized_expr[i].type == VARIABLE) {
            append_constant_token(poliz, &poliz_index, tokenized_expr[i].value);
            
        } else if (tokenized_expr[i].type == SPECIAL_SYMBOL) {
            if (tokenized_expr[i].special_symbol == '(') {
                ++valence_index;
                append_special_token(stack, &stack_index, '(');
                
            } else if (tokenized_expr[i].special_symbol == ',' || tokenized_expr[i].special_symbol == ')') {
                while (stack[stack_index].type != SPECIAL_SYMBOL)  {
                    
                    if (stack[stack_index].type == OPERATION) {
                        append_operation_token(poliz, &poliz_index, stack[stack_index].operation);
                        
                    } else if (stack[stack_index].type == FUNCTION) {
                        append_function_token(poliz,
                                              &poliz_index,
                                              stack[stack_index].name,
                                              stack[stack_index].function_ptr,
                                              stack[stack_index].function_valence);
                    }
                    --stack_index;
                }
                if (tokenized_expr[i].special_symbol == ',') {
                    ++valence_stack[valence_index];
                }
                else {
                    --stack_index;
                    if (stack_index >= 0 && stack[stack_index].type == FUNCTION) {
                        stack[stack_index].function_valence = valence_stack[valence_index] + 1;
                    }
                    --valence_index;
                }

            }
        } else if (tokenized_expr[i].type == OPERATION) {
            while (stack_index >= 0 && (stack[stack_index].type == OPERATION
                                        || stack[stack_index].type == FUNCTION) &&
                   define_operation_priority(stack[stack_index]) >=
                   define_operation_priority(tokenized_expr[i])) {

                if (stack[stack_index].type == OPERATION) {
                    append_operation_token(poliz, &poliz_index, stack[stack_index].operation);
                    
                } else if (stack[stack_index].type == FUNCTION) {
                    
                    append_function_token(poliz,
                                          &poliz_index,
                                          stack[stack_index].name,
                                          stack[stack_index].function_ptr,
                                          stack[stack_index].function_valence);
                    
                }
                --stack_index;
            }
            append_operation_token(stack, &stack_index, tokenized_expr[i].operation);

        } else if (tokenized_expr[i].type == FUNCTION) {
            append_function_token(stack,
                                  &stack_index,
                                  tokenized_expr[i].name,
                                  tokenized_expr[i].function_ptr,
                                  -1);
            
        }
        ++i;
    }

    while (stack_index >= 0) {
        if (stack[stack_index].type == OPERATION) {
            append_operation_token(poliz, &poliz_index, stack[stack_index].operation);
            
        } else if (stack[stack_index].type == FUNCTION) {
            append_function_token(poliz,
                                  &poliz_index,
                                  stack[stack_index].name,
                                  stack[stack_index].function_ptr,
                                  stack[stack_index].function_valence);
        }
        --stack_index;
    }
    ++poliz_index;
    poliz[poliz_index].type = END_OF_EXPRESSION;
}

void print_poliz(token_t* poliz) {
    printf("\n");
    for (size_t i = 0; poliz[i].type != END_OF_EXPRESSION; ++i) {
        if (poliz[i].type == CONSTANT || poliz[i].type == VARIABLE) {
            printf("%d ", poliz[i].value);
        } else if (poliz[i].type == FUNCTION) {
            printf("%s ", poliz[i].name);
        } else if (poliz[i].type == OPERATION) {
            printf("%c ", poliz[i].operation);
        } else if (poliz[i].type == SPECIAL_SYMBOL) {
            printf("%c", poliz[i].special_symbol);
        }
    }
}

typedef enum {
    mov,
    sub,
    add,
    mul,
    ldr,
    str,
    lsl,
    push,
    pop,
    bx,
    END
} arm_operations_e;

typedef enum {
    code_mov_r0_0 = 0xe3a00000, // code for r0 = #0
    code_mov_r1_0 = 0xe3a01000, // code for r1 = #0
    code_mov_r5_0 = 0xe3a05000, // code for r5 = #0
    code_mov_lr_pc = 0xe1a0e00f, // code for lr = pc
    code_sub_sp_4 = 0xe24dd004, // code for sp -= 4
    code_add_sp_4 = 0xe28dd004, // code for sp += 4
    code_add_lr_4 = 0xe28ee004, // code for lr += 4
    code_sub_r0_r1 = 0xe0400001, // code for r0 -= r1
    code_add_r0_r1 = 0xe0800001, // code for r0 += r1
    code_add_r5_r1 = 0xe0855001, // code for r5 += r1
    code_mul_r0_r1 = 0xe0000091, // code for r0 *= r1
    code_ldr_r0_sp = 0xe59d0000, // code for ldr r0, [sp]
    code_ldr_r1_sp = 0xe59d1000, // code for ldr r1, [sp]
    code_ldr_r2_sp = 0xe59d2000, // code for ldr r2, [sp]
    code_ldr_r3_sp = 0xe59d3000, // code for ldr r3, [sp]
    code_str_r0_sp = 0xe58d0000, // code for str r0, [sp]
    code_lsl_r1_8 = 0xe1a01401, // code for lsl r1, #8
    code_lsl_r1_16 = 0xe1a01801, // code for lsl r1, #16
    code_lsl_r1_24 = 0xe1a01c01, // code for lsl r1, #24
    code_push_r5_lr = 0xe92d4020, // code for push {r5, lr}
    code_pop_r5_lr = 0xe8bd4020, // code for pop {r5, lr}
    code_bx_r5 = 0xe12fff15, // code for bx r5
    code_bx_lr = 0xe12fff1e, // code for bx lr
} arm_operations_codes_e;

char* print_operation(arm_operations_e op) {
    char *out;
    if (op == mov) {
        out = "mov";
    } else if (op == sub) {
        out = "sub";
    } else if (op == add) {
        out = "add";
    } else if (op == mul) {
        out = "mul";
    } else if (op == ldr) {
        out = "ldr";
    } else if (op == str) {
        out = "str";
    } else if (op == lsl) {
        out = "lsl";
    } else if (op == push) {
        out = "push";
    } else if (op == pop) {
        out = "pop";
    } else {
        out = "bx";
    }
    return out;
}

typedef enum {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    sp,
    lr,
    pc,
    nan
} arm_registers_e;

char* print_register(arm_registers_e reg) {
    char *out;
    if (reg == r0) {
        out = "r0";
    } else if (reg == r1) {
        out = "r1";
    } else if (reg == r2) {
        out = "r2";
    } else if (reg == r3) {
        out = "r3";
    } else if (reg == r5) {
        out = "r5";
    } else if (reg == sp) {
        out = "sp";
    } else if (reg == lr) {
        out = "lr";
    } else if (reg == pc) {
        out = "pc";
    } else {
        out = "";
    }
    return out;
}

typedef enum {
    WITH_CONST,
    WITHOUT_CONST,
    MOV_CONST
} CONST_FLAG;

typedef struct {
    arm_operations_e operation;
    arm_registers_e left_register;
    arm_registers_e right_register;
    int constant;
    CONST_FLAG flag;
    int shift;

} arm_instruction_t;

void append_instruction(arm_instruction_t* arm_code,
                                     int* code_index,
                                     arm_operations_e operation,
                                     arm_registers_e first_reg,
                                     arm_registers_e second_reg,
                                     int value,
                                     CONST_FLAG flag) {
    arm_instruction_t instruction;
    instruction.operation = operation;
    instruction.left_register = first_reg;
    instruction.right_register = second_reg;
    instruction.constant = value;
    instruction.flag = flag;
    
    arm_code[*(code_index)] = instruction;
    ++(*code_index);
}

void mov_const(arm_instruction_t* arm_code, int* code_index, int value, arm_registers_e reg) {
    uint32_t first_byte = (value & 0xFF);
    uint32_t second_byte = (value & 0xFF00) >> 8;
    uint32_t third_byte = (value & 0xFF0000) >> 16;
    uint32_t fourth_byte = (value & 0xFF000000) >> 24;

    if (reg != r5) {
        append_instruction(arm_code, code_index, sub, sp, nan, 4, WITH_CONST);
    }

    append_instruction(arm_code, code_index, mov, reg, nan, first_byte, WITH_CONST);

    append_instruction(arm_code, code_index, mov, r1, nan, second_byte, WITH_CONST);

    append_instruction(arm_code, code_index, lsl, r1, nan, 8, WITH_CONST);

    append_instruction(arm_code, code_index, add, reg, r1, -1, WITHOUT_CONST);

    append_instruction(arm_code, code_index, mov, r1, nan, third_byte, WITH_CONST);

    append_instruction(arm_code, code_index, lsl, r1, nan, 16, WITH_CONST);

    append_instruction(arm_code, code_index, add, reg, r1, -1, WITHOUT_CONST);

    append_instruction(arm_code, code_index, mov, r1, nan, fourth_byte, WITH_CONST);

    append_instruction(arm_code, code_index, lsl, r1, nan, 24, WITH_CONST);

    append_instruction(arm_code, code_index, add, reg, r1, -1, WITHOUT_CONST);

    if (reg != r5) {
        append_instruction(arm_code, code_index, str, r0, sp, -1, WITHOUT_CONST);
    }
}

arm_operations_e define_operation(token_t token) {
    arm_operations_e oper;
    if (token.operation == '+') {
        oper = add;
    } else if (token.operation == '-') {
        oper = sub;
    } else {
        oper = mul;
    }
    return oper;
}

void transate_poliz_to_arm(token_t* poliz, arm_instruction_t* arm_code) {
    int poliz_index = 0;
    int code_index = 0;

    append_instruction(arm_code, &code_index, push, r5, lr, -1, WITHOUT_CONST);

    while (poliz[poliz_index].type != END_OF_EXPRESSION) {
        if (poliz[poliz_index].type == CONSTANT) {
            mov_const(arm_code, &code_index, poliz[poliz_index].value, r0);

        } else if (poliz[poliz_index].type == OPERATION) {
            append_instruction(arm_code, &code_index, ldr, r1, sp, -1, WITHOUT_CONST);
            
            append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);
            
            append_instruction(arm_code, &code_index, ldr, r0, sp, -1, WITHOUT_CONST);

            append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);

            arm_operations_e oper = define_operation(poliz[poliz_index]);

            append_instruction(arm_code, &code_index, oper, r0, r1, -1, WITHOUT_CONST);

            append_instruction(arm_code, &code_index, sub, sp, nan, 4, WITH_CONST);

            append_instruction(arm_code, &code_index, str, r0, sp, -1, WITHOUT_CONST);
            
        } else if (poliz[poliz_index].type == FUNCTION) {
            mov_const(arm_code, &code_index, (uint32_t)(poliz[poliz_index].function_ptr), r5);
            
            if (poliz[poliz_index].function_valence > 3) {
                append_instruction(arm_code, &code_index, ldr, r3, sp, -1, WITHOUT_CONST);

                append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);
            }
            if (poliz[poliz_index].function_valence > 2) {
                append_instruction(arm_code, &code_index, ldr, r2, sp, -1, WITHOUT_CONST);

                append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);
            }
            if (poliz[poliz_index].function_valence > 1) {
                append_instruction(arm_code, &code_index, ldr, r1, sp, -1, WITHOUT_CONST);

                append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);
            }
            if (poliz[poliz_index].function_valence > 0) {
                append_instruction(arm_code, &code_index, ldr, r0, sp, -1, WITHOUT_CONST);

                append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);
            }
            append_instruction(arm_code, &code_index, mov, lr, pc, -1, WITHOUT_CONST);

            append_instruction(arm_code, &code_index, add, lr, nan, 4, WITH_CONST);
            
            append_instruction(arm_code, &code_index, bx, r5, nan, -1, WITHOUT_CONST);

            append_instruction(arm_code, &code_index, sub, sp, nan, 4, WITH_CONST);

            append_instruction(arm_code, &code_index, str, r0, sp, -1, WITHOUT_CONST);

        }
        ++poliz_index;
    }
    append_instruction(arm_code, &code_index, ldr, r0, sp, -1, WITHOUT_CONST);

    append_instruction(arm_code, &code_index, add, sp, nan, 4, WITH_CONST);

    append_instruction(arm_code, &code_index, pop, r5, lr, -1, WITHOUT_CONST);

    append_instruction(arm_code, &code_index, bx, lr, nan, -1, WITHOUT_CONST);

    append_instruction(arm_code, &code_index, END, nan, nan, -1, WITHOUT_CONST);
}

void append_byte_code(uint32_t code, uint32_t* out_buffer, int* buffer_index) {
    out_buffer[(*buffer_index)] = code;
    ++(*buffer_index);
}

uint32_t encode_push(arm_instruction_t instruction) {
    return code_push_r5_lr;
}

uint32_t encode_pop(arm_instruction_t instruction) {
    return code_pop_r5_lr;
}

uint32_t encode_sub(arm_instruction_t instruction) {
    return (instruction.left_register == sp) ? code_sub_sp_4 : code_sub_r0_r1;
}

uint32_t encode_add(arm_instruction_t instruction) {
    uint32_t code;
    if (instruction.left_register == lr) {
        code = code_add_lr_4;
    } else if (instruction.left_register == r5) {
        code = code_add_r5_r1;
    } else {
        code = (instruction.left_register == sp) ? code_add_sp_4 : code_add_r0_r1;
    }
    return code;
}

uint32_t encode_mul(arm_instruction_t instruction) {
    return code_mul_r0_r1;
}

uint32_t encode_lsl(arm_instruction_t instruction) {
    uint32_t code;
    if (instruction.constant == 8) {
        code = code_lsl_r1_8;
    } else if (instruction.constant == 16) {
        code = code_lsl_r1_16;
    } else {
        code = code_lsl_r1_24;
    }
    return code;
}

uint32_t encode_mov(arm_instruction_t instruction) {
    uint32_t code;
    if (instruction.left_register == lr) {
        code = code_mov_lr_pc;
    } else {
        if (instruction.left_register == r5) {
            code = code_mov_r5_0;
        } else if (instruction.left_register == r1) {
            code = code_mov_r1_0;
        } else {
            code = code_mov_r0_0;
        }
        code += instruction.constant;
    }
    return code;
}

uint32_t encode_ldr(arm_instruction_t instruction) {
    uint32_t code;
    if (instruction.left_register == r0) {
        code = code_ldr_r0_sp;
    } else if (instruction.left_register == r1) {
        code = code_ldr_r1_sp;
    } else if (instruction.left_register == r2) {
        code = code_ldr_r2_sp;
    } else {
        code = code_ldr_r3_sp;
    }
    return code;
}

uint32_t encode_str(arm_instruction_t instruction) {
    return code_str_r0_sp;
}

uint32_t encode_bx(arm_instruction_t instruction) {
    return (instruction.left_register == lr) ? code_bx_lr : code_bx_r5;
}

void translate_arm_to_byte(arm_instruction_t* arm_code, uint32_t* out_buffer) {
    int code_index = 0;
    int buffer_index = 0;
    
    while (arm_code[code_index].operation != END) {
        arm_instruction_t inst = arm_code[code_index];
        uint32_t code;
        if (inst.operation == push) {
            code = encode_push(inst);
            
        } else if (inst.operation == pop) {
            code = encode_pop(inst);
            
        } else if (inst.operation == sub) {
            code = encode_sub(inst);

        } else if (inst.operation == add) {
            code = encode_add(inst);
            
        } else if (inst.operation == mul) {
            code = encode_mul(inst);
            
        } else if (inst.operation == lsl) {
            code = encode_lsl(inst);
            
        } else if (inst.operation == mov) {
            code = encode_mov(inst);
            
        } else if (inst.operation == ldr) {
            code = encode_ldr(inst);
            
        } else if (inst.operation == str) {
            code = encode_str(inst);
            
        } else {
            code = encode_bx(inst);
            
        }
        ++code_index;
        append_byte_code(code, out_buffer, &buffer_index);
    }
}


void print_arm_code(arm_instruction_t* arm_code) {
    int code_index = 0;
    while (arm_code[code_index].operation != END) {
        arm_instruction_t inst = arm_code[code_index];
        if (inst.operation == push || inst.operation == pop) {
            printf("%s {%s, %s} \n", print_operation(inst.operation), print_register(inst.left_register), print_register(inst.right_register));
        } else if (inst.operation == add || inst.operation == sub || inst.operation == mul || inst.operation == mov || inst.operation == lsl) {
            if (inst.flag == WITH_CONST) {
                printf("%s %s , #%d \n", print_operation(inst.operation), print_register(inst.left_register), inst.constant);
            } else if (inst.flag == WITHOUT_CONST) {
                printf("%s %s , %s \n", print_operation(inst.operation), print_register(inst.left_register), print_register(inst.right_register));
            } else if (inst.flag == MOV_CONST) {
                printf("%s %s , #%d, #%d \n", print_operation(inst.operation), print_register(inst.left_register), inst.constant, inst.shift);
            }
        } else if (inst.operation == ldr || inst.operation == str) {
            printf("%s %s , [%s] \n", print_operation(inst.operation), print_register(inst.left_register), print_register(inst.right_register));
        } else if (inst.operation == bx) {
            printf("%s %s \n", print_operation(inst.operation), print_register(inst.left_register));
        }
        ++code_index;
    }
}

void
jit_compile_expression_to_arm(const char* expression,
                              const symbol_t* externs,
                              void* out_buffer) {
    size_t expression_len = strlen(expression) + 100;
    token_t* tokenized_expr = malloc(expression_len * sizeof(token_t));
    tokenize_expr(expression, tokenized_expr, externs);

    token_t* poliz = malloc(expression_len * sizeof(token_t));
    token_t* stack = malloc(expression_len * sizeof(token_t));

    make_poliz(poliz, tokenized_expr, stack);

    arm_instruction_t* arm_code = malloc(expression_len * 12 * sizeof(arm_instruction_t));
    out_buffer = (uint32_t*)out_buffer;

    transate_poliz_to_arm(poliz, arm_code);
    translate_arm_to_byte(arm_code, out_buffer);
    
    free(poliz);
    free(stack);
    free(arm_code);

}


