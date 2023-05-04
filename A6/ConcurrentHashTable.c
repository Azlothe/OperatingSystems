/**
 * Description: This uses mutex locking to handle concurrent accesses. Sequentiality is minimized by locking nodes, not entire slots or the table.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 5/3/2023
 * Creation date: 5/1/2023
 **/

#include <string.h>
#include <stdlib.h>
#include "ConcurrentHashTable.h"
#include <pthread.h>
#include <stdio.h>

// Set preferred default size of hash table
#define DEFAULT_HASH_SIZE 100


// BEGIN struct nlist FUNCTION IMPLEMENTATION

/**
 * This functions frees the memory of a nlist struct
 * Assumption: *node points to allocated memory. If the passed in pointer points to NULL, it will not be freed and will return.
 * Input parameters: a pointer to nlist
 * Returns: N/A
 * Note: Specific implementation will vary on the nlist struct definition. Make necessary changes if needed.
 **/
void freeNList(struct nlist *node){
    if (node == NULL) return;

    free(node->name);
    free(node);
}

/**
 * This function prints out relevant information of the inputted nlist
 * Assumption: *node points to allocated memory. If the passed in pointer points to NULL, it will not be freed and will return.
 * Input parameters: a pointer to nlist
 * Returns: N/A
 * Note: Specific implementation will vary on the nlist struct definition. Make necessary changes and change format if needed.
 **/
void printNList(struct nlist *node){
    if (node == NULL) return;

    printf("%s: %d\n", node->name, node->count);
}

// END struct nlist FUNCTION IMPLEMENTATION



/**
 * This is a hash function calculated using string ASCII value
 * Assumption: s is NULL-terminated
 * Input parameters: a string
 * Returns: an unsigned integer
 **/
unsigned hash(char *s) {
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval;
}


/**
 * This function takes a computed hash and limits it within the size bounds of the given hash table
 * Assumption:
    * s is NULL-terminated
    * the size of hashtab is defined
 * Input parameters: a pointer to ConcurrentHashTable, a string
 * Returns: an unsigned integer
 **/
unsigned tableHash(ConcurrentHashTable *hashtab, char *s) {
    return hash(s) % hashtab->size;
}


/**
 * This function initializes and allocates all associated memory for a ConcurrentHashTable
 * Assumptions:
    * If memory for *hashtab had been allocated, the size should also be set by the user.
    * If no memory has been allocated where *hashtab points to NULL, default settings apply.
 * Input parameters: a pointer to ConcurrentHashTable
 * Returns: a pointer to ConcurrentHashTable (may be the same if input not NULL or otherwise new address)
 **/
ConcurrentHashTable *initializeHashTable(ConcurrentHashTable *hashtab){

    if (hashtab == NULL){
        // Memory has not been allocated, so we allocate it and set the size to the default

        hashtab = (ConcurrentHashTable *) malloc(sizeof(ConcurrentHashTable));
        hashtab->size = DEFAULT_HASH_SIZE; // set as default value
    }

    int bucketNum = hashtab->size;

    // Allocate memory for all the buckets
    hashtab->bucket = (struct nlist **) malloc(bucketNum * sizeof(struct nlist *));

    // Set all bucket pointers to NULL. As we insert, the tail of each linked list will be NULL
    for (int i = 0; i < bucketNum; i++)
        hashtab->bucket[i] = NULL;

    return hashtab;
}


// TODO make key generic
/**
 * This function looks up an nlist in the hash table based on the key. The key here is based on the name
 * Assumptions:
    * The key can be changed
    * key is NULL-terminated
 * Input parameters: a pointer to ConcurrentHashTable, and a key string
 * Returns: a pointer to the found nlist or NULL if not found
 * Note: Function will return NULL if *hashtab points to NULL
 **/
struct nlist *lookup(ConcurrentHashTable *hashtab, char *key) {

    if (hashtab == NULL) return NULL;

