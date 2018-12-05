#include <stdio.h>
#include "list.h"

int main() {
//    struct List list = {};
//    listCtor(&list);
//
//    for (int i = 0; i < 7; i++) {
//        listPushFront(&list, i);
//    }
//
//    listInsert(&list, 0, 42);
//    listInsert(&list, 6, 42);
//
//    for (int i = 0; i < 9; i++) {
//        printf("%d\n", listGet(&list, i));
//    }
//
//    printf("\n");
//    listDump(&list);
//    listClear(&list);
//
//    for (int i = 0; i < 2; i++) {
//        printf("%d\n", listPopBack(&list));
//    }
//
//    for (int i = 3; i < 5; i++) {
//        listPushBack(&list, i);
//    }
//
//    for (int i = 3; i < 5; i++) {
//        printf("%d\n", listPopFront(&list));
//    }
//
//    listDtor(&list);
//
//    struct List list1 = {};
//    struct List list2 = {};
//
//    listCtor(&list1);
//    listCtor(&list2);
//
//    listInsert(&list1, 0, 5);
//    listInsert(&list2, 0, 3);
//    listInsert(&list2, 1, 37);
//    listMerge(&list1, &list2);
//
//    for (int i = 0;i < 3; i++){
//        printf("%d\n", listPopBack(&list1));
//    }
//
//    listDtor(&list1);
    struct List list = {};
    listCtor(&list);
    listInsert(&list, 0, 55555);
    l
    printf("%d", listGet(&list, 0));
    listDtor(&list);

    return 0;
}