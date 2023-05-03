#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ConcurrentHashTable.h"

#define THREAD_NUM 2
#define HASH_SIZE 101
#define MAX_LENGTH 31


// String resources
#define OPEN_FILE_MESSAGE "opened %s"


pthread_t threads[THREAD_NUM];

pthread_mutex_t logIndexLock = PTHREAD_MUTEX_INITIALIZER;
int logIndex = 0;

static ConcurrentHashTable *nameCount;

void printLog(char *formatMessage, ...){

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    pthread_mutex_lock(&logIndexLock);
    ++logIndex;
    pthread_mutex_unlock(&logIndexLock);

    printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d : ", logIndex, pthread_self(), getpid(), local->tm_mon + 1, local->tm_mday, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec);

    va_list args;

    va_start(args, formatMessage);

    vprintf(formatMessage, args);

    va_end(args);

    printf("\n");
}


void *readFile(void *fileName) {

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("range: cannot open file %s\n", (char *) fileName);
        return NULL;
    }

    printLog(OPEN_FILE_MESSAGE, fileName, NULL);

    char lineInput[MAX_LENGTH];

    // terminate if passed empty file
    if (fgets(lineInput, MAX_LENGTH, fp) == NULL) {
        fprintf(stderr, "File %s is empty\n", (char *) fileName);
        return NULL;
    }

    int lineCounter = 0;

    do {
        ++lineCounter;

        // skip empty lines and warn line is empty
        if (strcmp(lineInput,"\n") == 0) {
            fprintf(stderr,"Warning - file %s line %d is empty\n", (char *) fileName, lineCounter);
            continue;
        }

        // remove the newline character from fgets
        lineInput[strlen(lineInput) - 1] = '\0';


        struct nlist *result = insert(nameCount, lineInput);

        pthread_mutex_lock(&result->nodeLock);

        ++result->count;

        pthread_mutex_unlock(&result->nodeLock);

    } while(fgets(lineInput, MAX_LENGTH, fp) != NULL); // EOF

    fclose(fp);

    return NULL;
}

int main(int argc, char *argv[]) {


    if (argc != THREAD_NUM + 1){
        fprintf(stderr, "Accepting only %d files\n", THREAD_NUM);
        return 1;
    }

    nameCount = (ConcurrentHashTable *) malloc(sizeof(ConcurrentHashTable));
    nameCount->size = HASH_SIZE;
    initializeHashTable(nameCount);

    printf("======================== Log Messages ========================\n");


    for (int i = 0; i < THREAD_NUM; i++){
        printf("Create thread %d\n", i+1);
        pthread_create(threads+i, NULL, readFile, (void *) argv[i+1]);
    }

    for (int i = 0; i < THREAD_NUM; i++){
        pthread_join(threads[i], (void **) NULL);
        printf("%d/%d threads exited\n", i+1, THREAD_NUM);
    }

    printf("\n======================== Name Counts ========================\n");

    printHashTable(nameCount);

    freeHashTable(nameCount);

    return 0;
}
