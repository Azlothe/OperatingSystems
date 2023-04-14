/**
 * Description: This module reads in and stores contacts with their names and phone numbers. Functions for ContactNode are defined here.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/13/2023
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

    return new;
}

char *GetName(ContactNode *contact){
    return contact->contactName;
}

char *GetPhoneNumber(ContactNode *contact){
    return contact->contactPhoneNumber;
}

void InsertAfter(ContactNode *previous, ContactNode *next){
    previous->nextNodePtr = next;
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
