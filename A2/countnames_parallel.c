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

const name empty = {'\0', 0};

name nameCount[maxNames];
int registeredNames = 0;

const long bufferSize = maxNames * sizeof(name);

int status;

// equality will be the name.input
int equals(name o1, name o2) {
    return (strcmp(o1.input, o2.input) == 0);
}


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

name find(name n) {
    for (int i = 0; i < registeredNames; i++)
        if (equals(n, nameCount[i]))
            return nameCount[i];

    //     return NULL;
    return empty;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No input file given\n");
        return 0;
    }

    int num = 0; // for parent, num = number of child processes; for child, num = their index in child[] and an assigned text file from argv[]
    pid_t childPID; // to determine if is spawned child
    pid_t child[argc - 1]; // store PID of child process

    int fd[2];
    pipe(fd); // all children will share pipe

    while (num < argc - 1 && (childPID = fork()) > 0)
        child[num++] = childPID;

    if (childPID == 0) {

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

        close(fd[0]); // close child read

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
    else if (childPID > 0) {

        close(fd[1]); // close parent write


        // first child process to end; use it as a base counter
        wait(NULL);

        read(fd[0], &status, sizeof(int));

        if(status > -1) {
            registeredNames = status;
            read(fd[0], nameCount, bufferSize);
        }

        int childSize;
        name childName[maxNames];


        // for the rest of the children
        for (int i = 1; i < num; i++) {
            wait(NULL);

            read(fd[0], &status, sizeof(int));

            if(status <= -1)
                continue;

            read(fd[0], childName, bufferSize);

            int j = 0;
            int result;
            name current;

            while(j < maxNames && !equals(childName[j], empty)){

                current = find(childName[j]);

                if (equals(current, empty))
                    nameCount[registeredNames++] = childName[j];
                else
                    current.count += childName[j].count;

                ++j;
            }
        }

        // print names and their corresponding count
        for (int i = 0; i < registeredNames; i++)
            printf("%s: %d\n", nameCount[i].input, nameCount[i].count);
    }
    else
        fprintf(stderr, "error occurred in forking");

    return 0;

}
