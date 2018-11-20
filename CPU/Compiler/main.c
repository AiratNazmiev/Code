#include <stdio.h>
#include <malloc.h>
#include <mem.h>
#include <assert.h>
#include "readtext/readtext.h"
#include "asm.h"

int *asmLabels(struct LinkedBuffer *asm_code, char *str_instr, char *str_arg, char *str);

static void asmTranslationError(unsigned int num);

int main(int argc, char *argv[]) {
    FILE *input = 0;

    if (argc == 3) {
        input = fopen(argv[1], "r");
    } else {
        asmTranslationError(ASM_OPEN_FILE_ERROR);
        return ASM_OPEN_FILE_ERROR;
    }

    struct LinkedBuffer asm_code = bufferLinking(input);

    fclose(input);

    char *byte_code = (char *) calloc(asm_code.numStr * StrMaxByteLen, sizeof(*byte_code));

    unsigned int pc = 0;

    char *str_instr = calloc(MaxStrLen, sizeof(*str_instr));
    char *str_arg = calloc(MaxArgLen, sizeof(*str_arg));
    char *str = calloc(MaxStrLen, sizeof(*str));

    int *labels = asmLabels(&asm_code, str_instr, str_arg, str);

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

        #define DEF_CMD(name, instr_enum, num, asm, asm_label, CPU) if (!strcmp(str_instr, #name)) {asm}
        #include "D:/Code/CLionProjects/asmCommands/commands.h"
        #undef DEF_CMD


        if (sscanf(asm_code.str_ptr[i], ":%d", &arg1) == 1) {
            if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                asmTranslationError(ASM_WRONG_LABEL);
            } else {
                labels[arg1] = pc;
            }
            continue;
        }

        if (!strcmp(asm_code.str_ptr[i], "")) {
            continue;
        } else {
            asmTranslationError(ASM_UNKNOWN_INSTRUCTION);
        }
    }

    FILE *output = fopen(argv[2], "wb");
    fwrite(byte_code, sizeof(*byte_code), pc, output);

    free(byte_code);
    free(str);
    free(str_arg);
    free(str_instr);
    free(labels);

    fclose(output);

    return 0;
}

static void asmTranslationError(unsigned int num) {
    switch (num) {
        case ASM_OPEN_FILE_ERROR:
            fprintf(stderr, "Translation error: cannot open input file");
            exit(ASM_OPEN_FILE_ERROR);
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
            fprintf(stderr, "Translation error: unknown error");
            exit(ASM_UNKNOWN_ERROR);
    }
}

int *asmLabels(struct LinkedBuffer *asm_code, char *str_instr, char *str_arg, char *str) {
    int *labels = (int *) calloc(LabelsNum, sizeof(*labels));

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

        #define DEF_CMD(name, instr_enum, num, asm, asm_label, CPU) if (!strcmp(str_instr, #name)) {asm_label}
        #include "D:/Code/CLionProjects/asmCommands/commands.h"
        #undef DEF_CMD


        if (sscanf(asm_code->str_ptr[i], ":%d", &arg1) == 1) {
            if ((arg1 < 0) || (arg1 >= LabelsNum)) {
                asmTranslationError(ASM_WRONG_LABEL);
            } else {
                labels[arg1] = pc;
            }
            continue;
        }

        if (!strcmp(asm_code->str_ptr[i], "")) {
            continue;
        } else {
            asmTranslationError(ASM_UNKNOWN_INSTRUCTION);
        }
    }

    return labels;
}