/**
 * Description: This takes in an integer and prints it out in binary
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/2/2023
 * Creation date: 2/28/2023
 **/

#include <stdio.h>
#include <string.h>

/**
 * This function converts an integer into its corresponding binary representation in reverse (from LSB to MSB in left to right)
 * Assumption: integerValue is not negative
 * Input parameters: an integer and an array to be written into
 * Returns: N/A
 **/
void IntToReverseBinary(int integerValue, char binaryValue[]) {

    int i = 0;

    do {
        binaryValue[i++] = (integerValue % 2) +'0'; // '0' = 48 ascii. needed, otherwise char array would only get ascii 0 of NULL and 1 of SOH
    } while ((integerValue /= 2) > 0);

    binaryValue[i] = '\0';
}

/**
 * This function reverses a string
 * Assumption: N/A
 * Input parameters: two strings. one is the original input and the other is the string to contain the reversed string
 * Returns: N/A
 **/
void StringReverse(char inputString[], char reversedString[]) {

    int length = strlen(inputString) - 1;

    for (int i = length; i >= 0; i--)
        reversedString[length- i] = inputString[i];

    reversedString[length + 1] = '\0';
}

int main(){
    int num;
    scanf("%d", &num);

    char binary[32];

    IntToReverseBinary(num, binary);

    char unreverse[32];
    StringReverse(binary, unreverse);

    printf("%s\n", unreverse);

    return 0;
}
