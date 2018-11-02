#include <stdio.h>
#include <malloc.h>
#include <float.h>
#include <assert.h>
#include <mem.h>
#include "stack.h"

#define ASSERT(check){\
    int res = check;\
    if ((res)){\
    fprintf(stderr, "Function %s: %s in File %s, line %d\n",__FUNCTION__ , #check, __FILE__, __LINE__);\
    }\
    switch (res){\
        case OK: break;\
        case NULL_STACK_POINTER: fprintf(stderr, "STACK: NULL_STACK_POINTER\n"); break;\
        case NULL_DATA_POINTER: fprintf(stderr, "STACK: NULL_DATA_POINTER\n"); break;\
        case NEGATIVE_SIZE: fprintf(stderr, "STACK: NEGATIVE_SIZE\n"); break;\
        case NEGATIVE_CAPACITY: fprintf(stderr, "STACK: NEGATIVE_CAPACITY\n"); break;\
        case STACK_OVERFLOW: fprintf(stderr, "STACK: STACK_OVERFLOW\n"); break;\
        case LEFT_BOARDER_DAMAGED: fprintf(stderr, "STACK: LEFT_BOARDER_DAMAGED\n"); break;\
        case RIGHT_BOARDER_DAMAGED: fprintf(stderr, "STACK: RIGHT_BOARDER_DAMAGED\n"); break;\
        case FORBIDDEN_REPLACEMENT: fprintf(stderr, "STACK: FORBIDDEN_REPLACEMENT\n"); break;\
        default: fprintf(stderr, "STACK: UNKNOWN ERROR\n");\
    }\
}

const unsigned int STACK_MAX_SIZE = INT_MAX;
const unsigned int STACK_EXP_REDUCT = 2; //if size * EXPANSION_REDUCTION_RATIO <= capacity then realloc memory and  if size = capacity then capacity * EXPANSION_REDUCTION_RATIO
const unsigned int STACK_POISON = INT_MAX;
const unsigned int STACK_EMPTY = 1;
const unsigned int STACK_NOT_EMPTY = 0;
const data_t ERROR_VALUE = INT_MAX;
const data_t STACK_DATA_POISON = INT_MAX;

#ifdef STACKSAFE

const unsigned int HASH_C = 42;
const unsigned int HASH_MOD = 123321;
const data_t LEFT_CANARY = DBL_MAX;
const data_t RIGHT_CANARY = DBL_MAX;

#endif

enum errorNum {
    OK,
    NEGATIVE_SIZE,
    NEGATIVE_CAPACITY,
    STACK_OVERFLOW,
    NULL_STACK_POINTER,
    NULL_DATA_POINTER,
    LEFT_BOARDER_DAMAGED,
    RIGHT_BOARDER_DAMAGED,
    FORBIDDEN_REPLACEMENT,
};

inline static void StackPoison(struct Stack *s);

inline static unsigned int hashCalc(const struct Stack *s);

enum errorNum StackOK(const struct Stack *s) {
    if (s == NULL) {
        return NULL_STACK_POINTER;
    }

    if (s->data == NULL) {
        return NULL_DATA_POINTER;
    }

    if (s->size >= STACK_MAX_SIZE) {
        return NEGATIVE_SIZE;
    }

    if (s->capacity >= STACK_MAX_SIZE) {
        return NEGATIVE_CAPACITY;
    }

    if (s->size > s->capacity) {
        return STACK_OVERFLOW;
    }

#ifdef STACKSAFE

    if (s->data[0] != LEFT_CANARY) {
        return LEFT_BOARDER_DAMAGED;
    }

    if (s->data[s->size + 1] != RIGHT_CANARY) {
        return RIGHT_BOARDER_DAMAGED;
    }

    if (s->hash != hashCalc(s)) {
        return FORBIDDEN_REPLACEMENT;
    }

#endif

    return OK;
}

