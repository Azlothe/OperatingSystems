#include <stdio.h>

int main(){

    int size;

    scanf("%d", &size);

    int val[size];

    for (int i = 0; i < size; i++){
        int temp;
        scanf("%d", & temp);
        val[i] = temp;
    }

    int threshold;
    scanf("%d", &threshold);

    for (int i = 0; i < size; i++)
        if (val[i] <= threshold)
            printf("%d,",val[i]);

    printf("\n");

    return 0;
}
