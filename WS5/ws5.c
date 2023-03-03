#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    const int totalProcesses = 4;

    pid_t childPID;

    for (int i = 1; i < totalProcesses; i *= 2)
        childPID = fork();

    printf("hello world from PID %d!\n", getpid());

    if (childPID > 0)
        for (int i = 1; i < 5; i++)
            wait(NULL);


    return 0;
}
