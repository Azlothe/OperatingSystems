#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){

    int parentID = getpid();

    fork();
    fork();

    printf("hello world from PID %d!\n", getpid());

    return 0;
}
