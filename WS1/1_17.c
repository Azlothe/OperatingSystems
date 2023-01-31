#include <stdio.h>

void arrowEdge(int bodyLength, int maxLength, int headInt){
    while (--maxLength >= 0) {
        if (--bodyLength >= 0)
            printf(" ");
        else
            printf("%d", headInt);
    }
    printf("\n");
}

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
