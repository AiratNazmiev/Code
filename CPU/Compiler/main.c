#include <stdio.h>
#include <malloc.h>
#include <mem.h>
#include <assert.h>
#include "readtext/readtext.h"
#include "stack/stack.h"
#include "asm.h"

struct Labels {
    int *lab;
    int *callLab;
};

struct Labels asmLabels(struct LinkedBuffer *asm_code, char *str_instr, char *str_arg, char *str);

static void asmTranslationError(unsigned int num);

int main() {
    FILE *input = fopen("code.txt", "r");

    struct LinkedBuffer asm_code = bufferLinking(input);

    fclose(input);

    char *byte_code = (char *) calloc(asm_code.numStr * StrMaxByteLen, sizeof(*byte_code));

    unsigned int pc = 0;

    char *str_instr = calloc(MaxStrLen, sizeof(*str_instr));
    char *str_arg = calloc(MaxArgLen, sizeof(*str_arg));
    char *str = calloc(MaxStrLen, sizeof(*str));

    struct Labels labels = asmLabels(&asm_code, str_instr, str_arg, str);

    for (int i = 0; i < asm_code.numStr; i++) {
        data_t arg1 = -1;
        memset(str_arg, 0, MaxArgLen);
        memset(str_instr, 0, MaxStrLen);
        memset(str, 0, MaxStrLen);

        if (asm_code.str_ptr[i] != NULL) {
            sscanf(asm_code.str_ptr[i], "%100s", str_instr);
        } else {
            break;
        }

        char *comment = 0;
        if ((comment = strstr(asm_code.str_ptr[i], ";")) != NULL) {
            *comment = 0;
        }

        if (!strcmp(str_instr, "push")) {

            if (sscanf(asm_code.str_ptr[i], "push %d", &arg1) == 1) { //5b
                byte_code[pc++] = ASM_PUSH_CONST;
                *(data_t *) (byte_code + pc) = arg1; //отличиче ((int *)name)[] от *(int *)(name + i)
                pc += sizeof(data_t);
                continue;
            }

            if (sscanf(asm_code.str_ptr[i], "push %s", str_arg) == 1) {

                if (!strcmp(str_arg, "ra")) {
                    byte_code[pc++] = ASM_PUSH_REG;
                    byte_code[pc++] = RA;
                    continue;
                }

                if (!strcmp(str_arg, "rb")) {
                    byte_code[pc++] = ASM_PUSH_REG;
                    byte_code[pc++] = RB;
                    continue;
                }

                if (!strcmp(str_arg, "rc")) {
                    byte_code[pc++] = ASM_PUSH_REG;
                    byte_code[pc++] = RC;
                    continue;
                }

                if (!strcmp(str_arg, "rd")) {
                    byte_code[pc++] = ASM_PUSH_REG;
                    byte_code[pc++] = RD;
                    continue;
                }

                if (sscanf(asm_code.str_ptr[i], "push [%d]", &arg1) == 1) { // добавить в стек элемент из RAM[arg1]
                    byte_code[pc++] = ASM_PUSH_RAM;
                    *(int *) (byte_code + pc) = arg1;
                    pc += sizeof(int);
                    continue;
                }
            }

            asmTranslationError(ASM_UNKNOWN_PUSH_ARG);
        }

        if (!strcmp(str_instr, "pop")) {

            if (sscanf(asm_code.str_ptr[i], "pop %s", str_arg) == 1) {

                if (!strcmp(str_arg, "ra")) {//5b
                    byte_code[pc++] = ASM_POP_REG;
                    byte_code[pc++] = RA;
                    continue;
                }

                if (!strcmp(str_arg, "rb")) {
                    byte_code[pc++] = ASM_POP_REG;
                    byte_code[pc++] = RB;
                    continue;
                }

                if (!strcmp(str_arg, "rc")) {
                    byte_code[pc++] = ASM_POP_REG;
                    byte_code[pc++] = RC;
                    continue;
                }

                if (!strcmp(str_arg, "rd")) {
                    byte_code[pc++] = ASM_POP_REG;
                    byte_code[pc++] = RD;
                    continue;
                }
            }

            if (sscanf(asm_code.str_ptr[i], "pop [%d]", &arg1) == 1) { // добавить в стек элемент из RAM[arg1]
                byte_code[pc++] = ASM_POP_RAM;
                *(int *) (byte_code + pc) = arg1;
                pc += sizeof(int);
                continue;
            }

            if (!strcmp(str_arg, "")) {
                byte_code[pc++] = ASM_POP_DEL;
                continue;
            } else {
                asmTranslationError(ASM_UNKNOWN_POP_ARG);
            }

        }

        if (!strcmp(str_instr, "add")) {
            byte_code[pc++] = ASM_ADD;
            continue;
        }

        if (!strcmp(str_instr, "sub")) {
            byte_code[pc++] = ASM_SUB;
            continue;
        }

        if (!strcmp(str_instr, "mul")) {
            byte_code[pc++] = ASM_MUL;
            continue;
        }

        if (!strcmp(str_instr, "div")) {
            byte_code[pc++] = ASM_DIV;
            continue;
        }

        if (!strcmp(str_instr, "in")) {
            byte_code[pc++] = ASM_IN;
            continue;
        }

        if (!strcmp(str_instr, "out")) {
            byte_code[pc++] = ASM_OUT;
            continue;
        }

        if (!strcmp(str_instr, "jmp")) {

            if (sscanf(asm_code.str_ptr[i], "jmp :%d", &arg1) == 1) {

                if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                    asmTranslationError(ASM_WRONG_LABEL);
                }

                byte_code[pc++] = ASM_JMP;
                *(int *) (byte_code + pc) = labels.lab[arg1];
                pc += sizeof(int);
                continue;
            } else {
                asmTranslationError(ASM_WRONG_JMP_ARG);
            }
        }

        if (!strcmp(str_instr, "call")) {

            if (sscanf(asm_code.str_ptr[i], "call :%d", &arg1) == 1) {
                byte_code[pc++] = ASM_CALL;
                *(int *) (byte_code + pc) = labels.lab[arg1];
                pc += sizeof(int);
                labels.callLab[arg1] = pc;
                continue;
            } else {
                asmTranslationError(ASM_WRONG_CALL_ARG);
            }
        }

        if (sscanf(asm_code.str_ptr[i], "ret :%d", &arg1) == 1) {//Do call analysis
            byte_code[pc++] = ASM_RET;
            *(int *) (byte_code + pc) = labels.callLab[arg1];//!!!!!!!!!!!!!!!!!!!!!
            pc += sizeof(int);
            continue;
        }

        if (!strcmp(str_instr, "ja")) {
            if (sscanf(asm_code.str_ptr[i], "ja :%d", &arg1) == 1) {
                byte_code[pc++] = ASM_JA;
                *(int *) (byte_code + pc) = labels.lab[arg1];
                pc += sizeof(int);
                continue;
            } else {
                asmTranslationError(ASM_WRONG_JA_ARG);
            }
        }

        if (!strcmp(str_instr, "jb")) {
            if (sscanf(asm_code.str_ptr[i], "jb :%d", &arg1) == 1) {
                byte_code[pc++] = ASM_JB;
                *(int *) (byte_code + pc) = labels.lab[arg1];
                pc += sizeof(int);
                continue;
            } else {
                asmTranslationError(ASM_WRONG_JB_ARG);
            }
        }

        if (!strcmp(str_instr, "je")) {
            if (sscanf(asm_code.str_ptr[i], "je :%d", &arg1) == 1) {
                byte_code[pc++] = ASM_JE;
                *(int *) (byte_code + pc) = labels.lab[arg1];
                pc += sizeof(int);
                continue;
            } else {
                asmTranslationError(ASM_WRONG_JE_ARG);
            }
        }

        if (sscanf(asm_code.str_ptr[i], ":%d", &arg1) == 1) {
            if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                asmTranslationError(ASM_WRONG_LABEL);
            } else {
                labels.lab[arg1] = pc;
            }
            continue;
        }

        if (!strcmp(str_instr, "end")) {
            byte_code[pc++] = ASM_END;
            continue; ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        if (!strcmp(asm_code.str_ptr[i], "")) {
            continue;
        } else {
            asmTranslationError(ASM_UNKNOWN_INSTRUCTION);
        }
    }

    FILE *output = fopen("bytecode.txt", "wb");
    fwrite(byte_code, sizeof(*byte_code), pc, output);

    free(byte_code);
    free(str);
    free(str_arg);
    free(str_instr);
    free(labels.lab);
    free(labels.callLab);

    fclose(output);

    printf("%X", byte_code);

    return 0;
}

