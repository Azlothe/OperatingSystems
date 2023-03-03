/**
 * Description: This takes in strings and prints them in reverse. It continues indefinitely until user quits with terms "quit", "Quit, or "q".
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/2/2023
 * Creation date: 2/28/2023
 **/

#include <stdio.h>
#include <string.h>

#define maxChar 51

char lineInput[maxChar];
char *quit[] = { "quit", "Quit", "q"};
int exitStringNum = sizeof(quit)/sizeof(quit[0]);

/**
 * This function prints out a string in reverse
 * Assumption: input is terminated with null character
 * Input parameters: string
 * Returns: N/A
 **/
void printReverse(char input[]){
    for (int i = strlen(input) - 1; i >= 0; i--)
        printf("%c", input[i]);

    printf("\n");
}

/**
 * This function prompts the user for the string to reverse and checks for any eligible terminating strings.
 * Assumption: N/A
 * Input parameters: N/A
 * Returns: an integer
 **/
int control(){
    fgets(lineInput, maxChar - 1, stdin);

    lineInput[strlen(lineInput) - 1] = '\0';

    for (int i = 0; i < exitStringNum; i++)
        if (strcmp(lineInput,quit[i]) == 0)
            return 0;

    printReverse(lineInput);

    return control();
}

int main(){
    return control();
}
