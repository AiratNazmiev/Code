#include <stdio.h>
#include <malloc.h>
#include <float.h>
#include <assert.h>
#include <mem.h>
#include "stack.h"

const unsigned int STACK_MAX_SIZE = UINT_MAX;
const unsigned int STACK_EXP_REDUCT = 2;//if size * EXPANSION_REDUCTION_RATIO <= capacity then realloc memory and  if size = capacity then capacity * EXPANSION_REDUCTION_RATIO
const unsigned int STACK_CTOR_SIZE = 1;
const unsigned int STACK_POISON = UINT_MAX;
const unsigned int STACK_EMPTY = 1;
const unsigned int STACK_NOT_EMPTY = 0;
const data_t ERROR_VALUE = DBL_MAX;
const data_t STACK_DATA_POISON = DBL_MAX;
const unsigned int HASH_C = 1872;
const unsigned int HASH_MOD = 123434243;
const data_t LEFT_CANARY = DBL_MAX - 1;
const data_t RIGHT_CANARY = DBL_MAX - 1;

enum errorNum {
    OK,
    NEGATIVE_SIZE,
    NEGATIVE_CAPACITY,
    STACK_OVERFLOW,
    NULL_STACK_POINTER,
    NULL_DATA_POINTER,
    LEFT_BOARDER_DAMAGED,
    RIGHT_BOARDER_DAMAGED,
    FORBIDDEN_REPLACEMENT
};

inline static void StackPoison(struct Stack *s);

inline static unsigned int hashCalc(const struct Stack *s);


/*
 * #include <errno.h>
 * perror("msg"); "msg: ... "
 * #define DUMP_ERR(msg) do{ perror(msg); fprintf(stderr, "At %s %s %s...);
 *
 *
*/

void StackCtor(struct Stack *s) {
    assert(s);

    s->data = (data_t *) calloc(STACK_CTOR_SIZE, sizeof(*(s->data)));

    if (s->data == NULL) {
        fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
    }

    s->size = STACK_CTOR_SIZE;
    s->capacity = STACK_CTOR_SIZE;
    s->hash = 0;
}

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

//    if (s->data[0] != LEFT_CANARY) {
//        return LEFT_BOARDER_DAMAGED;
//    }
//
//    if (s->data[s->size - 1] != RIGHT_CANARY) {
//        return RIGHT_BOARDER_DAMAGED;
//    }

    if (s->hash != hashCalc(s)) {
        return FORBIDDEN_REPLACEMENT;
    }

    return OK;
}

void StackPush(struct Stack *s, data_t value) {
    ASSERT(StackOK(s));

    if (s->capacity == s->size) {

        if (s->capacity * STACK_EXP_REDUCT >= STACK_MAX_SIZE) {
            fprintf(stderr, "Function %s: Stack max size error %s, line %d\n", __FUNCTION__, __FILE__,
                    __LINE__);
        }

        data_t *tmp = realloc(s->data, s->capacity * STACK_EXP_REDUCT * sizeof(data_t));

        if (tmp == NULL) {
            fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__,
                    __LINE__);
        } else {
            s->data = tmp;
            s->capacity = s->capacity * STACK_EXP_REDUCT;
        }
    }

    s->data[s->size++] = value;
    s->hash = (s->hash + ((unsigned int) value * HASH_C)) % HASH_MOD;
}


data_t StackPop(struct Stack *s) {
    ASSERT(StackOK(s));

    if (s->size < 1) {
        fprintf(stderr, "Function %s: Cannot pop element error (empty stack) in File %s, line %d\n", __FUNCTION__,
                __FILE__, __LINE__);
        return ERROR_VALUE;
    }

    if (s->size * STACK_EXP_REDUCT <= s->capacity) {
        data_t *tmp = realloc(s->data, s->capacity / STACK_EXP_REDUCT * sizeof(data_t));
        if (tmp == NULL) {
            fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__,
                    __LINE__);

            return ERROR_VALUE;
        } else {
            s->data = tmp;
            s->capacity = s->capacity / STACK_EXP_REDUCT;
        }
    }


    data_t value = s->data[--s->size];
    s->data[s->size] = STACK_DATA_POISON;
    s->hash = hashCalc(s);

    return value;
}

data_t StackPeek(const struct Stack *s) {
    ASSERT(StackOK(s));

    if (s->size > 0) {
        return s->data[s->size - 1];
    } else {
        fprintf(stderr, "Function %s: Cannot peek element error (empty stack) in File %s, line %d\n", __FUNCTION__,
                __FILE__,
                __LINE__);
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
    s->data = realloc(s->data, STACK_CTOR_SIZE);

    if (s->data == NULL) {
        fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__,
                __LINE__);
    }

    s->capacity = STACK_CTOR_SIZE;
    s->size = 0;
    s->hash = 0;
}

void StackDtor(struct Stack *s) {
    ASSERT(StackOK(s));

    StackPoison(s);
    free(s->data);
    s->data = NULL;

    s->size = STACK_POISON;
    s->capacity = STACK_POISON;
    s->hash = 0;
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

    memcpy(arr, s->data, sizeof(data_t) * s->size);
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

    for (int i = 0; i < s->size; i++) {
        fprintf(f, "[in] data[%u] = %lf\n", i, s->data[i]);
    }

    for (int i = s->size; i < s->capacity; i++) {
        fprintf(f, "[out] data[%u] = %lf\n", i, s->data[i]);
    }

    fprintf(f, "}");
}

inline static void StackPoison(struct Stack *s) {
    for (int i = 0; i < s->size; i++) {
        s->data[i] = STACK_DATA_POISON;
    }
}

inline static unsigned int hashCalc(const struct Stack *s) {
    unsigned int sum = 0;

    for (int i = 0; i < s->size; i++) {
        sum = (sum + (((unsigned int) s->data[i]) * HASH_C)) % HASH_MOD;
    }

    return sum;
}