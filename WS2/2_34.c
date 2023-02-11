/**
 * Description: This takes in an input as name and prints out a string using that name
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/11/2023
 * Creation date: 2/10/2023
 **/

#include <stdio.h>

int main() {

    char userName[50];

    scanf("%s", userName);

    printf("Hello %s, and welcome to CS Online!\n", userName);

    return 0;
}
