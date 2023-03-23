#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

char *findChar(char *str, char find) {
    int iterate = 0;
    int len = strlen(str);

    while (str[iterate] != find && iterate < len)
        ++iterate;

    if (iterate >= len) return NULL;

    return &str[iterate];
}

char *getWord(char *begin, char *end) {
    while (*begin == ' ')
        ++begin;

    while (*end == ' ')
        *end-- = '\0';

    return begin;
}


int main() {
    size_t length = 10;
    char *input = (char *) malloc(length);

    while (1) {

        printf("Enter input string:\n");

        length = getline(&input, &length, stdin);
        input[--length] = '\0';

        if (strcmp(input, "q") == 0) break;

        char *iterate = findChar(input, ',');

        if (!iterate) {
            printf("Error: No comma in string\n\n");
            continue;
        }

        *iterate = '\0';

        char *firstWord = getWord(input, iterate - 1);
        char *secondWord = getWord(iterate + 1, input + length);

        printf("First word: %s\n"
        "Second word: %s\n\n",
        firstWord,secondWord
        );

    }


    free(input);

    return 0;

}
