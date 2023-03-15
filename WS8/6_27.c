/**
 * Description: This takes in a character and a string and outputs how many times the character occurs in the string
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/14/2023
 * Creation date: 3/14/2023
 **/

#include <stdio.h>
#include <string.h>

#define maxLength 50

/**
 * This function counts the character occurrence
 * Assumption: userString is null terminated and is a proper string
 * Input parameters: a character and a character pointer
 * Returns: an integer
 **/

int CountCharacters(char userChar, char* userString) {
    int count = 0;

    for (int i = 0; i < strlen(userString); i++)
        if (userChar == userString[i])
            ++count;

    return count;
}

int main() {

    char search;
    char str[maxLength];

    scanf("%c", &search);
    fgets(str, maxLength, stdin);

    int count = CountCharacters(search, str);

    printf("%d %c", count, search);
    if (count != 1)
        printf("'s");

    printf("\n");

    return 0;
}
