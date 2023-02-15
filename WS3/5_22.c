#include <stdio.h>

int main(){

    int size;
    scanf("%d", &size);

    int val[size];

    for (int i = 0; i < size; i++){
        int temp;
        scanf("%d", &temp);
        val[i] = temp;
    }

    int lowerBound;
    int upperBound;

    scanf("%d %d",&lowerBound, &upperBound);

    for (int i = 0; i < size; i++)
        if (val[i] >= lowerBound && val[i] <= upperBound)
            printf("%d ", val[i]);

    printf("\n");

    return 0;
}