void StackCtor(struct Stack *s) {
#ifdef STACKSAFE

    s->data = (data_t *) calloc(2, sizeof(*(s->data))); //////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! calloc 2

#else

    s->data = (data_t *) calloc(1, sizeof(*(s->data)));

#endif
    if (s->data == NULL) {
        fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
    }

    s->size = 0;
    s->capacity = 0;

#ifdef STACKSAFE

    s->data[0] = LEFT_CANARY;
    s->data[1] = RIGHT_CANARY;
    s->hash = 0;

#endif
}

void StackPush(struct Stack *s, data_t value) {
    ASSERT(StackOK(s));

    data_t *tmp = 0;

    if (s->capacity == 0) {

#ifdef STACKSAFE

        tmp = realloc(s->data, 3 * sizeof(data_t));

#else

        tmp = realloc(s->data, 1 * sizeof(data_t));

#endif

        if (tmp == NULL) {
            fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        } else {
            s->data = tmp;
            s->size = 1;
            s->capacity = 1;

#ifdef STACKSAFE

            s->data[1] = value;
            s->data[2] = RIGHT_CANARY;
            s->hash = (s->hash + ((unsigned int) value * HASH_C)) % HASH_MOD;

#else

            s->data[0] = value;

#endif

        }
    } else {

        if (s->capacity == s->size) {

#ifdef STACKSAFE

            if (2 + s->capacity * STACK_EXP_REDUCT >= STACK_MAX_SIZE) {
                fprintf(stderr, "Function %s: Stack max size error %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
            }

            tmp = realloc(s->data, (2 + s->capacity * STACK_EXP_REDUCT) * sizeof(data_t));

#else

            if (s->capacity * STACK_EXP_REDUCT >= STACK_MAX_SIZE) {
                fprintf(stderr, "Function %s: Stack max size error %s, line %d\n", __FUNCTION__, __FILE__,
                        __LINE__);
            }

            tmp = realloc(s->data, (s->capacity * STACK_EXP_REDUCT) * sizeof(data_t));

#endif

            if (tmp == NULL) {
                fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
            } else {
                s->data = tmp;
                s->capacity = s->capacity * STACK_EXP_REDUCT;
            }
        }


#ifdef STACKSAFE

        s->data[1 + s->size++] = value;
        s->data[1 + s->size] = RIGHT_CANARY;
        s->hash = (s->hash + ((unsigned int) value * HASH_C)) % HASH_MOD;

#else

        s->data[s->size++] = value;

#endif
    }
}


data_t StackPop(struct Stack *s) {
    ASSERT(StackOK(s));

    if (s->size < 1) {
        fprintf(stderr, "Function %s: Cannot pop element error (empty stack) in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return ERROR_VALUE;
    }

    if (s->size * STACK_EXP_REDUCT <= s->capacity) {

#ifdef STACKSAFE

        data_t *tmp = realloc(s->data, (2 + s->capacity / STACK_EXP_REDUCT) * sizeof(data_t));

#else

        data_t *tmp = realloc(s->data, (s->capacity / STACK_EXP_REDUCT) * sizeof(data_t));

#endif

        if (tmp == NULL) {
            fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__,
                    __LINE__);

            return ERROR_VALUE;
        } else {
            s->data = tmp;
            s->capacity = s->capacity / STACK_EXP_REDUCT;
        }
    }

#ifdef STACKSAFE

    data_t value = s->data[s->size--];
    s->data[1 + s->size] = RIGHT_CANARY;
    s->hash = hashCalc(s);

#else

    data_t value = s->data[--s->size];
    s->data[s->size] = STACK_DATA_POISON;

#endif

    return value;
}

data_t StackPeek(const struct Stack *s) {
    ASSERT(StackOK(s));

    if (s->size > 0) {

#ifdef STACKSAFE

        return s->data[s->size];

#else

        return s->data[s->size - 1];

#endif
    } else {
        fprintf(stderr, "Function %s: Cannot peek element error (empty stack) in File %s, line %d\n", __FUNCTION__,
                __FILE__, __LINE__);

        return ERROR_VALUE;
    }
}

unsigned int StackIsEmpty(const struct Stack *s) {
    ASSERT(StackOK(s));

    return (s->size > 0) ? STACK_EMPTY : STACK_NOT_EMPTY;
}

unsigned int StackSize(const struct Stack *s) {
    ASSERT(StackOK(s));

    return s->size;
}

void StackClear(struct Stack *s) {
    ASSERT(StackOK(s));

    StackPoison(s);

#ifdef STACKSAFE

    s->data = realloc(s->data, 2 * sizeof(data_t));

#else

    s->data = realloc(s->data, 1 * sizeof(data_t));

#endif

    if (s->data == NULL) {
        fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__,
                __LINE__);
    }

#ifdef STACKSAFE

    s->data[0] = LEFT_CANARY;
    s->data[1] = RIGHT_CANARY;
    s->hash = 0;

#endif

    s->capacity = 0;
    s->size = 0;
}

