#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>

#define outputFileExtension ".out"
#define errorFileExtension ".err"


/**
* This nlist is a node, which stores one command's info in the
hashtable.
* There is one hashtable slot for any hash value. The linked list allows for storing multiple nlist nodes within a slot.
    * The nlist *next field is a pointer to the next node in the linked list.
*/
struct nlist { //table entry

    struct nlist *next; // next entry in chain


    struct timespec startTime;
    struct timespec finishTime;

    int index; // line index in the input text file

    pid_t pid;

    char *command;

};

/**
* Using a hashtable to store the commands in
hash slots by PID's.
* Lookup a command using a pid to retrieve all the info you need:
    * command
    * index (i.e. line number)
    * start time
    * completion time
* The hashtable is an array of nlists where each array slot represents a computed hash value. Each array position holds a linked list of nlist nodes
slot.
*/
#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE]; // pointer table

// Form hash value for input pid
unsigned hash(unsigned int pid) {
    // TODO simple hash function, may change
    return pid % HASHSIZE;
}


// Look for the nlist node that stores the corresponding PID
struct nlist *lookup(int pid) {

    struct nlist *np;

    // Traverse the linked list under the calculated hash value slot
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
            return np; // found

    return NULL; // not found
}


// Insert a new nlist node into the hash table
struct nlist *insert(char *command, int pid, int index) {

    struct nlist *np;

    unsigned hashval;

    if ((np = lookup(pid)) == NULL) { // couldn't find an existing node; we have to create it

        np = (struct nlist *) malloc(sizeof(*np));

        if (np == NULL) return NULL;

        np->pid = pid;
        np->index = index;
        np->command = strdup(command);

        // We insert at the head of the linked list
        // The most recently created/inserted will be the quickest to access
        hashval = hash(pid);
        np->next = hashtab[hashval];

        hashtab[hashval] = np;
    }

    return np;
}


// Split a character array into strings separated by an inputted delimiter
char **split(char *input, char delim) {

    int delimCount = 1;

    for (int i = 0; i < strlen(input); i++)
        if (input[i] == delim)
            ++delimCount;

    char **strings = (char **) malloc(delimCount * sizeof(char *));

    int strNum = 0;
    int written = 0;

    char *currentChar = input;


    while (strNum < delimCount) {
        if (!written) { // has string already been saved?

            // start of a new string, so we save it

            strings[strNum++] = currentChar;
            // output[strNum++] = &input[i];
            written = 1; // only run once per string we save and at the start. we save the full string, not only part of the string
        }

        ++currentChar;

        while(*currentChar == delim) {
            *(currentChar++) = '\0';     // while skipping delimiters, change them to null characters to cut off the most recent character pointer
            written = 0;           // signify that next string will be new
        }
    }

    // final pointer will be NULL
    strings[strNum] = 0;

    return strings;

}


// Construct PID.out and PID.err to redirect stdout and stderr respectively
void redirectToFilePID(int pid){
    int lenPID = log10(pid) + 2;

    char *out = (char *) malloc(lenPID + strlen(outputFileExtension));
    char *err = (char *) malloc(lenPID + strlen(errorFileExtension));

    sprintf(out, "%d%s", pid, outputFileExtension);
    sprintf(err, "%d%s", pid, errorFileExtension);

    dup2(open(out, O_RDWR | O_CREAT | O_APPEND, 0777), STDOUT_FILENO);
    dup2(open(err, O_RDWR | O_CREAT | O_APPEND, 0777), STDERR_FILENO);

    free(out);
    free(err);
}



int main(){
    char *input = NULL;
    size_t size = 0;

    int status;

    int count = 1;
    pid_t forkPID = getpid();

    struct timespec currentTime;

    while (getline(&input, &size, stdin) >= 0 && forkPID > 0) { // read line and fork until EOF. one child process for each line command

        input[strlen(input) - 1] = '\0';

        clock_gettime(CLOCK_MONOTONIC, &currentTime);

        if ((forkPID = fork()) == 0) break;

        struct nlist *hashNode = insert(input, forkPID, count++);
        hashNode->startTime = currentTime;

    }

    if (forkPID == 0){

        redirectToFilePID(getpid());

        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout); // flush buffer before exec. otherwise, will not be printed as buffer gets overrided by exec


        char **parameters = split(input, ' ');

        execvp(parameters[0], parameters);

        perror(input);
        exit(2);
    }
    else if (forkPID > 0){
        while ((forkPID = wait(&status)) > 0) {

            clock_gettime(CLOCK_MONOTONIC, &currentTime);

            struct nlist *completedProcess = lookup(forkPID);
            completedProcess->finishTime = currentTime;

            double elapsed = currentTime.tv_sec - (completedProcess->startTime).tv_sec;

            redirectToFilePID(forkPID);

            printf("Finished at %ld, runtime duration %.1f\n", currentTime.tv_sec, elapsed);
            fflush(stdout); // flush now on handling current child. otherwise, the prints will stay in the buffer and be dumped in the wrong child output file

        }
    }
    else
        fprintf(stderr, "error occurred in forking");

    return 0;
}
