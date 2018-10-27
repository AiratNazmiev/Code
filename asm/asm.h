#ifndef ASM_H
#define ASM_H

typedef int data_t;

const unsigned int StrMaxByteLen = 5;
const unsigned int MaxStrLen     = 100;
const unsigned int MaxArgLen     = 100;
const unsigned int LabelsNum     = 16;

enum translationError {
    ASM_UNKNOWN_ERROR       = -1,
    ASM_UNKNOWN_INSTRUCTION = -2,
    ASM_UNKNOWN_PUSH_ARG    = -3,
    ASM_UNKNOWN_POP_ARG     = -4,
    ASM_WRONG_LABEL         = -5,
    ASM_WRONG_JMP_ARG       = -6,
    ASM_WRONG_CALL_ARG      = -7
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
    ASM_JA         = 15,//>
    ASM_JB         = 16,//<
    ASM_JE         = 17,//==
    ASM_END        = 18
};

enum registerNum {
    RA = 1,
    RB = 2,
    RC = 3,
    RD = 4
};

#endif //ASM_H