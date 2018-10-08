#include <stdio.h>
#include <malloc.h>
#include <mem.h>
#include <float.h>
#include "stack.h"

const unsigned int STACK_MAX_SIZE = UINT_MAX;
const unsigned int STACK_EXPANSION_REDUCTION_RATIO = 2;//if size * EXPANSION_REDUCTION_RATIO <= capacity then realloc memory and  if size = capacity then capacity * EXPANSION_REDUCTION_RATIO
const unsigned int STACK_CTOR_SIZE = 1;
const unsigned int STACK_POISON = 0;
const unsigned int STACK_EMRTY = 1;
const unsigned int STACK_NOT_EMPTY = 0;
const data_t ERROR_VALUE = 0;

enum errorNum {
    NEGATIVE_SIZE = 1, NEGATIVE_CAPACITY, STACK_OVERFLOW, NULL_STACK_POINTER, NULL_DATA_POINTER
};

void StackCtor(struct Stack *s) {
    s->data = (data_t *) calloc(STACK_CTOR_SIZE, sizeof(*(s->data)));
    s->size = 0;
    s->capacity = STACK_CTOR_SIZE;
}

unsigned int StackOK(const struct Stack *s) {
    if (s == NULL) {
        return NULL_DATA_POINTER;
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

    return 0;
}

void StackPush(struct Stack *s, data_t value) {
    ASSERT(!StackOK(s));

    if (s->capacity == s->size) {
        data_t *tmp = realloc(s->data, s->capacity * STACK_EXPANSION_REDUCTION_RATIO * sizeof(data_t));

        if (tmp == NULL) {
            fprintf(stderr, "Function %s: memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        } else {
            s->data = tmp;
            s->capacity = s->capacity * STACK_EXPANSION_REDUCTION_RATIO;
        }
    }

    s->data[s->size] = value;
    s->size++;
}

data_t StackPop(struct Stack *s) {
    ASSERT(!StackOK(s));

    if (s->size <= 0) {
        fprintf(stderr, "Function %s: Cannot pop element in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return ERROR_VALUE;
    }

    if (s->size * STACK_EXPANSION_REDUCTION_RATIO <= s->capacity) {
        data_t *tmp = realloc(s->data, s->capacity / STACK_EXPANSION_REDUCTION_RATIO * sizeof(data_t));

        if (tmp == NULL) {
            fprintf(stderr, "Function %s: Memory allocation error in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);

            return ERROR_VALUE;
        } else {
            s->data = tmp;
            s->capacity = s->capacity / STACK_EXPANSION_REDUCTION_RATIO;
        }
    }

    data_t value = s->data[--s->size];
    s->data[s->size] = STACK_POISON;

    return value;
}

data_t StackPeek(const struct Stack *s) {
    ASSERT(!StackOK(s));

    if (s->size > 0) {
        return s->data[s->size - 1];
    } else {
        fprintf(stderr, "Function %s: Cannot peek element in File %s, line %d\n", __FUNCTION__, __FILE__,
                __LINE__);
        return ERROR_VALUE;
    }
}

unsigned int StackIsEmpty(const struct Stack *s) {
    ASSERT(!StackOK(s));

    if (s->size == 0) {
        return STACK_EMRTY;
    } else {
        return STACK_NOT_EMPTY;
    }
}

unsigned int StackSize(const struct Stack *s) {
    ASSERT(!StackOK(s));

    return s->size;
}

void StackClear(struct Stack *s) {
    ASSERT(!StackOK(s));

    memset(s->data, STACK_POISON, sizeof(*(s->data)) * s->size);
    s->data = realloc(s->data, STACK_CTOR_SIZE);
    s->capacity = STACK_CTOR_SIZE;
    s->size = 0;
}

void StackDtor(struct Stack *s) {
    ASSERT(!StackOK(s));

    memset(s->data, STACK_POISON, sizeof(data_t) * s->size);
    free(s->data);
    s->data = NULL;

    s->size = 0;
    s->capacity = 0;
}

data_t *StackToArray(const struct Stack *s, data_t *arr) {
    ASSERT(!StackOK(s));

    if (s == NULL) {
        fprintf(stderr, "Function %s: Stack NULL pointer in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return NULL;
    }

    if (arr == NULL) {
        fprintf(stderr, "Function %s: Array NULL pointer in File %s, line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return NULL;
    }

    arr = (data_t *) calloc(s->size, sizeof(data_t));
    memcpy(arr, s->data, sizeof(data_t) * s->size);
    return arr;
}