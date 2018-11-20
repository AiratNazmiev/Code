#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>
#include <process.h>

#include "stack/stack.h"
#include "CPU.h"

const unsigned int RAMLen = 16;
const unsigned int RegisterNum = 4;
const unsigned int CPU_POISON = UINT_MAX;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        CPUerror(CPU_OPEN_FILE_ERROR);
    }

    struct Stack callStack = {};
    StackCtor(&callStack);

    struct CPU CPU = {};
    CPUCtor(&CPU, argv[1]);

    while (CPU.pc < CPU.codeSize) {
        CPU_ASSERT(CPUOk(&CPU));

        switch (CPU.byte_code[CPU.pc]) {

        #define DEF_CMD(name, instr_enum, num, asm, asm_label, processing)  case num: {processing; break;}
        #include "D:/Code/CLionProjects/asmCommands/commands.h"
        #undef DEF_CMD

            default: CPUerror(CPU_UNKNOWN_INSTRUCTION);

        }
    }

    ASM_END_LABEL:;// form case ASM_END in main switch

    FILE *dumpFile = fopen("CPUDdmp.txt", "w");
    CPUDump(dumpFile, &CPU);
    fclose(dumpFile);

    CPUDtor(&CPU);

    return 0;
}

void CPUCtor(struct CPU *CPU, char *input) {
    static struct Stack stack = {};
    StackCtor(&stack);
    CPU->stack = &stack;

    CPU->pc = 0;

    unsigned int codeSize = 0;
    CPU->byte_code = readCode(input, &codeSize);
    CPU->codeSize = codeSize;

    CPU->registers = calloc(RegisterNum + 1, sizeof(*(CPU->registers)));
    CPU->ram = calloc(RAMLen, sizeof(*(CPU->ram)));
}

void CPUDtor(struct CPU *CPU) {
    CPU_ASSERT(CPUOk(CPU));
    StackDtor(CPU->stack);
    CPU->pc = CPU_POISON;
    CPU->codeSize = CPU_POISON;

    free(CPU->byte_code);
    free(CPU->registers);
    free(CPU->ram);

    CPU->byte_code = NULL;
    CPU->registers = NULL;
    CPU->ram = NULL;
}

int CPUOk(struct CPU *CPU){
    if (CPU == NULL){
        return CPU_NULLPTR;
    }

    if (CPU->stack == NULL){
        return CPU_STACK_NULLPTR;
    }

    if (CPU->byte_code == NULL){
        return CPU_CODE_NULLPTR;
    }

    if (CPU->ram == NULL){
        return CPU_RAM_NULLPTR;
    }

    if (CPU->registers == NULL){
        return CPU_REG_NULLPTR;
    }

    if (CPU->pc == CPU_POISON){
        return CPU_DAMAGED_PC;
    }

    if (CPU->codeSize == CPU_POISON){
        return CPU_DAMAGED_CODESIZE;
    }

    return CPU_OK;
}

int CPUerror(unsigned int num) {
    switch (num) {
        case CPU_WRONG_FILE_NAME:
            fprintf(stderr, "CPU error: wrong input file name");
            exit(CPU_OPEN_FILE_ERROR);
        case CPU_OPEN_FILE_ERROR:
            fprintf(stderr, "CPU error: cannot open input file");
            exit(CPU_OPEN_FILE_ERROR);
        case CPU_UNKNOWN_INSTRUCTION:
            fprintf(stderr, "CPU error: unknown instruction");
            exit(CPU_UNKNOWN_INSTRUCTION);
        default:
            fprintf(stderr, "CPU error: unknown error");
            exit(CPU_UNKNOWN_ERROR);
    }
}

void CPUDump(FILE* file, struct CPU *CPU){
    fprintf(file, "<<<<<CPU DUMP>>>>> {\n\n");
    fprintf(file, "1)CPU.stack:\n\n");
    StackDump(file, CPU->stack);
    fprintf(file, "\n2)CPU.pc = %u\n\n", CPU->pc);
    fprintf(file, "3)CPU.codeSize = %u\n\n", CPU->codeSize);
    fprintf(file, "4)&(CPU.bytecode) = [0x%X]\n\n", CPU->byte_code);

    fprintf(file, "5)&(CPU.registers) = [0x%X]: ", CPU->registers);
    for (int i = 1; i <= RegisterNum; i++) fprintf(file, "%d ", CPU->registers[i]);
    fprintf(file, "\n");

    fprintf(file, "\n6)&(CPU.ram) = [0x%X]: ", CPU->ram);
    for (int i = 0; i < RAMLen; i++) fprintf(file, "%d ", CPU->ram[i]);
    fprintf(file, "\n}\n\n");
}

char *readCode(char *filename, unsigned int *size) {
    if (filename == NULL) {
        CPUerror(CPU_WRONG_FILE_NAME);
    }

    FILE *input = fopen(filename, "rb");

    if (input == NULL) {
        CPUerror(CPU_OPEN_FILE_ERROR);
    }

    struct stat fileStat = {};
    fstat(fileno(input), &fileStat);
    *size = fileStat.st_size;

    char *buffer = (char *) calloc(size, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), size, input);

    fclose(input);

    return buffer;
}