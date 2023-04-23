/**
 * Description: This takes in commands (separated by a new line) and runs them in parallel. It creates a .out file for the output and a .err file for any error messages. It has been improved upon the old proc_manager to resemble a system daemon to restart processes. To do so, it stores the start & completion time.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/22/2023
 * Creation date: 4/22/2023
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>

// Edit these to change the file extensions of the log files
#define outputFileExtension ".out"
#define errorFileExtension ".err"

// Edit this value to change the slot sizes of the hash table
#define HASHSIZE 101



/**
* This nlist is a node, which stores one command's info in the
hashtable.
* There is one hashtable slot for any hash value. The linked list allows for storing multiple nlist nodes within a slot.
    * The nlist *next field is a pointer to the next node in the linked list.
*/
struct nlist { //table entry
    char *command;
    int index; // line index in the input text file
    pid_t pid;

    struct timespec startTime;
    struct timespec finishTime;

    struct nlist *next; // next entry in chain
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
static struct nlist *hashtab[HASHSIZE]; // pointer table



/**
 * This function will compute a hash value for given input
 * Assumption: N/A
 * Input parameters: an integer
 * Returns: the hash value
 **/
unsigned hash(unsigned int pid) {
    return pid % HASHSIZE;
}



/**
 * This function searches for a corresponding nlist node within the hash table
 * Assumption: N/A
 * Input parameters: an integer that serves as the primary key of identification
 * Returns: the nlist node if found; NULL if not found
 **/
struct nlist *lookup(int pid) {

    struct nlist *np;

    // Traverse the linked list under the calculated hash value slot
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
            return np; // found

    return NULL; // not found
}



/**
 * This function creates and inserts an nlist node into the table
 * Assumption: A NULL terminated string is passed to char *command
 * Input parameters: a string as the command, an integer as the pid, an integer as the index
 * Returns: the nlist node (either created or already existing)
 **/
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



/**
 * This function frees the hash table and all of its memory contents
 * Assumptions:
    * A NULL terminated string is passed to char *command
    * hashTable has been malloc'd and is stored in heap memory
 * Input parameters: a string as the command, an integer as the pid, an integer as the index
 * Returns: the nlist node (either created or already existing)
 **/
void freeHashTable(struct nlist **hashTable){

    // Run through all slots of the hash table
    for (int i = 0; i < HASHSIZE; i++){
        struct nlist *hashSlotNode = hashTable[i];

        while (hashSlotNode != NULL) {
            // Traverse the linked list within a single slot

            free(hashSlotNode->command); // free the string malloc'd by strdup() within insert()

            struct nlist *nextNode = hashSlotNode->next; // save the next node to set to when the previous one will be freed
            free(hashSlotNode);

            hashSlotNode = nextNode;
        }
    }

    // hashtab is not in heap memory, freeing would give an invalid memory address
    // free(hashTable);
}



/**
 * This function splits a character array into strings determined by a passed delimiter
 * Assumption:
    * A NULL terminated string is passed into *input
 * Input parameters: a character array and a delimiter character
 * Returns: an array of strings as a **char
 * Notes:
        * input[] will be changed
            * all instances of delimiter will be changed to NULL character
 **/
char **split(char *input, char delim) {

    // Go through the input and count the number of strings we will have

    int totalStr = 1; // initially set to 1 because delimiter are in between strings, so there will be one less delimiter than strings

    for (int i = 0; i < strlen(input); i++)
        if (input[i] == delim)
            ++totalStr;

    char **strings = (char **) malloc( (totalStr + 1) * sizeof(char *)); // +1 because one extra for a final NULL

    int strNum = 0;
    int written = 0; // determines if currently-read string has already been written and saved

    char *currentChar = input;

    while (strNum < totalStr) {
        if (!written) { // has string already been saved?

            // start of a new string, so we save it

            strings[strNum++] = currentChar;
            written = 1; // only run once per string we save and at the start. we save the full string, not only part of the string
        }

        ++currentChar;

        while(*currentChar == delim) {
            *(currentChar++) = '\0'; // while skipping delimiters, change them to null characters to cut off the most recent character pointer
            written = 0; // signify that next string will be new
        }
    }

    // final pointer will be NULL
    strings[strNum] = 0;

    return strings;
}



/**
 * This function redirects all stdout messages to PID.out and all stderr messages to PID.err
 * Assumptions:
    * all messages will be format as {PID}.{file extension}
    * new files can be opened/created
 * Input parameters: an integer
 * Returns: N/A
 * Notes:
    * the file extension are subject to change
    * PID is given from the caller. The caller may input any integer value; it does not have to be the PID.
 **/
void redirectToFilePID(int PID){
    int lenPID = log10(PID) + 2; // add +2 because +1 to account for the 1 digit not calculated by a log_base_10, and the other +1 for null terminating character at the end

    // file extensions are subject to change, so we add their calculated lengths
    char *out = (char *) malloc(lenPID + strlen(outputFileExtension));
    char *err = (char *) malloc(lenPID + strlen(errorFileExtension));

    // construct PID.out and PID.err
    sprintf(out, "%d%s", PID, outputFileExtension);
    sprintf(err, "%d%s", PID, errorFileExtension);

    // replace stdout and stderr file descriptors
    dup2(open(out, O_RDWR | O_CREAT | O_APPEND, 0777), STDOUT_FILENO);
    dup2(open(err, O_RDWR | O_CREAT | O_APPEND, 0777), STDERR_FILENO);

    free(out);
    free(err);
}



int main(){

    // can't be sure what will be in the hash table at first, so we preemptively set all slots to NULL
    for (int i = 0; i < HASHSIZE; i++)
        hashtab[i] = NULL;

    char *input = NULL;
    size_t size = 0;

    int status;

    int count = 1;
    pid_t forkPID = getpid();

    struct timespec currentTime;

    while (getline(&input, &size, stdin) >= 0 && forkPID > 0) { // read line and fork until EOF. one child process for each line command

        input[strlen(input) - 1] = '\0';

        clock_gettime(CLOCK_MONOTONIC, &currentTime); // start time

        // child process immediately goes to run while parent saves child info
        if ((forkPID = fork()) == 0) break;

        struct nlist *hashNode = insert(input, forkPID, count++);
        hashNode->startTime = currentTime;

    }

    if (forkPID == 0){ // BEGIN CHILD PROCESS

        redirectToFilePID(getpid());

        // Restarting will come to EXECUTE_COMMAND
        EXECUTE_COMMAND:

        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout); // flush buffer before exec. otherwise, will not be printed as buffer gets overrided by exec

        char **parameters = split(input, ' ');
        execvp(parameters[0], parameters);

        // if here is reached, exec failed
        perror(input);
        exit(2);

    } // END CHILD PROCESS

