/**
 * Description: This takes integers and swaps their values
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/14/2023
 * Creation date: 3/14/2023
 **/
#include <stdio.h>

/**
 * This function swaps the values referenced by two pointers
 * Assumption: N/A
 * Input parameters: two integer pointers
 * Returns: N/A
 **/
void swap(int *val1, int *val2) {
    int temp;
    temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

/**
 * This function takes in four integers as the specification and swaps the pairs
 * Assumption: N/A
 * Input parameters: four integer pointers
 * Returns: N/A
 **/
void SwapValues(int *userVal1, int *userVal2, int *userVal3, int *userVal4) {
    swap(userVal1, userVal2);
    swap(userVal3, userVal4);
}

int main() {
    int userVal1;
    int userVal2;
    int userVal3;
    int userVal4;

    scanf("%d %d %d %d", &userVal1, &userVal2, &userVal3, &userVal4);

    SwapValues(&userVal1, &userVal2, &userVal3, &userVal4);

    printf("%d %d %d %d\n", userVal1, userVal2, userVal3, userVal4);

    return 0;
}
