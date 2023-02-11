/**
 * Description: This takes in three integers and prints out the smallest one
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/11/2023
 * Creation date: 2/10/2023
 **/

#include <stdio.h>

int main() {

    int first;
    int second;
    int third;

    scanf("%d %d %d", &first, &second, &third);

    if (first > second)
        printf("%d", (second > third) ? third : second);
    else
        printf("%d", (first > third) ? third : first);

    printf("\n");

    return 0;
}
