#include <stdio.h>
#include "stack.h"

int main() {
    struct Stack s = {};
    StackCtor(&s);
    StackPush(&s, 32);
    StackPush(&s, 42);
    StackPush(&s, 432);
    printf("%lf\n", StackPop(&s));
    printf("%lf\n", StackPop(&s));
    StackClear(&s);
    StackPush(&s, 3432);
    StackPush(&s, 4254);
    StackPush(&s, 43232);
    printf("%d\n", StackSize(&s));
    StackPush(&s, 353452);
    StackPush(&s, 4234232);
    StackPush(&s, 123432);
    printf("%lf\n", StackPop(&s));
    printf("%lf\n", StackPeek(&s));
    StackDtor(&s);
}