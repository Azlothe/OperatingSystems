#include <stdio.h>
#include <string.h>


void IntToReverseBinary(int integerValue, char binaryValue[]) {

    int i = 0;

    do {
        binaryValue[i++] = (integerValue % 2) +'0';
    } while ((integerValue /= 2) > 0);

    binaryValue[i] = '\0';
}

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
