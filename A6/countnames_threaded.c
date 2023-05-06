/**
 * Description: This counts the occurrences of strings in the inputted text files. Each file is assigned to a thread. They read the strings line by line.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 5/5/2023
 * Creation date: 5/1/2023
 **/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ConcurrentHashTable.h"

// Define how many threads
#define THREAD_NUM 2

// Define the number of slots for the hash table
#define HASH_SIZE 101

// Define a read length limit
#define MAX_LENGTH 31

// String resources
#define OPEN_FILE_MESSAGE "opened %s\n"


pthread_t threads[THREAD_NUM]; // array of threads

// logIndex is simply the log message number. We need the lock as multiple threads could concurrently print log messages and modify this value.
int logIndex = 0;
pthread_mutex_t logIndexLock = PTHREAD_MUTEX_INITIALIZER;

static ConcurrentHashTable *nameCount; // using a hashtable to store the names and counts


// Store the thread that created this THREADDATA
typedef struct THREADDATA_STRUCT {
    pthread_t creator;
} THREADDATA;

/**
 * *p is global to all threads
 * Despite threads sharing the heap, THREADDATA should be malloc'd and freed by the same thread
 * Lock is needed as concurrent malloc and free operations are prone to race conditions
 */
THREADDATA *p = NULL;
pthread_mutex_t threadDataLock = PTHREAD_MUTEX_INITIALIZER;


/**
 * This function prints passed in log messages. It prefixes all messages with a header - includes a message number, thread ID, PID ID, and current timestamp.
 * Assumption: The variable arguments match with the formatMessage.
 * Input parameters: a format string, and any other arguments
 * Returns: N/A
 **/
void printLog(char *formatMessage, ...){

    // Get current time
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    // Critical section: modify value of logIndex
    pthread_mutex_lock(&logIndexLock);
    ++logIndex;
    pthread_mutex_unlock(&logIndexLock);

    /**
     * Typical header would look like:
        * Logindex 3: thread 1234567898764321, PID 1234, 05/03/2023 09:48:37 :

     * Month has +1 because tm_mon is starting from 0.
     * Year has +1900 because tm_year gives the years since 1900
     */
    printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d : ", logIndex, pthread_self(), getpid(), local->tm_mon + 1, local->tm_mday, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec);

    // Print and append the passed log message
    va_list args;

    va_start(args, formatMessage);

    vprintf(formatMessage, args);

    va_end(args);
}


/**
 * This function allocates memory for THREADDATA to pointer *p
 * Assumption: *p is global
 * Input parameters: N/A
 * Returns: N/A
 **/
void createThreadData(){
    pthread_t me = pthread_self();

    // Critical section: Concurrent malloc from different threads may cause memory leaks
    pthread_mutex_lock(&threadDataLock);

    if (p == NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator=pthread_self();
    }

    pthread_mutex_unlock(&threadDataLock);

    printLog( (p && p->creator == me) ? "This is thread %ld and I created THREADDATA %p\n" : "This is thread %ld and I can access the THREADDATA %p\n", me, p);
}


/**
 * This function frees the memory associated with THREADDATA by pointer *p
 * Assumption: *p is global
 * Input parameters: N/A
 * Returns: N/A
 **/
void freeThreadData(){

    pthread_t me = pthread_self();

    // Critical section: Multiple frees on same pointer have undefined behavior. The multiple frees are even more likely to occur with concurrent threads.
    pthread_mutex_lock(&threadDataLock);

    if (p && p->creator == me) {
        printLog("This is thread %ld and I delete THREADDATA\n", me);

        free(p);
        p = NULL;
    }
    else
        printLog("This is thread %ld and I can access the THREADDATA\n",me);

    pthread_mutex_unlock(&threadDataLock);
}


/**
 * This function takes in a file and counts occurrences of newline-separated strings.
 * Assumption: N/A (fileName should refer to a file, but error can be handled)
 * Input parameters: a void pointer
 * Returns: NULL
 **/
void *readFile(void *fileName) {

    createThreadData();

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        // could not open/find file
        printf("range: cannot open file %s\n", (char *) fileName);
        return NULL;
    }
    // Here, fp has opened a valid file fileName and points to it

    printLog(OPEN_FILE_MESSAGE, fileName, NULL);


    // BEGIN COUNTING
    char lineInput[MAX_LENGTH];

    // Terminate if passed empty file
    if (fgets(lineInput, MAX_LENGTH, fp) == NULL) {
        fprintf(stderr, "File %s is empty\n", (char *) fileName);
        return NULL;
    }

    int lineCounter = 0;

    do {
        ++lineCounter;

        // Skip empty lines and warn line is empty
        if (strcmp(lineInput,"\n") == 0) {
            fprintf(stderr,"Warning - file %s line %d is empty\n", (char *) fileName, lineCounter);
            continue;
        }

        lineInput[strlen(lineInput) - 1] = '\0'; // remove the newline character from fgets

        struct nlist *result = insert(nameCount, lineInput); // find if already exists, otherwise make it

        // Critical section: modify the count value
        pthread_mutex_lock(&result->nodeLock);
        ++result->count;
        pthread_mutex_unlock(&result->nodeLock);

    } while(fgets(lineInput, MAX_LENGTH, fp) != NULL); // read until EOF

    // END COUNTING COMPLETED

    fclose(fp);

    freeThreadData();

    return NULL;
}


int main(int argc, char *argv[]) {

    // For simplicity, one file for each thread
    if (argc != THREAD_NUM + 1){
        fprintf(stderr, "Accepting only %d files\n", THREAD_NUM);
        return 1;
    }

    // Initialize the hash table
    nameCount = (ConcurrentHashTable *) malloc(sizeof(ConcurrentHashTable));
    nameCount->size = HASH_SIZE;
    initializeHashTable(nameCount);

    printf("======================== Log Messages ========================\n");

    // Create threads
    for (int i = 0; i < THREAD_NUM; i++){
        printf("Create thread %d\n", i+1);
        pthread_create(threads+i, NULL, readFile, (void *) argv[i+1]);
    }

    // Wait for threads to complete
    for (int i = 0; i < THREAD_NUM; i++){
        pthread_join(threads[i], NULL);
        printf("%d/%d threads exited\n", i+1, THREAD_NUM);
    }

    printf("\n======================== Name Counts ========================\n");

    // Print the name and their counts
    printHashTable(nameCount);

    // Free the hash table and all associated memory
    freeHashTable(nameCount);

    return 0;
}
