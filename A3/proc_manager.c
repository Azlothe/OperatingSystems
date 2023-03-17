#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <sys/wait.h>

#define maxLength 31

char* concatenate(int num, char *suffix){

    int places = log10(num);

    int concatLength = places + strlen(suffix) + 2;
    char concat[concatLength];

    for (int i = places; i >= 0; --i){
        concat[i] = (num % 10) + '0';
        num /= 10;
    }

    concat[places + 1] = 0;

    strcat(concat, suffix);

    char *ret = malloc(concatLength*sizeof(char));
    strcpy(ret, concat);
    return ret;

}


void split(char *output[], char input[], char delim) {

    int length = strlen(input);

    //     if (input[length - 1] == '\n')
    //         input[--length] = '\0';

    int strNum = 0;
    int written = 0;

    int i = 0;
    while (input[i] == delim)
        ++i;

    while (i < length) {
        if (!written) {
            output[strNum++] = &input[i];
            written = 1;
        }

        ++i;

        while(input[i] == delim) {
            input[i++] = '\0';
            written = 0;
        }
    }

    output[strNum] = 0;
}

int main(){

    char command[maxLength];

    int status;

    int count = 1;
    pid_t childPID;

    while (fgets(command, maxLength - 1, stdin) != NULL && (childPID = fork()) > 0)
        ++count;

    if (childPID == 0) {
        char *out = concatenate(getpid(), ".out");
        char *err = concatenate(getpid(), ".err");

        dup2(open(out, O_RDWR | O_CREAT | O_APPEND, 0777), STDOUT_FILENO);
        dup2(open(err, O_RDWR | O_CREAT | O_APPEND, 0777), STDERR_FILENO);

        free(out);
        free(err);

        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout);

        command[strlen(command) - 1] = '\0';

        char *parameters[strlen(command) + 1];
        split(parameters, command, ' ');

        execvp(parameters[0], parameters);
        perror(command);
        exit(2);
    }

    else if (childPID > 0) {
        while ((childPID = wait(&status)) > 0) {

            char *outFile = concatenate(childPID, ".out");

            dup2(open(outFile, O_WRONLY | O_APPEND, 0777), STDOUT_FILENO);

            printf("Finished child %d with pid of parent %d\n", childPID, getpid());
            fflush(stdout);

            char *errFile = concatenate(childPID, ".err");

            dup2(open(errFile, O_WRONLY | O_APPEND, 0777), STDERR_FILENO);

            if (WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));

            free(outFile);
            free(errFile);
        }
    }

    else
        fprintf(stderr, "error occurred in forking");

    return 0;
}
