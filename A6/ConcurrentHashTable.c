#include <string.h>
#include <stdlib.h>
#include "ConcurrentHashTable.h"
#include <pthread.h>
#include <stdio.h>

#define DEFAULT_HASH_SIZE 100

unsigned hash(char *s) {
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval;
}

unsigned tableHash(ConcurrentHashTable *hashtab, char *s) {
    return hash(s) % hashtab->size;
}

ConcurrentHashTable *initializeHashTable(ConcurrentHashTable *hashtab){

    if (hashtab == NULL){
        hashtab = (ConcurrentHashTable *) malloc(sizeof(ConcurrentHashTable));
        hashtab->size = DEFAULT_HASH_SIZE; // set as default value
    }

    int bucketNum = hashtab->size;

    hashtab->bucket = (struct nlist **) malloc(bucketNum * sizeof(struct nlist *));

    for (int i = 0; i < bucketNum; i++)
        hashtab->bucket[i] = NULL;

    return hashtab;
}


struct nlist *lookup(ConcurrentHashTable *hashtab, char *key) {

    struct nlist *np = hashtab->bucket[tableHash(hashtab, key)];

    // Traverse the linked list under the calculated hash value slot
    while (np){

        pthread_mutex_lock(&np->nodeLock);

        if (strcmp(key, np->name) == 0) {
            // found
            pthread_mutex_unlock(&np->nodeLock);
            return np;
        }

        pthread_mutex_unlock(&np->nodeLock);

        np = np->next;
    }

    return NULL; // not found
}


struct nlist *insert(ConcurrentHashTable *hashtab, char *name) {

    struct nlist *np;

    if ((np = lookup(hashtab, name)) == NULL) { // couldn't find an existing node; we have to create it

        np = (struct nlist *) malloc(sizeof(struct nlist));
        if (np == NULL) return NULL;

        np->name = strdup(name);
        np->count = 0;

        pthread_mutex_init(&np->nodeLock, NULL);

        struct nlist **slot = hashtab->bucket + tableHash(hashtab, name);

        // We insert at the head of the linked list
        // The most recently created/inserted will be the quickest to access
        np->next = *slot;

        if (*slot == NULL)
            *slot = np;

        else {
            pthread_mutex_t lock = (*slot)->nodeLock;

            pthread_mutex_lock(&lock);

            *slot = np;

            pthread_mutex_unlock(&lock);
        }
    }

    return np;
}

void freeNList(struct nlist *node){
    free(node->name);
    free(node);
}

void freeHashTable(ConcurrentHashTable *hashtab){

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

void printNList(struct nlist *node){
    printf("%s: %d\n", node->name, node->count);
}

void printHashTable(ConcurrentHashTable *hashtab){

    for (int i = 0; i < hashtab->size; i++) {
        struct nlist *current = hashtab->bucket[i];

        while (current){
            printNList(current);
            current = current->next;
        }
    }
}
