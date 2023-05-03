#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#define THREAD_NUM 2

#define OPEN_FILE_MESSAGE "opened %s"


pthread_t threads[THREAD_NUM];

pthread_mutex_t logIndexLock = PTHREAD_MUTEX_INITIALIZER;
int logIndex = 0;

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

    printLog(OPEN_FILE_MESSAGE, fileName, NULL);
    return NULL;
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < THREAD_NUM; i++){
        pthread_create(threads+i, NULL, readFile, (void *) argv[i+1]);
    }

    for (int i = 0; i < THREAD_NUM; i++){
        pthread_join(threads[i], (void **) NULL);
    }

    return 0;
}
