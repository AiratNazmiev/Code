#include <stdio.h>
#include <malloc.h>
#include "stack.h"

int main() {
    struct Stack s = {};
    StackCtor(&s);
    for (int i = 0; i < 15; i++) {
        StackPush(&s, i);
    }

    //StackDump(stdout, &s);

    FILE *output = fopen("unittests.txt", "w");
    for (int i = 14; i >= 0; i--) {
        data_t tmp = StackPop(&s);
        UNITTEST(tmp, (data_t) i, output);
    }

    for (int i = 0; i < 15; i++) {
        StackPush(&s, i);
    }

    UNITTEST(StackPeek(&s), 42.0, output); //wrong test

    data_t *arr = StackToArray(&s);
    for (int i = 14; i >= 0; i--) {
        UNITTEST(arr[i], (data_t) i, output);
    }

    StackArrayDtor(arr);

    for (int i = 14; i >= 0; i--) {
        data_t tmp = StackPeek(&s);
        UNITTEST(tmp, (data_t) i, output);
        UNITTEST((data_t) StackSize(&s), (data_t) i + 1, output)
        tmp = StackPop(&s);
        UNITTEST(tmp, (data_t) i, output);
    }

    UNITTEST((data_t) StackIsEmpty(&s), 0.0, output);

    for (int i = 0; i < 18; i++) {
        StackPush(&s, i);
    }

    for (int i = 17; i >= 10; i--) {
        StackPop(&s);
    }

    StackDump(output, &s);
    StackDtor(&s);
}