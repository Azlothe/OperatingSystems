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

    int firstSmall = val[0]; // smallest
    int secondSmall = val[1]; // second smallest

    for (int i = 1; i < size; i++){
        if (val[i] < firstSmall) {
            secondSmall = firstSmall;
            firstSmall = val[i];
        }
        else if (val[i] < secondSmall)
            secondSmall = val[i];
    }

    printf("%d %d\n", firstSmall, secondSmall);

    return 0;
}