static void asmTranslationError(unsigned int num) {
    switch (num) {
        case ASM_UNKNOWN_INSTRUCTION:
            fprintf(stderr, "Translation error: unknown instruction");
            exit(ASM_UNKNOWN_INSTRUCTION);
        case ASM_UNKNOWN_POP_ARG:
            fprintf(stderr, "Translation error: unknown pop argument");
            exit(ASM_UNKNOWN_POP_ARG);
        case ASM_UNKNOWN_PUSH_ARG:
            fprintf(stderr, "Translation error: unknown push argument");
            exit(ASM_UNKNOWN_PUSH_ARG);
        case ASM_WRONG_LABEL:
            fprintf(stderr, "Translation error: wrong label");
            exit(ASM_WRONG_LABEL);
        case ASM_WRONG_JMP_ARG:
            fprintf(stderr, "Translation error: wrong jmp argument");
            exit(ASM_WRONG_JMP_ARG);
        case ASM_WRONG_CALL_ARG:
            fprintf(stderr, "Translation error: wrong call argument");
            exit(ASM_WRONG_CALL_ARG);
        case ASM_WRONG_JA_ARG:
            fprintf(stderr, "Translation error: wrong ja argument");
            exit(ASM_WRONG_JA_ARG);
        case ASM_WRONG_JB_ARG:
            fprintf(stderr, "Translation error: wrong jb argument");
            exit(ASM_WRONG_JB_ARG);
        case ASM_WRONG_JE_ARG:
            fprintf(stderr, "Translation error: wrong je argument");
            exit(ASM_WRONG_JE_ARG);
        default:
            fprintf(stderr, "Translation error: unknown instruction");
            exit(ASM_UNKNOWN_ERROR);
    }
}

