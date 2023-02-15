#include <stdio.h>

#define maxSize 20

int main() {

    int val[maxSize];
    int registeredVals;

    scanf("%d", &registeredVals);

    for (int i = 0; i < registeredVals; i++)
        scanf("%d", &val[i]);

    while (registeredVals-- > 0)
        printf("%d,",val[registeredVals]);

    printf("\n");

    return 0;

}
