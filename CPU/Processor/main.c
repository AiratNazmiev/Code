#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <process.h>
#include "stack/stack.h"
#include "asm.h"

const unsigned int RAMLen = 16;
const unsigned int RegisterNum = 4;

struct CPU {
    struct Stack *stack;
    unsigned int pc;
    unsigned int codeSize;
    char *byte_code;
    data_t *registers;
    data_t *ram;
};

char *readCode(char *filename, unsigned int *size);

void CPUCtor(struct CPU *CPU);

void CPUDtor(struct CPU *CPU);

void pushConst(struct CPU *CPU);

void pushRegister(struct CPU *CPU);

void pushRAM(struct CPU *CPU);

void popReg(struct CPU *CPU);

void popRAM(struct CPU *CPU);

void popDel(struct CPU *CPU);

void add(struct CPU *CPU);

void sub(struct CPU *CPU);//First sub Second or conversely

void mul(struct CPU *CPU);

void div(struct CPU *CPU);

void in(struct CPU *CPU);

void out(struct CPU *CPU);//STACK PEEK

void jmp(struct CPU *CPU); //To number of byte

void call(struct CPU *CPU);

void ret(struct CPU *CPU);

void ja(struct CPU *CPU);

void jb(struct CPU *CPU);

void je(struct CPU *CPU);

//void end(struct CPU *CPU);

int main() {
    struct CPU CPU = {};
    CPUCtor(&CPU);

    while (CPU.pc < CPU.codeSize) {
        switch (CPU.byte_code[CPU.pc]) {
            case ASM_PUSH_CONST:
                pushConst(&CPU);
                break;
            case ASM_PUSH_REG:
                pushRegister(&CPU);
                break;
            case ASM_PUSH_RAM:
                pushRAM(&CPU);
                break;
            case ASM_POP_REG:
                popReg(&CPU);
                break;
            case ASM_POP_RAM:
                popRAM(&CPU);
                break;
            case ASM_POP_DEL:
                popDel(&CPU);
                break;
            case ASM_ADD:
                add(&CPU);
                break;
            case ASM_SUB:
                sub(&CPU);
                break;
            case ASM_MUL:
                mul(&CPU);
                break;
            case ASM_DIV:
                div(&CPU);
                break;
            case ASM_IN:
                in(&CPU);
                break;
            case ASM_OUT:
                out(&CPU);
                break;
            case ASM_JMP:
                jmp(&CPU);
                break;
            case ASM_CALL:
                call(&CPU);
                break;
            case ASM_RET:
                ret(&CPU);
                break;
            case ASM_JA:
                ja(&CPU);
                break;
            case ASM_JB:
                jb(&CPU);
                break;
            case ASM_JE:
                je(&CPU);
                break;
            case ASM_END:
                goto ASM_END_LABEL;
                //default:

        }
    }

    ASM_END_LABEL: // form case ASM_END in main switch

    CPUDtor(&CPU);

    return 0;
}

void CPUCtor(struct CPU *CPU) {
    static struct Stack stack = {};
    StackCtor(&stack);
    CPU->stack = &stack;
    CPU->pc = 0;
    unsigned int codeSize = 0;
    CPU->byte_code = readCode("D:\\Code\\CLionProjects\\asm\\cmake-build-debug\\bytecode.txt", &codeSize);
    CPU->codeSize = codeSize;
    CPU->registers = calloc(RegisterNum + 1, sizeof(*(CPU->registers)));
    CPU->ram = calloc(RAMLen, sizeof(*(CPU->ram)));
}

void CPUDtor(struct CPU *CPU) {
    StackDtor(CPU->stack);
    CPU->pc = 0;
    CPU->codeSize = 0;
    free(CPU->byte_code);
    free(CPU->registers);
    free(CPU->ram);
    CPU->byte_code = NULL;
    CPU->registers = NULL;
    CPU->ram = NULL;
}

char *readCode(char *filename, unsigned int *size) {
    FILE *input = fopen(filename, "rb");

    struct stat fileStat = {};
    fstat(fileno(input), &fileStat);
    *size = fileStat.st_size;
    char *buffer = (char *) calloc(size, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), size, input);

    fclose(input);

    return buffer;
}

void pushConst(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, *(data_t *) (CPU->byte_code + CPU->pc));
    CPU->pc += sizeof(data_t);
}

void pushRegister(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, CPU->registers[CPU->byte_code[CPU->pc++]]);
}

void pushRAM(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, CPU->ram[CPU->byte_code[CPU->pc]]);
    CPU->pc += sizeof(int);
}

void popReg(struct CPU *CPU) {
    CPU->pc++;
    CPU->registers[CPU->byte_code[CPU->pc++]] = StackPop(CPU->stack);
}

void popRAM(struct CPU *CPU) {
    CPU->pc++;
    CPU->ram[CPU->byte_code[CPU->pc]] = StackPop(CPU->stack);
    CPU->pc += sizeof(int);
}

void popDel(struct CPU *CPU) {
    CPU->pc++;
    StackPop(CPU->stack);
}

void add(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, StackPop(CPU->stack) + StackPop(CPU->stack));
}

void sub(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, StackPop(CPU->stack) - StackPop(CPU->stack));
}

void mul(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, StackPop(CPU->stack) * StackPop(CPU->stack));
}

void div(struct CPU *CPU) {
    CPU->pc++;
    StackPush(CPU->stack, StackPop(CPU->stack) / StackPop(CPU->stack));
}

void in(struct CPU *CPU) {
    system("pause");
    printf("Enter input number\n");
    data_t input = 0;
    if (scanf("%d", &input) == 0) {
        while (scanf("%d", &input) == 0) {
            printf("Input error: no input data\n");
            system("pause");
        }
    }
    StackPush(CPU->stack, input);
    CPU->pc++;
}

void out(struct CPU *CPU) {
    printf("%d\n", StackPeek(CPU->stack));
    CPU->pc++;
}

void jmp(struct CPU *CPU) {
    CPU->pc++;
    CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
}

void call(struct CPU *CPU) {
    CPU->pc++;
    CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
}

void ret(struct CPU *CPU) {
    CPU->pc++;
    CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
}

void ja(struct CPU *CPU) {
    CPU->pc++;
    data_t a = StackPop(CPU->stack);
    data_t b = StackPeek(CPU->stack);
    if (a > b) {
        CPU->pc += sizeof(int);
    } else {
        CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
    }
    StackPush(CPU->stack, a);
}

void jb(struct CPU *CPU) {
    CPU->pc++;
    data_t a = StackPop(CPU->stack);
    data_t b = StackPeek(CPU->stack);
    if (a < b) {
        CPU->pc += sizeof(int);
    } else {
        CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
    }
    StackPush(CPU->stack, a);
}

void je(struct CPU *CPU) {
    CPU->pc++;
    data_t a = StackPop(CPU->stack);
    data_t b = StackPeek(CPU->stack);
    if (a == b) {
        CPU->pc += sizeof(int);
    } else {
        CPU->pc = *(int *) (CPU->byte_code + CPU->pc);
    }
    StackPush(CPU->stack, a);
}