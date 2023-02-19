/**
 * Description: This counts the occurrences of strings in an inputted text file. It reads the string line by line.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/19/2023
 * Creation date: 2/13/2023
 **/

#include <stdio.h>
#include <string.h>

#define maxNames 100
#define maxLength 31

// mapping between name and count will be the index
// index will express the name and its count
char names[maxNames][maxLength];
int count[maxNames];

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
        if (strcmp(names[i], compare) == 0)
            return i;

    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // require only one parameter - the text file
        fprintf(stderr, "No file given\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("error: cannot open file\n");
        return 1;
    }

    char lineInput[maxLength];

    // terminate if passed empty file
    if (fgets(lineInput, maxLength, fp) == NULL) {
        fprintf(stderr, "File is empty");
        return 0;
    }

    // initialize name array with null characters
    for (int i = 0; i < maxNames; i++)
        for (int j = 0; j < maxLength; j++)
            names[i][j] = '\0';

    int lineCounter = 0;

    do {
        ++lineCounter;

        // skip empty lines and warn line is empty
        if (strcmp(lineInput,"\n") == 0) {
            fprintf(stderr,"Warning: Line %d is empty\n", lineCounter);
            continue;
        }

        // remove the newline character from fgets
        lineInput[strlen(lineInput) - 1] = '\0';


        int result = indexOf(lineInput);

        if (result <= -1) {

            // new input is not registered; register it
            int j = 0;
            while (lineInput[j] != '\0'){
                names[registeredNames][j] = lineInput[j];
                ++j;
            }

            ++count[registeredNames];
            ++registeredNames;
        }
        else
            ++count[result];
    }
    while(fgets(lineInput, maxLength, fp) != NULL); // EOF

    // print names and their corresponding count
    for (int i = 0; i < registeredNames; i++)
        printf("%s: %d\n", names[i], count[i]);

    fclose(fp);

    return 0;
}
