/**
 * Description: This takes in commands (separated by a new line) and runs them in parallel. It creates a .out file for the output and a .err file for any error messages.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/20/2023
 * Creation date: 3/16/2023
 **/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <sys/wait.h>

#define maxLength 31
#define outputFileExtension ".out"
#define errorFileExtension ".err"

/**
 * This function concatenates an integer and a string as the suffix
 * Assumption: suffix is null terminated and a proper string
 * Input parameters: an integer and a suffix string
 * Returns: the concatenated string
 **/
char* concatenate(int num, char *suffix){

    int places = log10(num); // taking the log base 10 gives (number of digits - 1)

    int concatLength = places + strlen(suffix) + 2; // add +2 because +1 to account for the 1 digit not calculated by a log_base_10, and the other +1 for null terminating character at the end
    char concat[concatLength];

    for (int i = places; i >= 0; --i){
        concat[i] = (num % 10) + '0'; // obtain digit and add '0' = 48 to convert to corresponding character ascii code
        num /= 10; // move to next numerical place
    }

    concat[places + 1] = 0; // terminate num in char array, so strcat can directly attach the suffix at this termination

    strcat(concat, suffix);

    // store + copy to heap for preservation after function returns
    char *ret = (char *) malloc(concatLength*sizeof(char));
    strcpy(ret, concat);
    return ret;

}

/**
 * This function splits a character array into strings determined by a passed delimiter
 * Assumption:
                * output[] is allocated to fit more than the number of strings input[] will give
                * input[] is terminated with null character and is proper string
 * Input parameters: an array of character pointers, a character array, and a delimiter character
 * Returns: N/A
 * Notes:
        * input[] will be changed
        * output[] will be changed and will store the strings
 **/
void split(char *output[], char input[], char delim) {

    int length = strlen(input);
    int strNum = 0;
    int written = 0;

    int i = 0;

    // skip to first character that is not the delimiter
    while (input[i] == delim)
        ++i;

    while (i < length) {
        if (!written) { // has string already been saved?

            // start of a new string, so we save it

            output[strNum++] = &input[i];
            written = 1; // only run once per string we save and at the start. we save the full string, not only part of the string
        }

        ++i;

        while(input[i] == delim) {
            input[i++] = '\0';     // while skipping delimiters, change them to null characters to cut off the most recent character pointer
            written = 0;           // signify that next string will be new
        }
    }

    // final pointer will be NULL
    output[strNum] = 0;
}

int main(){

    char command[maxLength];

    int status;

    int count = 1;
    pid_t childPID;

    while (fgets(command, maxLength - 1, stdin) != NULL && (childPID = fork()) > 0) // read line and fork until EOF. one child process for each line command
        ++count;

    if (childPID == 0) { // BEGIN CHILD PROCESS
        char *out = concatenate(getpid(), outputFileExtension); // construct PID.out
        char *err = concatenate(getpid(), errorFileExtension); // construct PID.err

        // replace stdout and stderr file descriptors
        dup2(open(out, O_RDWR | O_CREAT | O_APPEND, 0777), STDOUT_FILENO);
        dup2(open(err, O_RDWR | O_CREAT | O_APPEND, 0777), STDERR_FILENO);

        free(out);
        free(err);

        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout); // flush buffer before exec. otherwise, will not be printed as buffer gets overrided by exec

        char *parameters[strlen(command)];
        command[strlen(command) - 1] = '\0'; // remove newline character from fgets

        split(parameters, command, ' ');
        execvp(parameters[0], parameters);

        perror(command);

        exit(2);
    } // END CHILD PROCESS

    else if (childPID > 0) { // BEGIN PARENT PROCESS
        while ((childPID = wait(&status)) > 0) {

            char *outFile = concatenate(childPID, outputFileExtension);

            dup2(open(outFile, O_WRONLY | O_APPEND, 0777), STDOUT_FILENO); // open finished child process PID.out and use it as stdout

            printf("Finished child %d with pid of parent %d\n", childPID, getpid());
            fflush(stdout); // flush now on handling current child. otherwise, the prints will stay in the buffer and be dumped in the wrong child output file

            char *errFile = concatenate(childPID, errorFileExtension);

            dup2(open(errFile, O_WRONLY | O_APPEND, 0777), STDERR_FILENO); // open finished child process PID.err and use it as stderr

            // stderr does not need fflush as it is unbuffered
            if (WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));

            else if (WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));

            free(outFile);
            free(errFile);
        }
    } // END PARENT PROCESS

    else
        fprintf(stderr, "error occurred in forking");

    return 0;
}
