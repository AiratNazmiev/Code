#ifndef ASM_H
#define ASM_H

#define CPU_EXTRA_COMMANDS

//typedef int data_t; defined in stack.h

const unsigned int StrMaxByteLen = 5;
const unsigned int MaxStrLen     = 100;
const unsigned int MaxArgLen     = 100;
const unsigned int LabelsNum     = 16;

enum translationError {
    ASM_OPEN_FILE_ERROR     = -1,
    ASM_UNKNOWN_ERROR       = -2,
    ASM_UNKNOWN_INSTRUCTION = -3,
    ASM_UNKNOWN_PUSH_ARG    = -4,
    ASM_UNKNOWN_POP_ARG     = -5,
    ASM_WRONG_LABEL         = -6,
    ASM_WRONG_JMP_ARG       = -7,
    ASM_WRONG_CALL_ARG      = -8,
    ASM_WRONG_JA_ARG        = -9,
    ASM_WRONG_JB_ARG        = -10,
    ASM_WRONG_JE_ARG        = -11
};

enum instructions {
    ASM_PUSH_CONST = 1,
    ASM_PUSH_REG   = 2,
    ASM_PUSH_RAM   = 3,
    ASM_POP_REG    = 4,
    ASM_POP_RAM    = 5,
    ASM_POP_DEL    = 6, //delete //mb do popd instead of pop
    ASM_ADD        = 7,
    ASM_SUB        = 8,
    ASM_MUL        = 9,
    ASM_DIV        = 10,
    ASM_IN         = 11,
    ASM_OUT        = 12,
    ASM_JMP        = 13,
    ASM_CALL       = 14,
    ASM_RET        = 15,
    ASM_JA         = 16,//>
    ASM_JB         = 17,//<
    ASM_JE         = 18,//==
    ASM_END        = 19
#ifdef CPU_EXTRA_COMMANDS
    ,ASM_INT_SQRT  = 20
#endif //CPU_EXTRA_COMMANDS
};

enum registerNum {
    RA = 1,
    RB = 2,
    RC = 3,
    RD = 4
};

#endif //ASM_H