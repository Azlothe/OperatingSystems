#include <stdio.h>
#include <string.h>

#define maxNames 100
#define maxLength 31


// TODO print warning messages to stderr
// check paramters for files



char names[maxNames][maxLength];
int count[maxNames];
int registeredNames = 0;

int indexOf(char compare[]){

    for (int i = 0; i < registeredNames; i++)
        if (strcmp(names[i], compare) == 0)
            return i;

    return -1;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) return 0; // no arguments passed

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("error: cannot open file\n");
        return 1;
    }

    char lineInput[maxLength];

    // terminate if passed empty file
    if (fgets(lineInput, maxLength, fp) == NULL)
        return 0;

    for (int i = 0; i < maxNames; i++)
        for (int j = 0; j < maxLength; j++)
            names[i][j] = '\0';

    do {
        // skip empty lines
        if (strcmp(lineInput,"\n") == 0) continue;

        lineInput[strlen(lineInput) - 1] = '\0';

        int result = indexOf(lineInput);

        if (result <= -1) {
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
    while(fgets(lineInput, maxLength, fp) != NULL);

    for (int i = 0; i < registeredNames; i++)
        printf("%s: %d\n", names[i], count[i]);

    fclose(fp);

    return 0;
}
