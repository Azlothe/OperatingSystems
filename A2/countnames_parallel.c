/**
 * Description: This counts the occurrences of strings in the inputted text files. It reads the string line by line.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/2/2023
 * Creation date: 3/1/2023
 **/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define maxNames 100
#define maxLength 31

typedef struct name {
    char input[maxLength];
    int count;
} name;

const name empty = { {'\0', 0} };

name nameCount[maxNames];
int registeredNames = 0;

const long bufferSize = maxNames * sizeof(name);

int status;


/**
 * This function finds if two inputted name structs are equivalent. It returns 1 for equal and 0 for not equal.
 * Assumption: o1.input and o2.input are proper C strings
 * Input parameters: two name parameters
 * Returns: an integer
 **/
int equals(name o1, name o2) {
    // equality will be based on the name.input
    return (strcmp(o1.input, o2.input) == 0);
}


/**
 * This function finds the index of an equivalent string stored in nameCount. It returns -1 if there is no found index.
 * Assumption: input compare is terminated with null character
 * Input parameters: string
 * Returns: an integer
 **/
int indexOf(char compare[]){

    // check through all existing names, not the entire array
    for (int i = 0; i < registeredNames; i++)
        if (strcmp(nameCount[i].input, compare) == 0)
            return i;

    return -1;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No input file given\n");
        return 0;
    }

    int num = 0; // for parent, num = number of child processes; for child, num = their index in child[] and an assigned text file from argv[]
    pid_t childPID; // to determine if is spawned child

    int fd[2];
    pipe(fd); // all children will share pipe

    while (num < argc - 1 && (childPID = fork()) > 0) // 1 parent and one child process for each file
        ++num;


    if (childPID == 0) { // BEGIN CHILD PROCESS

        close(fd[0]); // close child read

        char *fileName = argv[num+1];

        FILE *fp = fopen(fileName, "r");
        if (fp == NULL){
            printf("range: cannot open file %s\n", fileName);

            status = -1;
            write(fd[1], &status, sizeof(int));
            return 1;
        }

        char lineInput[maxLength];

        // terminate if passed empty file
        if (fgets(lineInput, maxLength, fp) == NULL) {
            fprintf(stderr, "File %s is empty\n", fileName);

            status = -1;
            write(fd[1], &status, sizeof(int));
            return 0;
        }

        // initialize name array with null characters
        for (int i = 0; i < maxNames; i++)
            nameCount[i] = empty;

        int lineCounter = 0;

        do {
            ++lineCounter;

            // skip empty lines and warn line is empty
            if (strcmp(lineInput,"\n") == 0) {
                fprintf(stderr,"Warning - file %s line %d is empty\n", fileName, lineCounter);
                continue;
            }

            // remove the newline character from fgets
            lineInput[strlen(lineInput) - 1] = '\0';

            int result = indexOf(lineInput);

            if (result <= -1) {

                // new input is not registered; register it
                name newName;

                strcpy(newName.input,lineInput);
                newName.count = 1;

                nameCount[registeredNames++] = newName;

            }
            else
                ++nameCount[result].count;

        }
        while(fgets(lineInput, maxLength, fp) != NULL); // EOF

        write(fd[1], &registeredNames, sizeof(int));
        write(fd[1], nameCount, bufferSize);
    }

    // END CHILD PROCESS ==========================================

    else if (childPID > 0) { // BEGIN PARENT PROCESS

        close(fd[1]); // close parent write

        int errorTotal = 0;

        // first child process to end; use it as a base counter
        do {
            wait(NULL);
            read(fd[0], &status, sizeof(int)); // error processing will read a negative integer. success would read in a positive registeredNames value
        }
        while (status <= -1 && ++errorTotal < num);

        if (errorTotal == num) // all passed files have not been scanned and all child processes exited prematurely
            return 0;

        registeredNames = status;
        read(fd[0], nameCount, bufferSize);

        name childName[maxNames];


        // for the rest of the children
        for (int i = 1; i < num; i++) {
            wait(NULL);

            read(fd[0], &status, sizeof(int));

            if(status <= -1)
                continue; // didn't process through file, so skip to next iteration/wait

            read(fd[0], childName, bufferSize);

            int j = 0;
            int result;

            // extracted the nameCount from buffer but may not be fully filled in
            while(j < maxNames && !equals(childName[j], empty)){

                if ((result = indexOf(childName[j].input)) <= -1)
                    nameCount[registeredNames++] = childName[j]; // did not find; fill it in
                else
                    nameCount[result].count += childName[j].count; // found; add what child counted

                ++j;
            }
        }

        // print names and their corresponding count
        for (int i = 0; i < registeredNames; i++)
            printf("%s: %d\n", nameCount[i].input, nameCount[i].count);
    }

    // END PARENT PROCESS  =============================================

    else
        fprintf(stderr, "error occurred in forking");

    return 0;

}
