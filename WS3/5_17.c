#include <stdio.h>

#define maxSize 9

int main(){
    int val[maxSize];
    int count = 0;
    int current;

    scanf("%d",&current);

    while (current >= 0 && count < maxSize) {
        val[count++] = current;
        scanf("%d",&current);
    }

    if (count >= maxSize) {
        printf("Too many inputs");
        return 0;
    }

    printf("%d\n", val[count/2]);

    return 0;
}
