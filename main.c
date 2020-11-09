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
    char* operation;
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
                token->name = (char*)malloc(strlen(externs[i].name) * sizeof(char));
                strcpy(token->name, externs[i].name);
            }
        }
        ++i;
    }
}

void process_string(char* current_string,
                    size_t expression_len,
                    token_types_e* current_token_type,
                    token_t* tokenized_str,
                    int index,
                    const symbol_t* externs) {
    if ((*current_token_type) == CONSTANT) {
        tokenized_str[index].value = atoi(current_string);
    } else if ((*current_token_type) == VARIABLE || (*current_token_type) == FUNCTION) {
        dereference_value(&tokenized_str[index], current_string, *current_token_type, externs);
    } else if ((*current_token_type) == OPERATION) {
        tokenized_str[index].operation = (char*)malloc(strlen(current_string) * sizeof(char));
        strcpy(tokenized_str[index].operation, current_string);
    } else if ((*current_token_type) == SPECIAL_SYMBOL) {
        tokenized_str[index].special_symbol = current_string[0];
    }

    tokenized_str[index].type = (*current_token_type);

    (*current_token_type) = UNDEFINED;
    current_string[0] = '\0';
}

void tokenize_expr (const char* expresion, token_t* tokenized_expr, const symbol_t* externs) {
    int tokenized_str_index = 0;
    size_t expression_len = strlen(expresion);

    token_types_e current_token_type = UNDEFINED;

    char* current_string = (char*)malloc(expression_len * sizeof(char));
    int string_index = 0;

    for (int i = 0; i < expression_len; ++i) {

        if (expresion[i] >= '0' && expresion[i] <= '9') {
            if (current_token_type == UNDEFINED) {
                current_token_type = CONSTANT;
            } else if (current_token_type != CONSTANT && current_token_type != VARIABLE) {
                current_string[string_index] = '\0';
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                string_index = 0;
                ++tokenized_str_index;
                current_token_type = CONSTANT;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if ((expresion[i] >= 'a' && expresion[i] <= 'z') || (expresion[i] >= 'A' && expresion[i] <= 'Z')) {
            if (current_token_type == UNDEFINED) {
                current_token_type = VARIABLE;
            } else if (current_token_type != VARIABLE) {
                current_string[string_index] = '\0';
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                string_index = 0;
                ++tokenized_str_index;
                current_token_type = VARIABLE;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if (expresion[i] == '+' || expresion[i] == '-' || expresion[i] == '*') {
            if (expresion[i] == '-') {
                if (current_string[string_index - 1] == ',' || current_string[string_index - 1] == '(') {
                    ++string_index;
                    current_string[string_index] = '\0';
                    process_string(current_string, expression_len, &current_token_type,
                                   tokenized_expr, tokenized_str_index, externs);
                    ++tokenized_str_index;
                    string_index = 0;
                    current_token_type = UNDEFINED;
                }

                if (current_token_type == UNDEFINED) {
                    current_string[string_index] = '0';
                    ++string_index;
                    current_token_type = CONSTANT;
                    current_string[string_index] = '\0';
                    process_string(current_string, expression_len, &current_token_type,
                                   tokenized_expr, tokenized_str_index, externs);
                    ++tokenized_str_index;
                    string_index = 0;
                    current_token_type = UNDEFINED;
                }
            }
            if (current_token_type == UNDEFINED) {
                current_token_type = OPERATION;
            }
            if (current_token_type != OPERATION) {
                current_string[string_index] = '\0';
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                ++tokenized_str_index;
                string_index = 0;
                current_token_type = OPERATION;
            }
            current_string[string_index] = expresion[i];
            ++string_index;

        } else if (expresion[i] == '(' || expresion[i] == ')' || expresion[i] == ',') {
            if (current_token_type == SPECIAL_SYMBOL) {
                current_string[string_index] = '\0';
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                ++tokenized_str_index;
                string_index = 0;
                current_token_type = UNDEFINED;
            }

            if (current_token_type == UNDEFINED) {
                current_token_type = SPECIAL_SYMBOL;
            }
            if (expresion[i] == '(' && current_token_type == VARIABLE) {
                current_string[string_index] = '\0';
                current_token_type = FUNCTION;
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                ++tokenized_str_index;
                string_index = 0;
                current_token_type = SPECIAL_SYMBOL;
            } else if (current_token_type != SPECIAL_SYMBOL) {
                current_string[string_index] = '\0';
                process_string(current_string, expression_len, &current_token_type,
                               tokenized_expr, tokenized_str_index, externs);
                ++tokenized_str_index;
                string_index = 0;
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
        current_string[string_index] = '\0';
        process_string(current_string, expression_len, &current_token_type,
                       tokenized_expr, tokenized_str_index, externs);
        ++tokenized_str_index;
    }

    tokenized_expr[tokenized_str_index].type = END_OF_EXPRESSION;
    free(current_string);
}

void tokenized_expr_to_str (char** tokenized_str, token_t* tokenized_expr) {
    int cnt = 0;
    for (size_t i = 0; tokenized_expr[i].type != END_OF_EXPRESSION; ++i) {
        tokenized_str[i] = (char*)malloc(100 * sizeof(char));
        if (tokenized_expr[i].type == CONSTANT || tokenized_expr[i].type == VARIABLE) {
            sprintf(tokenized_str[i], "%d", tokenized_expr[i].value);
        } else if (tokenized_expr[i].type == FUNCTION) {
            strcpy(tokenized_str[i], tokenized_expr[i].name);
        } else if (tokenized_expr[i].type == OPERATION) {
            strcpy(tokenized_str[i], tokenized_expr[i].operation);
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
        if (!strcmp(token.operation, "+") || !strcmp(token.operation, "-")) {
            return 1;
        }
        if (!strcmp(token.operation, "*")) {
            return 2;
        }
    } else if (token.type == FUNCTION) {
        return 3;
    }
    return 4;
};

void make_poliz (token_t* poliz, const token_t* tokenized_expr, token_t* stack) {
    int i = 0;

    int poliz_index = -1;
    int stack_index = -1;

    int* valence_stack = (int*)calloc(100, sizeof(int));
    int valence_index = -1;

    while (tokenized_expr[i].type != END_OF_EXPRESSION) {
        if (tokenized_expr[i].type == CONSTANT || tokenized_expr[i].type == VARIABLE) {
            ++poliz_index;
            poliz[poliz_index].value = tokenized_expr[i].value;
            poliz[poliz_index].type = CONSTANT;
        } else if (tokenized_expr[i].type == SPECIAL_SYMBOL) {
            if (tokenized_expr[i].special_symbol == '(') {
                ++valence_index;

                ++stack_index;
                stack[stack_index].special_symbol = '(';
                stack[stack_index].type = SPECIAL_SYMBOL;
            } else if (tokenized_expr[i].special_symbol == ',' || tokenized_expr[i].special_symbol == ')') {
                while (stack[stack_index].type != SPECIAL_SYMBOL)  {
                    ++poliz_index;

                    if (stack[stack_index].type == OPERATION) {
                        poliz[poliz_index].operation = (char*)malloc(strlen(stack[stack_index].operation) * sizeof(char));
                        strcpy(poliz[poliz_index].operation, stack[stack_index].operation);
                        poliz[poliz_index].type = OPERATION;
                    } else if (stack[stack_index].type == FUNCTION) {
                        poliz[poliz_index].name = (char *) malloc(strlen(stack[stack_index].name) * sizeof(char));
                        strcpy(poliz[poliz_index].name, stack[stack_index].name);
                        poliz[poliz_index].type = FUNCTION;
                        poliz[poliz_index].function_ptr = stack[stack_index].function_ptr;
                        poliz[poliz_index].function_valence = stack[stack_index].function_valence;
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
            while (stack_index >= 0 && (stack[stack_index].type == OPERATION || stack[stack_index].type == FUNCTION) &&
                   define_operation_priority(stack[stack_index]) >= define_operation_priority(tokenized_expr[i])) {
                ++poliz_index;

                if (stack[stack_index].type == OPERATION) {
                    poliz[poliz_index].operation = (char*)malloc(strlen(stack[stack_index].operation) * sizeof(char));
                    strcpy(poliz[poliz_index].operation, stack[stack_index].operation);
                    poliz[poliz_index].type = OPERATION;
                } else if (stack[stack_index].type == FUNCTION) {
                    poliz[poliz_index].name = (char *) malloc(strlen(stack[stack_index].name) * sizeof(char));
                    strcpy(poliz[poliz_index].name, stack[stack_index].name);
                    poliz[poliz_index].type = FUNCTION;
                    poliz[poliz_index].function_ptr = stack[stack_index].function_ptr;
                    poliz[poliz_index].function_valence = stack[stack_index].function_valence;
                }
                --stack_index;
            }
            ++stack_index;
            stack[stack_index].operation = (char*)malloc(strlen(tokenized_expr[i].operation) * sizeof(char));
            strcpy(stack[stack_index].operation, tokenized_expr[i].operation);
            stack[stack_index].type = OPERATION;

        } else if (tokenized_expr[i].type == FUNCTION) {
            ++stack_index;
            stack[stack_index].name = (char*)malloc(strlen(tokenized_expr[i].name) * sizeof(char));
            strcpy(stack[stack_index].name, tokenized_expr[i].name);
            stack[stack_index].type = FUNCTION;
            stack[stack_index].function_ptr = tokenized_expr[i].function_ptr;
        }
        ++i;
    }

    while (stack_index >= 0) {
        ++poliz_index;
        if (stack[stack_index].type == OPERATION) {
            poliz[poliz_index].operation = (char*)malloc(strlen(stack[stack_index].operation) * sizeof(char));
            strcpy(poliz[poliz_index].operation, stack[stack_index].operation);
            poliz[poliz_index].type = OPERATION;
        } else if (stack[stack_index].type == FUNCTION) {
            poliz[poliz_index].name = (char*)malloc(strlen(stack[stack_index].name) * sizeof(char));
            strcpy(poliz[poliz_index].name, stack[stack_index].name);
            poliz[poliz_index].type = FUNCTION;
            poliz[poliz_index].function_ptr = stack[stack_index].function_ptr;
            poliz[poliz_index].function_valence = stack[stack_index].function_valence;
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
            printf("%s ", poliz[i].operation);
        } else if (poliz[i].type == SPECIAL_SYMBOL) {
            printf("%c", poliz[i].special_symbol);
        }
    }
}

typedef enum {
    mov = 0xe3a00000, // code for r0, #0
    sub = 0xe24dd004, // code for sp -= 4
    add = 0xe28dd004, // code for sp += 4
    mul = 0xe0000091, // code for ro *= r1
    ldr = 0xe59d0000, // code for ldr r0, [sp]
    str = 0xe58d0000, // code for str r0, [sp]
    lsl = 0xe1a01401, // code for lsl r1, #8
    push = 0xe92d4020, // code for push {r5, lr}
    pop = 0xe8bd4020, // code for pop {r5, lr}
    bx = 0xe12fff1e, // code for bx lr
    END
} arm_operations_e;

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

arm_instruction_t build_instruction(arm_operations_e operation,
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
    return instruction;
}

void mov_const(arm_instruction_t* arm_code, int* code_index, int value, arm_registers_e reg, uint32_t* out_buffer, int* buffer_index) {
    uint32_t first_byte = (value & 0xFF);
    uint32_t second_byte = (value & 0xFF00) >> 8;
    uint32_t third_byte = (value & 0xFF0000) >> 16;
    uint32_t fourth_byte = (value & 0xFF000000) >> 24;

    uint32_t shift_for_lsl = 0x400;
    uint32_t shift_for_str = 0x5000;
    uint32_t shift_for_add = 0x55000;
    uint32_t shift_for_mov = 0x5000;
    uint32_t code_for_add = 0xe0800001;
    uint32_t code_for_mov_r1 = 0x1000;
    
    if (reg != r5) {
        arm_code[(*code_index)] = build_instruction(sub, sp, nan, 4, WITH_CONST);
        out_buffer[(*buffer_index)] = sub;
        ++(*buffer_index);
        ++(*code_index);
    }

    arm_code[(*code_index)] = build_instruction(mov, reg, nan, first_byte, WITH_CONST);
    out_buffer[(*buffer_index)] = ((reg == r5) ? mov + shift_for_mov : mov) + first_byte;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(mov, r1, nan, second_byte, WITH_CONST);
    out_buffer[(*buffer_index)] =  mov + code_for_mov_r1 + second_byte;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(lsl, r1, nan, 8, WITH_CONST);
    out_buffer[(*buffer_index)] = lsl;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(add, reg, r1, -1, WITHOUT_CONST);
    out_buffer[(*buffer_index)] = (reg == r5) ? code_for_add + shift_for_add : code_for_add;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(mov, r1, nan, third_byte, WITH_CONST);
    out_buffer[(*buffer_index)] = mov + code_for_mov_r1 + third_byte;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(lsl, r1, nan, 16, WITH_CONST);
    out_buffer[(*buffer_index)] = lsl + shift_for_lsl;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(add, reg, r1, -1, WITHOUT_CONST);
    out_buffer[(*buffer_index)] = (reg == r5) ? code_for_add + shift_for_add : code_for_add;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(mov, r1, nan, fourth_byte, WITH_CONST);
    out_buffer[(*buffer_index)] = mov + code_for_mov_r1 + fourth_byte;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(lsl, r1, nan, 24, WITH_CONST);
    out_buffer[(*buffer_index)] = lsl + shift_for_lsl + shift_for_lsl;
    ++(*buffer_index);
    ++(*code_index);

    arm_code[(*code_index)] = build_instruction(add, reg, r1, -1, WITHOUT_CONST);
    out_buffer[(*buffer_index)] = (reg == r5) ? code_for_add + shift_for_add : code_for_add;
    ++(*buffer_index);
    ++(*code_index);

    if (reg != r5) {
        arm_code[(*code_index)] = build_instruction(str, reg, sp, -1, WITHOUT_CONST);
        out_buffer[(*buffer_index)] =  str;
        ++(*buffer_index);
        ++(*code_index);
    }
}

void transate_poliz_to_arm(token_t* poliz, arm_instruction_t* arm_code, uint32_t * out_buffer) {
    int poliz_index = 0;
    int code_index = 0;
    int buffer_index = 0;


    arm_code[code_index] = build_instruction(push, r5, lr, -1, WITHOUT_CONST);
    out_buffer[(buffer_index)] = push;
    ++(buffer_index);
    ++code_index;
    
    uint32_t shift_for_ldr_r1_sp = 0x1000;
    uint32_t shift_for_ldr_r2_sp = 0x2000;
    uint32_t shift_for_ldr_r3_sp = 0x3000;


    while (poliz[poliz_index].type != END_OF_EXPRESSION) {
        if (poliz[poliz_index].type == CONSTANT) {
            mov_const(arm_code, &code_index, poliz[poliz_index].value, r0, out_buffer, &buffer_index);

        } else if (poliz[poliz_index].type == OPERATION) {
            arm_code[code_index] = build_instruction(ldr, r1, sp, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = ldr + shift_for_ldr_r1_sp;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
            out_buffer[(buffer_index)] = add;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(ldr, r0, sp, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = ldr;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
            out_buffer[(buffer_index)] = add;
            ++(buffer_index);
            ++code_index;

            arm_operations_e oper;
            uint32_t operation_code;
            if (!strcmp(poliz[poliz_index].operation, "+")) {
                oper = add;
                operation_code = 0xe0800001;
            } else if (!strcmp(poliz[poliz_index].operation, "-")) {
                oper = sub;
                operation_code = 0xe0400001;
            } else {
                oper = mul;
                operation_code = mul;
            }

            arm_code[code_index] = build_instruction(oper, r0, r1, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = operation_code;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(sub, sp, nan, 4, WITH_CONST);
            out_buffer[(buffer_index)] = sub;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(str, r0, sp, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = str;
            ++(buffer_index);
            ++code_index;
        } else if (poliz[poliz_index].type == FUNCTION) {
            mov_const(arm_code, &code_index, (uint32_t)(poliz[poliz_index].function_ptr), r5, out_buffer, &buffer_index);
            if (poliz[poliz_index].function_valence > 3) {
                arm_code[code_index] = build_instruction(ldr,r3, sp, -1, WITHOUT_CONST);
                out_buffer[(buffer_index)] = ldr + shift_for_ldr_r3_sp;
                ++(buffer_index);
                ++code_index;

                arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
                out_buffer[(buffer_index)] = add;
                ++(buffer_index);
                ++code_index;
            }
            if (poliz[poliz_index].function_valence > 2) {
                arm_code[code_index] = build_instruction(ldr,r2, sp, -1, WITHOUT_CONST);
                out_buffer[(buffer_index)] = ldr + shift_for_ldr_r2_sp;
                ++(buffer_index);
                ++code_index;

                arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
                out_buffer[(buffer_index)] = add;
                ++(buffer_index);
                ++code_index;
            }
            if (poliz[poliz_index].function_valence > 1) {
                arm_code[code_index] = build_instruction(ldr,r1, sp, -1, WITHOUT_CONST);
                out_buffer[(buffer_index)] = ldr + shift_for_ldr_r1_sp;
                ++(buffer_index);
                ++code_index;

                arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
                out_buffer[(buffer_index)] = add;
                ++(buffer_index);
                ++code_index;
            }
            if (poliz[poliz_index].function_valence > 0) {
                arm_code[code_index] = build_instruction(ldr,r0, sp, -1, WITHOUT_CONST);
                out_buffer[(buffer_index)] = ldr;
                ++(buffer_index);
                ++code_index;

                arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
                out_buffer[(buffer_index)] = add;
                ++(buffer_index);
                ++code_index;
            }
            uint32_t code_for_mov_lr_pc = 0xe1a0e00f;
            arm_code[code_index] = build_instruction(mov, lr, pc, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = code_for_mov_lr_pc;
            ++(buffer_index);
            ++code_index;

            uint32_t code_for_add_lr_4 = 0xe28ee004;
            arm_code[code_index] = build_instruction(add, lr, nan, 4, WITH_CONST);
            out_buffer[(buffer_index)] = code_for_add_lr_4;
            ++(buffer_index);
            ++code_index;

            uint32_t code_for_bx_r5 = 0xe12fff15;
            arm_code[code_index] = build_instruction(bx, r5, nan, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = code_for_bx_r5;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(sub, sp, nan, 4, WITH_CONST);
            out_buffer[(buffer_index)] = sub;
            ++(buffer_index);
            ++code_index;

            arm_code[code_index] = build_instruction(str, r0, sp, -1, WITHOUT_CONST);
            out_buffer[(buffer_index)] = str;
            ++(buffer_index);
            ++code_index;

        }
        ++poliz_index;
    }
    arm_code[code_index] = build_instruction(ldr, r0, sp, -1, WITHOUT_CONST);
    out_buffer[(buffer_index)] = ldr;
    ++(buffer_index);
    ++code_index;

    arm_code[code_index] = build_instruction(add, sp, nan, 4, WITH_CONST);
    out_buffer[(buffer_index)] = add;
    ++(buffer_index);
    ++code_index;

    arm_code[code_index] = build_instruction(pop, r5, lr, -1, WITHOUT_CONST);
    out_buffer[(buffer_index)] = pop;
    ++(buffer_index);
    ++code_index;

    arm_code[code_index] = build_instruction(bx, lr, nan, -1, WITHOUT_CONST);
    out_buffer[(buffer_index)] = bx;
    ++(buffer_index);
    ++code_index;

    arm_code[code_index] = build_instruction(END, nan, nan, -1, WITHOUT_CONST);
    ++code_index;
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
    token_t* tokenized_expr = (token_t*)malloc(expression_len * sizeof(token_t));
    tokenize_expr(expression, tokenized_expr, externs);

    token_t* poliz = (token_t*)malloc(expression_len * sizeof(token_t));
    token_t* stack = (token_t*)malloc(expression_len * sizeof(token_t));

    make_poliz(poliz, tokenized_expr, stack);

    arm_instruction_t* arm_code = (arm_instruction_t*)malloc(expression_len * 12 * sizeof(arm_instruction_t));
    out_buffer = (uint32_t*)out_buffer;

    transate_poliz_to_arm(poliz, arm_code, out_buffer);

}
