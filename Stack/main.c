#include <stdio.h>
#include "stack.h"

int main() {
    struct Stack s = {};
    StackCtor(&s);
    for (int i = 0; i < 15; i++) {
        StackPush(&s, i);
    }
    StackDump(stdout, &s);

    FILE *output = fopen("unittests.txt", "w");
    for (int i = 14; i >= 0; i--) {
        data_t tmp = StackPop(&s);
        printf("%lf\n", tmp);
        UNITTEST(tmp, (data_t) i , output);
    }

    for (int i = 0; i < 15; i++) {
        StackPush(&s, i);
    }

    data_t *arr = StackToArray(&s);
    for (int i = 14; i >= 0; i--) {
        printf("%lf\n", arr[i]);
        UNITTEST(arr[i], (data_t) i , output);
    }
}