    else if (forkPID > 0){ // BEGIN PARENT PROCESS
        while ((forkPID = wait(&status)) > 0) {

            clock_gettime(CLOCK_MONOTONIC, &currentTime); // completion time

            redirectToFilePID(forkPID);

            if (WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));

            else if (WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));

            struct nlist *completedProcess = lookup(forkPID);
            completedProcess->finishTime = currentTime;

            double elapsed = currentTime.tv_sec - (completedProcess->startTime).tv_sec;

            printf("Finished at %ld, runtime duration %.1f\n", currentTime.tv_sec, elapsed);
            fflush(stdout); // flush now on handling current child. otherwise, the prints will stay in the buffer and be dumped in the wrong child output file


            if (elapsed > 2){
                // RESTART Procedure

                clock_gettime(CLOCK_MONOTONIC, &currentTime); // get the start time

                // reset the command to execute and the file line number
                input = completedProcess->command;
                count = completedProcess->index;

                if ((forkPID = fork()) == 0) { // restarting child process
                    redirectToFilePID(getpid());
                    printf("RESTARTING\n");
                    fprintf(stderr, "RESTARTING\n");

                    goto EXECUTE_COMMAND;
                }

                // save the restarting process to the hash table
                struct nlist *hashNode = insert(input, forkPID, count);
                hashNode->startTime = currentTime;

            }
            else
                fprintf(stderr, "spawning too fast\n");

        }

        // all processes have completed execution, we now free to prevent memory leaks
        free(input);
        freeHashTable(hashtab);

    } // END PARENT PROCESS

    else
        fprintf(stderr, "error occurred in forking");

    return 0;
}
