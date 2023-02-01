/**
 * Description: This prints a right-facing arrow
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/1/2023
 * Creation date: 1/30/2023
 **/

#include <stdio.h>


/**
 * This function prints a line of the arrow tip without body
 * Assumption: headInt is single digit integer
 * Input parameters: the char length for body, total char length of arrow, integer used for head
 * Returns: n/a
**/
void arrowEdge(int bodyLength, int maxLength, int headInt){
    while (--maxLength >= 0) {
        if (--bodyLength >= 0)
            printf(" ");
        else
            printf("%d", headInt);
    }
    printf("\n");
}

/**
 * This function prints a line of the arrow body
 * Assumption: bodyInt and headInt are single digit integers
 * Input parameters: the char length for body, total char length of arrow, integer used for body, integer used for head
 * Returns: n/a
**/
void arrowBody(int bodyLength, int maxLength, int bodyInt,int headInt){
    while (--maxLength >= 0) {
        if (--bodyLength >= 0)
            printf("%d",bodyInt);
        else
            printf("%d", headInt);
    }
    printf("\n");
}

int main(void) {

    int baseInt;
    int headInt;

    scanf("%d %d", &baseInt, &headInt);

    arrowEdge(4,5,headInt);
    arrowEdge(4,6,headInt);
    arrowBody(4,7,baseInt,headInt);
    arrowBody(4,8,baseInt,headInt);
    arrowBody(4,7,baseInt,headInt);
    arrowEdge(4,6,headInt);
    arrowEdge(4,5,headInt);

    return 0;
}