    // Calculate hash value slot
    struct nlist *np = hashtab->bucket[tableHash(hashtab, key)];

    // Traverse the linked list under calculated slot
    while (np){

        // Critical section: reading mutable key. If we strictly assume key may never change after being inserted, we may reduce the critical section to only the head of each linked list. We only modify at the head of each list when we insert in the hash table slots.
        pthread_mutex_lock(&np->nodeLock);

        if (strcmp(key, np->name) == 0) {
            // found
            pthread_mutex_unlock(&np->nodeLock);
            return np;
        }

        pthread_mutex_unlock(&np->nodeLock);

        // No critical section because links between nodes will not change based on our hash table implementation.
        np = np->next;
    }

    return NULL; // not found
}


/**
 * This function finds a corresponding nlist if it already exists in the hash table. If it does not exist, the function creates and inserts it into the hash table.
 * Assumption: name is NULL-terminated
 * Input parameters: a pointer to ConcurrentHashTable, and a name string
 * Returns: a pointer to nlist (either found or newly created)
 * Note: Function will return NULL if *hashtab points to NULL
 **/
struct nlist *insert(ConcurrentHashTable *hashtab, char *name) {

    if (hashtab == NULL) return NULL;

    struct nlist *np;

    // The hash table must exist here as already previously checked, so condition must only pass when name is not found.
    if ((np = lookup(hashtab, name)) == NULL) {

        // couldn't find an existing node; we have to create it

        np = (struct nlist *) malloc(sizeof(struct nlist));
        if (np == NULL) return NULL;

        np->name = strdup(name);
        np->count = 0;

        pthread_mutex_init(&np->nodeLock, NULL);

        struct nlist **slot = hashtab->bucket + tableHash(hashtab, name); // equivalently the address of &bucket[tableHash]

        // We insert at the head of the linked list
        // The most recently created/inserted will be the quickest to access
        np->next = *slot;

        if (*slot == NULL) // Slot is empty; no need to lock
            *slot = np;

        else { // Slot is non-empty; we do need to lock

            pthread_mutex_t tempSlotLock = (*slot)->nodeLock; // *slot will be changed, so temporarily save the lock

            // Critical section: modifying the head of linked list in bucket.
            // Without lock, searches in other threads will incorrectly start from the next node as the head.
            pthread_mutex_lock(&tempSlotLock);
            *slot = np;
            pthread_mutex_unlock(&tempSlotLock);
        }
    }

    return np;
}


/**
 * This function frees all memory associated with the hash table
 * Assumptions:
    * *hashtab points to a ConcurrentHashTable. If it points to NULL, we assume there is nothing to free and return.
    * Function freeNList is defined and correctly frees all associated memory with a nlist struct. Not all memory may be successfully freed if implemented incorrectly
 * Input parameters: a pointer to ConcurrentHashTable
 * Returns: N/A
 **/
void freeHashTable(ConcurrentHashTable *hashtab){

    if (hashtab == NULL) return;

    // Run through all slots of the hash table
    for (int i = 0; i < hashtab->size; i++){

        struct nlist *current = hashtab->bucket[i];

        // Traverse the linked list within a single slot
        while (current){
            struct nlist *next = current->next; // save the next node to set to after the current one will be freed
            freeNList(current); // free the current node
            current = next;
        }
    }

    free(hashtab->bucket);
    free(hashtab);
}


/**
 * This function prints all nlist entries stored within the hash table
 * Assumption: Memory for *hashtab has been malloc'd. Nothing will be printed if hashtab points to NULL.
 * Input parameters: a pointer to ConcurrentHashTable
 * Returns: N/A
 * Note: Printing format should be defined within the function printNList. This function only serves to iterate through the hash table.
 **/
void printHashTable(ConcurrentHashTable *hashtab){

    if (hashtab == NULL) return;

    for (int i = 0; i < hashtab->size; i++) {
        struct nlist *current = hashtab->bucket[i];

        while (current){
            printNList(current);
            current = current->next;
        }
    }
}
