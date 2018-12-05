#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

const data_t INFO_POISON = INT_MAX;

static struct ListElem *listElemCtor(data_t value) {
    struct ListElem *elem = (struct ListElem *) calloc(1, sizeof(*elem));

    if (elem != NULL) {
        elem->info = value;
        elem->next = NULL;
        elem->prev = NULL;
    } else {
        LIST_ASSERT(LIST_MEM_ALLOC_ERROR);

        return NULL;
    }

    return elem;
}

static int listElemDtor(struct ListElem *listElem) {
    listElem->info = INFO_POISON;
    listElem->next = NULL;
    listElem->prev = NULL;
    free(listElem);

    return 0;
}

int listCtor(struct List *list) {
    if (list != NULL) {
        list->head = NULL;
        list->tail = NULL;
        list->count = 0;
    } else {
        LIST_ASSERT(LIST_NULLPTR);

        return LIST_NULLPTR;
    }

    return 0;
}

int listDtor(struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count != 0) {
        struct ListElem *elem = list->head;

        for (int i = 1; i < list->count; i++) {
            elem = elem->next;
            listElemDtor(elem->prev);
        }
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    return 0;
}

int listOk(const struct List *list) {
    if (list == NULL) {
        return LIST_NULLPTR;
    }

    if (!((list->count == 0) && (list->head == NULL) && (list->tail == NULL))) {

        if (list->count == UINT_MAX) {
            return LIST_NEGATIVE_COUNTER;
        }

        if ((list->head == NULL) && (list->count != 0)) {
            return LIST_HEAD_DAMAGED;
        }

        if ((list->tail == NULL) && (list->count != 0)) {
            return LIST_TAIL_DAMAGED;
        }

        struct ListElem *elem1 = list->head;
        struct ListElem *elem2 = list->tail;

        for (int i = 0; i < list->count - 1; i++) {
            elem1 = elem1->next;
            elem2 = elem2->prev;

            if (elem1 == NULL || elem2 == NULL) {
                return LIST_POINTER_CHAIN_DAMAGED;
            }

            if ((elem1->info == INFO_POISON) || (elem2->info == INFO_POISON)) {
                return LIST_DELETED_ELEMENT;
            }
        }

        if ((elem1 != list->tail) || (elem2 != list->head)) {
            return LIST_POINTER_CHAIN_DAMAGED;
        }

        elem1 = list->head;

        int cnt = 1;
        while (elem1->next != NULL) {
            elem1 = elem1->next;
            cnt++;
        }

        if (cnt != list->count) {
            return LIST_WRONG_COUNTER;
        }
    }

    return OK;
}

void listDump(const struct List *list) {
    FILE *input = fopen("listDump.txt", "w");
    fprintf(input, "List Dump {\n");

    struct ListElem *elem = list->head;
    for (int i = 0; i < list->count; i++) {
        fprintf(input, "%d) %d: [0x%X]\n", i, elem->info, elem);
        elem = elem->next;
    }

    fprintf(input, "}\n");
    fclose(input);
}

int listPushFront(struct List *list, data_t value) {
    LIST_ASSERT(listOk(list));

    struct ListElem *elem = listElemCtor(value);
    elem->next = list->head;

    if (list->head != NULL) {
        (list->head)->prev = elem;
    }

    if (list->count == 0) {
        list->tail = elem;
    }

    list->head = elem;
    list->count++;

    return 0;
}

int listPushBack(struct List *list, data_t value) {
    LIST_ASSERT(listOk(list));

    struct ListElem *elem = listElemCtor(value);
    elem->prev = list->tail;

    if (list->head != NULL) {
        (list->tail)->next = elem;
    }

    if (list->count == 0) {
        list->head = elem;
    }

    list->tail = elem;
    list->count++;

    return 0;
}

int listInsert(struct List *list, unsigned int index, data_t value) {
    LIST_ASSERT(listOk(list));

    if ((index >= 0) && (index <= list->count)) {
        if (index == 0) {
            listPushFront(list, value);
        } else if (index == list->count) {
            listPushBack(list, value);
        } else {
            struct ListElem *cur = list->head;
            struct ListElem *elem = listElemCtor(value);

            for (int i = 1; i < index; i++) {
                cur = cur->next;
            }

            elem->next = cur->next;
            elem->prev = cur;
            (elem->next)->prev = elem;
            cur->next = elem;
            list->count++;
        }

        return 0;
    } else {
        LIST_ASSERT(LIST_INDEX_OUT_OF_BOARDERS);

        return LIST_INDEX_OUT_OF_BOARDERS;
    }
}

data_t listPopFront(struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count > 0) {
        data_t value = (list->head)->info;

        if (list->count > 1) {
            list->head = list->head->next;
            listElemDtor(list->head->prev);
            list->head->prev = NULL;
        } else {
            listElemDtor(list->head);
            list->head = NULL;
            list->tail = NULL;
        }

        list->count--;

        return value;
    } else {
        LIST_ASSERT(LIST_EMPTY);

        return INFO_POISON;
    }
}

data_t listPopBack(struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count > 0) {
        data_t value = (list->tail)->info;

        if (list->count > 1) {
            list->tail = list->tail->prev;
            listElemDtor(list->tail->next);
            list->tail->next = NULL;
        } else {
            listElemDtor(list->tail);
            list->head = NULL;
            list->tail = NULL;
        }

        list->count--;

        return value;
    } else {
        LIST_ASSERT(LIST_EMPTY);

        return INFO_POISON;
    }
}

data_t listGetFront(const struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count > 0) {
        return list->head->info;
    } else {
        return INFO_POISON;
    }
}

data_t listGetBack(const struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count > 0) {
        return list->tail->info;
    } else {
        return INFO_POISON;
    }
}

data_t listGet(const struct List *list, unsigned int index) {
    LIST_ASSERT(listOk(list));

    if ((list->count > 0) && (index < list->count)) {
        struct ListElem *elem = list->head;

        for (int i = 1; i <= index; i++) {
            elem = elem->next;
        }

        return elem->info;
    } else {
        LIST_ASSERT(LIST_INDEX_OUT_OF_BOARDERS);

        return INFO_POISON;
    }
}

int listMerge(struct List *list1, struct List *list2) {
    LIST_ASSERT(listOk(list1) || listOk(list2));

    if (list2->count != 0) {
        (list1->tail)->next = (list2->head);
        (list2->head)->prev = list1->tail;
        list1->tail = list2->tail;
        list1->count = list1->count + list2->count;
    }
    return 0;
}

int listClear(struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count != 0) {
        struct ListElem *cur = (list->head);

        for (int i = 1; i < list->count; i++) {
            cur = cur->next;
            listElemDtor(cur->prev);
        }

        listElemDtor(cur);
        list->head = NULL;
        list->tail = NULL;

        list->count = 0;
    }

    return 0;
}

int listIsEmpty(const struct List *list) {
    LIST_ASSERT(listOk(list));

    if (list->count > 0) {
        return 0;
    } else if (list->count == 0) {
        return 1;
    }
}