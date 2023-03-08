/**
 * Description: This takes in a name or phrase and checks whether or not it is a palindrome.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/8/2023
 * Creation date: 3/8/2023
 **/

#include <stdio.h>
#include <string.h>

#define maxLength 50

int main(){

    char str[maxLength];
    fgets(str, maxLength, stdin);

    int length = strlen(str) - 2;

    char *start = &str[0];
    char *end = &str[length];

    for (int i = 0; i <= (length/2); i++) {

        while (*start == ' ') // skip spaces from beginning
            ++start;

        while (*end == ' ') // skip spaces from end
            --end;

        if (*start++ != *end--){
            printf("not a palindrome: %s", str);
            return 0;
        }
    }

    printf("palindrome: %s", str);

    return 0;
}