struct Labels asmLabels(struct LinkedBuffer *asm_code, char *str_instr, char *str_arg, char *str) {
    int *labels = (int *) calloc(LabelsNum, sizeof(*labels));
    int *callLabels = (int *) calloc(LabelsNum, sizeof(*callLabels));

    unsigned int pc = 0;

    for (int i = 0; i < asm_code->numStr; i++) {
        data_t arg1 = -1;
        memset(str_arg, 0, MaxArgLen);
        memset(str_instr, 0, MaxStrLen);
        memset(str, 0, MaxStrLen);

        if (asm_code->str_ptr[i] != NULL) {
            sscanf(asm_code->str_ptr[i], "%100s", str_instr);
        } else {
            break;
        }

        char *comment = 0;
        if ((comment = strstr(asm_code->str_ptr[i], ";")) != NULL) {
            *comment = 0;
        }

        if (!strcmp(str_instr, "push")) {

            if (sscanf(asm_code->str_ptr[i], "push %d", &arg1) == 1) { //5b
                pc += (sizeof(data_t) + 1);
                continue;
            }

            if (sscanf(asm_code->str_ptr[i], "push %s", str_arg) == 1) {

                if (!strcmp(str_arg, "ra")) {
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rb")) {
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rc")) {
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rd")) {
                    pc += 2;
                    continue;
                }

                if (sscanf(asm_code->str_ptr[i], "push [%d]", &arg1) == 1) {
                    pc += (sizeof(int) + 1);
                    continue;
                }
            }

            asmTranslationError(ASM_UNKNOWN_PUSH_ARG);
        }

        if (!strcmp(str_instr, "pop")) {

            if (sscanf(asm_code->str_ptr[i], "pop %s", str_arg) == 1) {

                if (!strcmp(str_arg, "ra")) {//5b
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rb")) {
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rc")) {
                    pc += 2;
                    continue;
                }

                if (!strcmp(str_arg, "rd")) {
                    pc += 2;
                    continue;
                }
            }

            if (sscanf(asm_code->str_ptr[i], "pop [%d]", &arg1) == 1) { // добавить в стек элемент из RAM[arg1]
                pc += (sizeof(int) + 1);
                continue;
            }

            if (!strcmp(str_arg, "")) {
                pc++;
                continue;
            } else {
                asmTranslationError(ASM_UNKNOWN_POP_ARG);
            }

        }

        if (!strcmp(str_instr, "add")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "sub")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "mul")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "div")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "in")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "out")) {
            pc++;
            continue;
        }

        if (!strcmp(str_instr, "jmp")) {

            if (sscanf(asm_code->str_ptr[i], "jmp :%d", &arg1)) {

                if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                    asmTranslationError(ASM_WRONG_LABEL);
                }

                pc += (sizeof(int) + 1);
                continue;
            }

        }

        if (!strcmp(str_instr, "call")) {
            pc += (sizeof(int) + 1);
            continue;
        }

        if (!strcmp(str_instr, "ret")) {
            pc += (sizeof(int) + 1);
            continue;
        }

        if (!strcmp(str_instr, "ja")) {
            pc += (sizeof(int) + 1);
            continue;
        }

        if (!strcmp(str_instr, "jb")) {
            pc += (sizeof(int) + 1);
            continue;
        }

        if (!strcmp(str_instr, "je")) {
            pc += (sizeof(int) + 1);
            continue;
        }

        if (sscanf(asm_code->str_ptr[i], ":%d", &arg1) == 1) {
            if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                asmTranslationError(ASM_WRONG_LABEL);
            } else {
                labels[arg1] = pc;
            }
            continue;
        }

        if (!strcmp(str_instr, "end")) {
            pc++;
            continue; /////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        if (!strcmp(asm_code->str_ptr[i], "")) {
            continue;
        } else {
            asmTranslationError(ASM_UNKNOWN_INSTRUCTION);
        }
    }
    struct Labels lab = {labels, callLabels};

    return lab;
}