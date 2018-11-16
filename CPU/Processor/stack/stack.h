#include <limits.h>

typedef int data_t; //CHANGE IN/OUT INSTRUCTIONS FOR SPECIFIC TYPE

//#define STACKSAFE

#ifndef STACK_H
#define STACK_H

#define TEST
#ifdef TEST

#define UNITTEST(what, ref, file){\
    if ((what) != (ref)){\
        fprintf(file, "FAILED: %s = %lf, expected %lf\n", #what, (what), (ref));\
   } else {\
        fprintf(file, "PASSED\n");\
   }\
}
#else
#define ASSERT
#define UNITTEST
#endif //UNITTEST

#ifdef STACKSAFE
/*!
 * @brief Stack structure, has data_t array, uint size and uint capacity with hash sum
 *
 */

struct Stack {
    data_t *data;
    unsigned int size;
    unsigned int capacity;
    unsigned int hash;
};
#else

/*!
 * @brief Stack structure, has data_t array, uint size and uint capacity with hash sum
 */

struct Stack {
    data_t *data;
    unsigned int size;
    unsigned int capacity;
};
#endif //STACKSAFE

/*!
 * @brief Push element to stack, reallocates (expand) memory if it is needed
 * @param s stack pointer
 * @param value pushing element
 */
void StackPush(struct Stack *s, data_t value);

/*!
 * @brief Pop element from stack, , reallocates (reduce) memory if it is needed
 * @param s stack pointer
 * @return popped element
 */

data_t StackPop(struct Stack *s);

/*!
 * @brief Peek element from stack (without removing)
 * @param s stack pointer
 * @return peeked element
 */

data_t StackPeek(const struct Stack *s);

/*!
 * @brief Checks if the stack is empty
 * @param s stack pointer
 * @return 1 if stack is empty, 0 if is not empty
 */

unsigned int StackIsEmpty(const struct Stack *s);

/*!
 * @brief Stack size
 * @param s stack pointer
 * @return size of s stack
 */

unsigned int StackSize(const struct Stack *s);

/*!
 * @brief Clears the stack without destructing it
 * @param s stack pointer
 */

void StackClear(struct Stack *s);

/*!
 * @brief Destructs the stack, frees the memory
 * @param s stack pointer
 */

void StackDtor(struct Stack *s);

/*!
 * @brief Constructs the stack, allocates memory
 * @param s stack pointer
 */

void StackCtor(struct Stack *s);

/*!
 * @brief Checks the stack's conditions
 * @param s stack pointer
 * @return the code of error (errorNum enum), 0 if everything ok
 */

enum errorNum StackOK(const struct Stack *s);

/*!
 * @brief Copies the elements from stack to array
 * @param s stack pointer
 * @param arr array pointer
 * @return rewritten array
 */

data_t *StackToArray(const struct Stack *s);

/*!
 * @brief frees allocated array (from StackToArr) memory
 * @param arr array from StackToArray
 */

void StackArrayDtor(data_t *arr);

/*!
 * @brief Information about stack
 * @param f output file
 * @param s stack pointer
 */

void StackDump(FILE *f, struct Stack *s);

#endif //STACK_H