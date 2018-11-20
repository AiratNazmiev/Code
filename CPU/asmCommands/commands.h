DEF_CMD(push, PUSH_CONST, 1,
{
    if (sscanf(asm_code.str_ptr[i], "push %d", &arg1) == 1) {
        byte_code[pc++] = ASM_PUSH_CONST;
        *(data_t *) (byte_code + pc) = arg1;
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
},

{
    if (sscanf(asm_code->str_ptr[i], "push %d", &arg1) == 1) {
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
},

{
    CPU.pc++;
    PUSH(*(data_t * )(CPU.byte_code + CPU.pc));
    CPU.pc += sizeof(data_t);
})

DEF_CMD(pushReg, PUSH_REG, 2, , ,
{
    CPU.pc++;
    PUSH(CPU.registers[CPU.byte_code[CPU.pc++]]);
})

DEF_CMD(pushRam, PUSH_RAM, 3, , ,
{
    CPU.pc++;
    PUSH(CPU.ram[CPU.byte_code[CPU.pc]]);
    CPU.pc += sizeof(int);
})

DEF_CMD(pop, POP_REG, 4,
{
if (sscanf(asm_code.str_ptr[i], "pop %s", str_arg) == 1) {

    if (!strcmp(str_arg, "ra")) {
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

    if (sscanf(asm_code.str_ptr[i], "pop [%d]", &arg1) == 1) {
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
},

{
if (sscanf(asm_code->str_ptr[i], "pop %s", str_arg) == 1) {

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

},

{
    CPU.pc++;
    CPU.registers[CPU.byte_code[CPU.pc++]] = POP;
})

DEF_CMD(popRam, POP_RAM, 5, , ,
{
    CPU.pc++;
    CPU.ram[CPU.byte_code[CPU.pc]] = POP;
    CPU.pc += sizeof(int);
})

DEF_CMD(popDel, POP_DEL, 6, , ,
{
    CPU.pc++;
    POP;
})

DEF_CMD(add, ADD, 7,
{
    byte_code[pc++] = ASM_ADD;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    PUSH(POP + POP);
})

DEF_CMD(sub, SUB, 8,
{
    byte_code[pc++] = ASM_SUB;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    PUSH(POP - POP);
})

DEF_CMD(mul, MUL, 9,
{
    byte_code[pc++] = ASM_MUL;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    PUSH(POP * POP);
})

DEF_CMD(div, DIV, 10,
{
    byte_code[pc++] = ASM_DIV;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    PUSH(POP / POP);
})

DEF_CMD(in, IN, 11,
{
    byte_code[pc++] = ASM_IN;
    continue;
},

{
    pc++;
    continue;
},

{
    system("pause");
    printf("Enter input number\n");
    data_t input = 0;
        while (scanf("%d", &input) == 0) {
            printf("Input error: no input data\n");
            system("pause");
        }
    PUSH(input);
    CPU.pc++;
})

DEF_CMD(out, OUT, 12,
{
    byte_code[pc++] = ASM_OUT;
    continue;
},

{
    pc++;
    continue;
},

{
    printf("%d\n", PEEK);
    CPU.pc++;
})

DEF_CMD(jmp, JMP, 13,
{
    if (sscanf(asm_code.str_ptr[i], "jmp :%d", &arg1) == 1) {

    if ((arg1 < 0) || (arg1 >= LabelsNum)) {
        asmTranslationError(ASM_WRONG_LABEL);
    }

        byte_code[pc++] = ASM_JMP;
        *(int *) (byte_code + pc) = labels[arg1];
        pc += sizeof(int);
        continue;
    } else {
        asmTranslationError(ASM_WRONG_JMP_ARG);
    }
},

{
    if (sscanf(asm_code->str_ptr[i], "jmp :%d", &arg1)) {

    if ((arg1 < 0) || (arg1 >= LabelsNum)) {
        asmTranslationError(ASM_WRONG_LABEL);
    }

    pc += (sizeof(int) + 1);
    continue;
    }
},

{
    CPU.pc++;
    CPU.pc = *(int *) (CPU.byte_code + CPU.pc);
})

DEF_CMD(call, CALL, 14,
{
    if (sscanf(asm_code.str_ptr[i], "call :%d", &arg1) == 1) {
        byte_code[pc++] = ASM_CALL;
        *(int *) (byte_code + pc) = labels[arg1];
        pc += sizeof(int);
        continue;
    } else {
        asmTranslationError(ASM_WRONG_CALL_ARG);
    }
},

{
    pc += (sizeof(int) + 1);
    continue;
},

{
    CPU.pc++;
    StackPush(&callStack, CPU.pc + sizeof(int));
    CPU.pc = *(int *) (CPU.byte_code + CPU.pc);
})

DEF_CMD(ret, RET, 15,
{
    byte_code[pc++] = ASM_RET;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    CPU.pc = StackPop(&callStack);
})

DEF_CMD(ja, JA, 16,
{
    if (sscanf(asm_code.str_ptr[i], "ja :%d", &arg1) == 1) {
        byte_code[pc++] = ASM_JA;
        *(int *) (byte_code + pc) = labels[arg1];
        pc += sizeof(int);
        continue;
    } else {
        asmTranslationError(ASM_WRONG_JA_ARG);
    }
},

{
    pc += (sizeof(int) + 1);
    continue;
},

{
    CPU.pc++;
    data_t a = POP;
    data_t b = PEEK;
    if (a > b) {
        CPU.pc += sizeof(int);
    } else {
        CPU.pc = *(int *) (CPU.byte_code + CPU.pc);
    }
    PUSH(a);
})

DEF_CMD(jb, JB, 17,
{
    if (sscanf(asm_code.str_ptr[i], "jb :%d", &arg1) == 1) {
    byte_code[pc++] = ASM_JB;
    *(int *) (byte_code + pc) = labels[arg1];
    pc += sizeof(int);
    continue;
    } else {
    asmTranslationError(ASM_WRONG_JB_ARG);
    }
},

{
    pc += (sizeof(int) + 1);
    continue;
},

{
    CPU.pc++;
    data_t a = POP;
    data_t b = PEEK;
    if (a < b) {
    CPU.pc += sizeof(int);
    } else {
    CPU.pc = *(int *) (CPU.byte_code + CPU.pc);
    }
    PUSH(a);
})

DEF_CMD(je, JE, 18,
{
    if (sscanf(asm_code.str_ptr[i], "je :%d", &arg1) == 1) {
        byte_code[pc++] = ASM_JE;
        *(int *) (byte_code + pc) = labels[arg1];
        pc += sizeof(int);
        continue;
    } else {
        asmTranslationError(ASM_WRONG_JE_ARG);
    }
},

{
    pc += (sizeof(int) + 1);
    continue;
},

{
    CPU.pc++;
    data_t a = POP;
    data_t b = PEEK;
    if (a == b) {
        CPU.pc += sizeof(int);
    } else {
        CPU.pc = *(int *) (CPU.byte_code + CPU.pc);
    }
    PUSH(a);
})

DEF_CMD(end, END, 19,
{
    byte_code[pc++] = ASM_END;
    continue;
},

{
    pc++;
    continue;
},

{
    goto ASM_END_LABEL;
})

DEF_CMD(sqrt, INT_SQRT, 20,
{
    byte_code[pc++] = ASM_INT_SQRT;
    continue;
},

{
    pc++;
    continue;
},

{
    CPU.pc++;
    PUSH((int) sqrt(POP));
})