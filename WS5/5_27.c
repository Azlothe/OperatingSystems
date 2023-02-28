#include <stdio.h>
#include <string.h>

#define maxChar 51

char lineInput[maxChar];
char *quit[] = { "quit", "Quit", "q"};
int exitStringNum = sizeof(quit)/sizeof(quit[0]);

void printReverse(char input[]){
    for (int i = strlen(input) - 1; i >= 0; i--)
        printf("%c", input[i]);

    printf("\n");
}

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
