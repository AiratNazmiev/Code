#ifndef CPU_CPU_H
#define CPU_CPU_H

#include <stdio.h>

typedef int data_t;

#define POP StackPop(CPU.stack) //used in defines
#define PUSH(num) StackPush(CPU.stack, num)
#define PEEK StackPeek(CPU.stack)


#define CPU_ASSERT(check){\
    int res = check;\
    if ((res)){\
    fprintf(stderr, "Function %s: %s in File %s, line %d\n",__FUNCTION__ , #check, __FILE__, __LINE__);\
    }\
    switch (res){\
        case CPU_OK: break;\
        case CPU_NULLPTR: fprintf(stderr, "CPU error: CPU nullptr");\
        case CPU_STACK_NULLPTR: fprintf(stderr, "CPU error: CPU stack nullptr");\
        case CPU_CODE_NULLPTR: fprintf(stderr, "CPU error: CPU bytecode nullptr");\
        case CPU_RAM_NULLPTR: fprintf(stderr, "CPU error: CPU ram nullptr");\
        case CPU_REG_NULLPTR: fprintf(stderr, "CPU error: CPU registers nullptr");\
        case CPU_DAMAGED_PC: fprintf(stderr, "CPU error: CPU pc is damaged");\
        case CPU_DAMAGED_CODESIZE: fprintf(stderr, "CPU error: CPU stack nullptr");\
        default : fprintf(stderr, "CPU error: CPU unknown error");\
    }\
}\

struct CPU {
    struct Stack *stack;
    unsigned int pc;
    unsigned int codeSize;
    char *byte_code;
    data_t *registers;
    data_t *ram;
};



enum CPUError {
    CPU_OK,
    CPU_NULLPTR,
    CPU_STACK_NULLPTR,
    CPU_CODE_NULLPTR,
    CPU_RAM_NULLPTR,
    CPU_REG_NULLPTR,
    CPU_DAMAGED_PC,
    CPU_DAMAGED_CODESIZE,
    CPU_WRONG_FILE_NAME,
    CPU_OPEN_FILE_ERROR,
    CPU_UNKNOWN_INSTRUCTION,
    CPU_UNKNOWN_ERROR
};

#define DEF_CMD(name, instr_enum, num, asm, asm_label, processing) ASM_##instr_enum = num,
enum commands{
#include "D:/Code/CLionProjects/asmCommands/commands.h"
    ASM_MAX
};
#undef DEF_CMD

//enum instructions {
//    ASM_PUSH_CONST = 1,
//    ASM_PUSH_REG   = 2,
//    ASM_PUSH_RAM   = 3,
//    ASM_POP_REG    = 4,
//    ASM_POP_RAM    = 5,
//    ASM_POP_DEL    = 6,
//    ASM_ADD        = 7,
//    ASM_SUB        = 8,
//    ASM_MUL        = 9,
//    ASM_DIV        = 10,
//    ASM_IN         = 11,
//    ASM_OUT        = 12,
//    ASM_JMP        = 13,
//    ASM_CALL       = 14,
//    ASM_RET        = 15,
//    ASM_JA         = 16,//>
//    ASM_JB         = 17,//<
//    ASM_JE         = 18,//==
//    ASM_END        = 19,
//    ASM_INT_SQRT  = 20
//};

char *readCode(char *filename, unsigned int *size);

void CPUCtor(struct CPU *CPU, char *input);

void CPUDtor(struct CPU *CPU);

int CPUOk(struct CPU *CPU);

void CPUDump(FILE* file, struct CPU *CPU);

int CPUerror(unsigned int num);


#endif //CPU_CPU_H
