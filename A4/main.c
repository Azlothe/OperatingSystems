/**
 * Description: This reads in inputs and stores them into a linked list. Memory usage is traced.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/9/2023
 * Creation date: 3/31/2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_tracer.h"

#define GROWTH_FACTOR 1.61 // Golden ratio factor to effectively reuse old memory for dynamic structures

struct LinkedListNode {
    char *str;
    int index;
    struct LinkedListNode *next;
};

typedef struct LinkedListNode LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head; // For this program, we only need the head. We do not have a strict need to store the tail at all times.
} LinkedList;


/**
 * This function creates and initializes a LinkedList and the head LinkedListNode.
 * Assumption: N/A
 * Input parameters: N/A
 * Returns: Pointer to created LinkedList
 **/
LinkedList *createLinkedList() {
    PUSH_TRACE("createLinkedList");

    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = malloc(sizeof(LinkedListNode));

    POP_TRACE();
    return list;
}

/**
 * This function creates a new LinkedListNode
 * Assumption: Any LinkedListNode just stores a string and an integer
 * Input parameters: An input string and an integer value
 * Returns: Pointer to created LinkedListNode
 **/
LinkedListNode *createNewNode(char *input, int val){

    PUSH_TRACE("createNewNode");

    LinkedListNode *new = malloc(sizeof(LinkedListNode));
    new->str = input;
    new->index = val;

    POP_TRACE();

    return new;
}

/**
 * This function prints all LinkedListNodes in the inputted LinkedList
 * Assumption: N/A
 * Input parameters: Pointer to LinkedList
 * Returns: Pointer to created LinkedListNode
 **/
void PrintNodes(LinkedList *list) {
    PUSH_TRACE("PrintNodes");

    LinkedListNode *iterate = list->head;

    do {
        printf("Line %d:\t%s", iterate->index, iterate->str);
    } while ((iterate = iterate->next));

    POP_TRACE();

}

/**
 * This function frees the inputted LinkedList and all of its LinkedListNodes
 * Assumption: N/A
 * Input parameters: Pointer to LinkedList
 * Returns: N/A
 **/
void freeLinkedList(LinkedList *list) {
    PUSH_TRACE("freeLinkedList");

    LinkedListNode *iterate = list->head;
    LinkedListNode *next; // *next holds the next node to move to after freeing iterate. iterate should not access its next after being freed.
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
        // First input was EOF, so we do not allocate memory for the LinkedList or the array. We go to cleanup code
        free(input);
        goto EXIT;
    }

    // Getting to this line means we successfully read the first input and was valid

    int allocatedItems = 10;
    char **array = malloc(allocatedItems * sizeof(char*));

    LinkedList *list = createLinkedList();

    // Initialize the values of the LinkedList head
    array[0] = strdup(input);
    list->head->str = array[0];
    list->head->index = 1;

    int count = 1; // array[count] stores the string. count + 1 is the line index

    LinkedListNode *previous = list->head; // Store the previous LinkedListNode to easily attach the next node in the list as previous->next

    while (getline(&input, &length, stdin) >= 0) {

        if (count >= allocatedItems) // Reallocate if we are about to go over our current allocated amount
            array = realloc(array, (allocatedItems *= GROWTH_FACTOR) * sizeof(char*));

        array[count] = strdup(input); // Duplicate the inputted string into the array

        previous->next = createNewNode(array[count], count + 1); // Order of parameters is undefined. ++count will be placed on its own.
        ++count;

        previous = previous->next;

    }

    free(input);
    input = NULL; // Pointer will no longer be used, so set to NULL

    previous->next = NULL; // Designate the LinkedListNode that previous points to as the tail by making its next node as NULL
    previous = NULL; // Pointer will no longer be used, so set to NULL

    PrintNodes(list);

    freeLinkedList(list);

    for (int i = 0; i < count; i++)
        free(array[i]);

    free(array);

    // Cleanup "main", "global" identifiers, and other termination cleanup.
    EXIT:
    POP_TRACE();
    terminationCleanup();

    return 0;
}
