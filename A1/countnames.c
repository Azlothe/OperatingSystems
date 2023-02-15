#include <stdio.h>
#include <string.h>

#define maxNames 100
#define maxLength 31

char names[maxNames][maxLength];
int count[maxNames];
int registeredNames = 0;

int indexOf(char compare[]){

    for (int i = 0; i < registeredNames; i++)
        if (strcmp(names[i], compare) == 0)
            return i;

    return -1;
}

int main() {

    for (int i = 0; i < maxNames; i++)
        for (int j = 0; j < maxLength; j++)
            names[i][j] = '\0';

    char lineInput[maxLength];

    //     printf("%s", names[0]);

    for (int i = 0; i < 7; i++){

        printf("Enter input\n");
//         scanf("%s", lineInput);
        fgets(lineInput, maxLength, stdin);

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

    for (int i = 0; i < registeredNames; i++)
        printf("%s: %d\n", names[i], count[i]);

    return 0;
}
