#include <stdio.h>

#define maxWords 20
#define maxLength 11

int main(){

    char words[maxWords][maxLength];
    int count;
    scanf("%d", &count);

    for (int i = 0; i < count; i++)
        scanf("%s", words[i]);

    char searchCharacter;
    scanf(" %c", &searchCharacter);


    for (int i = 0; i < count; i++){
        int j = 0;
        while(words[i][j] != searchCharacter && words[i][j] != '\0')
            j++;

        if (words[i][j] == searchCharacter)
            printf("%s ",words[i]);
    }

    printf("\n");

    return 0;
}
