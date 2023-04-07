#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_tracer.h"

#define GROWTH_FACTOR 1.61

struct LinkedListNode {
    char *str;
    int index;
    struct LinkedListNode *next;
};

typedef struct LinkedListNode LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head;
} LinkedList;



LinkedList *createLinkedList() {
    PUSH_TRACE("createLinkedList");

    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = malloc(sizeof(LinkedListNode));

    POP_TRACE();
    return list;
}

LinkedListNode *createNewNode(char *input, int val){

    PUSH_TRACE("createNewNode");

    LinkedListNode *new = malloc(sizeof(LinkedListNode));
    new->str = input;
    new->index = val;

    POP_TRACE();

    return new;
}

void PrintNodes(LinkedList *list) {
    PUSH_TRACE("PrintNodes");

    LinkedListNode *iterate = list->head;

    do {
        printf("%d and %s", iterate->index, iterate->str);
    } while ((iterate = iterate->next));

    POP_TRACE();

}

void freeLinkedList(LinkedList *list) {
    PUSH_TRACE("freeLinkedList");

    LinkedListNode *iterate = list->head;
    LinkedListNode *next;
    while (iterate) {
        next = iterate->next;
        free(iterate);
        iterate = next;
    }

    free(list);

    POP_TRACE();
}


int main() {

    PUSH_TRACE("main");

    char *input = NULL;
    size_t length = 0;

    if (getline(&input, &length, stdin) < 0) {
        free(input);
        goto EXIT;
    }

    int items = 10;
    char **array = malloc(items * sizeof(char*));

    LinkedList *list = createLinkedList();

    array[0] = strdup(input);
    list->head->str = array[0];
    list->head->index = 1;

    int count = 1;

    LinkedListNode *previous = list->head;

    while (getline(&input, &length, stdin) >= 0) {

        if (count >= items)
            array = realloc(array, (items *= GROWTH_FACTOR) * sizeof(char*));

        array[count] = strdup(input);

        previous->next = createNewNode(array[count], count + 1);
        ++count;

        previous = previous->next;

    }

    free(input);
    input = NULL;

    previous->next = NULL;

    previous = NULL;

    PrintNodes(list);

    freeLinkedList(list);

    for (int i = 0; i < count; i++)
        free(array[i]);

    free(array);

    // cleanup
    EXIT:
    POP_TRACE();
    terminationCleanup();

    return 0;
}
