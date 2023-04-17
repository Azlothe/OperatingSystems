/**
 * Description: This module reads in and stores contacts with their names and phone numbers. Functions for ContactNode are defined here.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/17/2023
 * Creation date: 4/13/2023
 **/

#include <stdio.h>
#include "ContactNode.h"
#include <stdlib.h>
#include <string.h>


ContactNode *InitializeContactNode(char name[], char phoneNum[]) {
    ContactNode *new = malloc(sizeof(ContactNode) + strlen(name) + strlen(phoneNum));

    strcpy(new->contactName, name);
    strcpy(new->contactPhoneNumber, phoneNum);
    new->nextNodePtr = NULL;

    return new;
}

char *GetName(ContactNode *contact){
    return contact->contactName;
}

char *GetPhoneNumber(ContactNode *contact){
    return contact->contactPhoneNumber;
}

// Insert the *toInsert ContactNode after *previous.
void InsertAfter(ContactNode *previous, ContactNode *toInsert){

    if (previous == NULL) return; // Handle case where we try to insert where the previous refers to NULL

    ContactNode *next = previous->nextNodePtr;

    previous->nextNodePtr = toInsert;
    toInsert->nextNodePtr = next; // Can handle cases at the tail of the linked list where next = NULL and intermediate insertions

}

ContactNode *GetNext(ContactNode *current){
    return current->nextNodePtr;
}

void PrintContactNode(ContactNode *contact){
    printf("Name: %s\n"
        "Phone number: %s\n"
        , contact->contactName, contact->contactPhoneNumber
    );
}
