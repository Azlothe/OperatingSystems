/**
 * Description: This takes in two inputs and prints out the first three integer divisions
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/11/2023
 * Creation date: 2/10/2023
 **/


#include <stdio.h>

int main() {
    int userNum;
    int x;

    scanf("%d %d", &userNum, &x);

    for (int i = 0; i < 3; i++)
        printf("%d ", userNum /= x);

    printf("\n");

    return 0;
}
