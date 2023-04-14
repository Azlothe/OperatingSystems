/**
 * Description: This module reads in and stores contacts with their names and phone numbers. Contact definition here.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/13/2023
 * Creation date: 4/13/2023
 **/

#define PHONE_NUM_SIZE 16

typedef struct ContactNode {
    struct ContactNode *nextNodePtr;
    char contactPhoneNumber[PHONE_NUM_SIZE];
    char contactName[];
} ContactNode;

ContactNode *InitializeContactNode(char name[], char phoneNum[]);

char *GetName(ContactNode *contact);

char *GetPhoneNumber(ContactNode *contact);

void InsertAfter(ContactNode *previous, ContactNode *next);

ContactNode *GetNext(ContactNode *current);

void PrintContactNode(ContactNode *contact);
