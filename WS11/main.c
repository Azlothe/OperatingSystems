/**
 * Description: This module reads in and stores contacts with their names and phone numbers.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/17/2023
 * Creation date: 4/13/2023
 **/

#include "ContactNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONTACTS 3

// Linked List definition for ContactNodes
typedef struct ContactList {
    ContactNode *head;
} ContactList;


// Read in a name and phone number. Initialize a new ContactNode with read input.
ContactNode *promptContact(int count) {
    printf("Person %d\n"
        "Enter name:\n"
        , count
    );

    char *name = NULL;
    size_t length = 0;

    getline(&name, &length, stdin);
    name[strlen(name) - 1] = '\0';

    printf("Enter phone number:\n");

    char phoneNumber[PHONE_NUM_SIZE];
    fgets(phoneNumber, PHONE_NUM_SIZE, stdin);
    phoneNumber[strlen(phoneNumber) - 1] = '\0';

    ContactNode *newContact = InitializeContactNode(name, phoneNumber);

    free(name);

    printf("Person %d: %s, %s\n\n", count, newContact->contactName, newContact->contactPhoneNumber);

    return newContact;

}

// Free linked list ContactList and its nodes
void freeList(ContactList *list){

    ContactNode *iterate = list->head;
    ContactNode *next;

    while (iterate) {
        next = iterate->nextNodePtr;
        free(iterate);
        iterate = next;
    }

    free(list);
}

// Print all contents of linked list ContactList
void printList(ContactList *list){

    printf("\nCONTACT LIST");

    ContactNode *iterate = list->head;

    while (iterate) {
        printf("\n");
        PrintContactNode(iterate);
        iterate = iterate->nextNodePtr;
    }
}

int main(){

    ContactList *list = malloc(sizeof(ContactList));

    int count = 1;

    list->head = promptContact(count++);


    ContactNode *previous = list->head;

    while (count <= CONTACTS) {
        previous->nextNodePtr = promptContact(count++);
        // InsertAfter(previous, promptContact(count++)); // no need to always call InsertAfter since we're only inserting at the tail and we keep track of it with *previous
        previous = previous->nextNodePtr;
    }

    previous->nextNodePtr = NULL;


    printList(list);

    freeList(list);


    return 0;
}
