#ifndef LIST_H
#define LIST_H

typedef int data_t; //info type

#include <zconf.h>

#define LIST_ASSERT(check){\
    int res = check;\
    if ((res)){\
    fprintf(stderr, "Function %s: %s in File %s, line %d\n",__FUNCTION__ , #check, __FILE__, __LINE__);\
    }\
    switch (res){\
        case OK: break;\
        case LIST_NULLPTR: fprintf(stderr, "List: LIST_NULLPTR\n"); break;\
        case LIST_MEM_ALLOC_ERROR: fprintf(stderr, "List: LIST_MEM_ALLOC_ERROR\n"); break;\
        case LIST_NEGATIVE_COUNTER: fprintf(stderr, "List: LIST_NEGATIVE_COUNTER\n"); break;\
        case LIST_HEAD_DAMAGED: fprintf(stderr, "List: LIST_HEAD_DAMAGED\n"); break;\
        case LIST_TAIL_DAMAGED: fprintf(stderr, "List: LIST_TAIL_DAMAGED\n"); break;\
        case LIST_POINTER_CHAIN_DAMAGED: fprintf(stderr, "List: LIST_POINTER_CHAIN_DAMAGED\n"); break;\
        case LIST_DELETED_ELEMENT: fprintf(stderr, "List: LIST_DELETED_ELEMENT\n"); break;\
        case LIST_WRONG_COUNTER: fprintf(stderr, "List: LIST_WRONG_COUNTER\n"); break;\
        case LIST_EMPTY: fprintf(stderr, "List: Cannot pop element\n"); break;\
        case LIST_INDEX_OUT_OF_BOARDERS: fprintf(stderr, "List: index is out of boarders\n"); break;\
        default: fprintf(stderr, "List: UNKNOWN ERROR\n");\
    }\
}

struct ListElem {
    data_t info;
    struct ListElem *next;
    struct ListElem *prev;
};

struct List {
    struct ListElem *head;
    struct ListElem *tail;
    unsigned int count;
};

enum listError {
    OK,
    LIST_NULLPTR,
    LIST_MEM_ALLOC_ERROR,
    LIST_NEGATIVE_COUNTER,
    LIST_HEAD_DAMAGED,
    LIST_TAIL_DAMAGED,
    LIST_POINTER_CHAIN_DAMAGED,
    LIST_DELETED_ELEMENT,
    LIST_WRONG_COUNTER,
    LIST_EMPTY,
    LIST_INDEX_OUT_OF_BOARDERS
};

static struct ListElem *listElemCtor(data_t value); // count>=1

static int listElemDtor(struct ListElem *listElem);

int listCtor(struct List *list);

int listOk(const struct List *list);

int listDtor(struct List *list);

void listDump(const struct List *list);

int listPushFront(struct List *list, data_t element);

int listPushBack(struct List *list, data_t value);

int listInsert(struct List *list, unsigned int index, data_t value);

data_t listPopFront(struct List *list);

data_t listPopBack(struct List *list);

data_t listGetFront(const struct List *list);

data_t listGetBack(const struct List *list);

data_t listGet(const struct List *list, unsigned int index);

int listMerge(struct List *list1, struct List *list2);

int listClear(struct List *list);

int listIsEmpty(const struct List *list);

#endif //LIST_H