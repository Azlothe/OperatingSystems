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

name nameCount[maxNames] = { {'\0',0} };

int registeredNames = 0;


/**
 * This function finds the index of an equivalent string stored in names. It returns -1 if there is no found index.
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

    // TODO
    // check if any arguments passed with argc

    if (argc < 2) {
        printf("No input file given\n");
        return 0;
    }

    int num = 0; // for parent, num = number of child processes; for child, num = their index in child[] and an assigned text file from argv[]
    pid_t childPID; // to determine if is spawned child
    pid_t child[argc - 1]; // store PID of child process

    while (num < argc - 1 && (childPID = fork()) > 0)
        child[num++] = childPID;

    if (childPID == 0) {

        char *fileName = argv[num+1];

        FILE *fp = fopen(fileName, "r");
        if (fp == NULL){
            printf("range: cannot open file %s\n", fileName);
            return 1; // TODO check this at parent wait(NULL) and dont read anything
        }

        char lineInput[maxLength];

        // terminate if passed empty file
        if (fgets(lineInput, maxLength, fp) == NULL) {
            fprintf(stderr, "File %s is empty", fileName);
            return 0; // TODO check this at parent wait(NULL) and dont read anything
        }

        // initialize name array with null characters
        for (int i = 0; i < maxNames; i++) {
            for (int j = 0; j < maxLength; j++)
                nameCount[i].input[j] = '\0';

            nameCount[i].count = 0;
        }

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

        for (int i = 0; i < registeredNames; i++)
            printf("%s: %d\n", nameCount[i].input, nameCount[i].count);
    }
    else if (childPID > 0) {

        for (int i = 0; i < num; i++) {
            //             waitpid(child[i], NULL, 0);
            wait(NULL);


            printf("\nran %d\n",i);

        }

        // print names and their corresponding count
        for (int i = 0; i < registeredNames; i++)
            printf("%s: %d\n", nameCount[i].input, nameCount[i].count);
    }
    else
        fprintf(stderr, "error occurred in forking");

    return 0;

}