void StackDtor(struct Stack *s) {
    ASSERT(StackOK(s));

    StackPoison(s);
    free(s->data);
    s->data = NULL;
    s->size = STACK_POISON;
    s->capacity = STACK_POISON;

#ifdef STACKSAFE

    s->hash = STACK_POISON;

#endif
}

data_t *StackToArray(const struct Stack *s) {
    ASSERT(StackOK(s));

    if (s == NULL) {
        fprintf(stderr, "Function %s: Stack NULL pointer error in File %s, line %d\n", __FUNCTION__, __FILE__,
                __LINE__);
        return NULL;
    }

    data_t *arr = (data_t *) calloc(s->size, sizeof(data_t));

    if (arr == NULL) {
        fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return NULL;
    }

#ifdef STACKSAFE

    memcpy(arr, s->data + 1, sizeof(data_t) * s->size);

#else

    memcpy(arr, s->data, sizeof(data_t) * s->size);

#endif

    return arr;
}

void StackArrayDtor(data_t *arr) {
    free(arr);
}

void StackDump(FILE *f, struct Stack *s) {
    fprintf(f, "StackDump\n{\n");
    fprintf(f, "Stack: [0x%X]\n", s);
    fprintf(f, "capacity = %u\n", s->capacity);
    fprintf(f, "size = %u\n", s->size);
    fprintf(f, "array data[%u] :[0x%X]\n", s->size, &(s->data[0]));

#ifdef STACKSAFE

    fprintf(f, "[canary] data[0] = %lf\n", s->data[0]);

    for (int i = 1; i < s->size; i++) {
        fprintf(f, "[in] data[%u] = %lf\n", i, s->data[i]);
    }

    fprintf(f, "[canary] data[%u] = %lf\n", s->size, s->data[0]);

    for (int i = s->size + 1; i < s->capacity + 2; i++) {
        fprintf(f, "[out] data[%u] = %lf\n", i, s->data[i]);
    }

#else

    for (int i = 0; i < s->size; i++) {
        fprintf(f, "[in] data[%u] = %d\n", i, s->data[i]);
    }

    for (int i = s->size; i < s->capacity; i++) {
        fprintf(f, "[out] data[%u] = %d\n", i, s->data[i]);
    }

#endif

    fprintf(f, "}\n");
}

inline static void StackPoison(struct Stack *s) {

#ifdef STACKSAFE

    for (int i = 0; i < s->size + 2; i++) {
        s->data[i] = STACK_DATA_POISON;
    }

#else

    for (int i = 0; i < s->size; i++) {
        s->data[i] = STACK_DATA_POISON;
    }

#endif
}

#ifdef STACKSAFE

inline static unsigned int hashCalc(const struct Stack *s) {
    unsigned int sum = 0;

    for (int i = 1; i < s->size + 1; i++) {
        sum = (sum + (((unsigned int) s->data[i]) * HASH_C)) % HASH_MOD;
    }

    return sum;
}

#endif