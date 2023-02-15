#include <stdio.h>

int main(){

    int next;
    scanf("%d", &next);

    int max = next;
    int sum = 0;
    int count = 0;

    while(next >= 0){
        if (next > max)
            max = next;
        sum += next;
        ++count;
        scanf("%d", &next);
    }

    printf("%d %d\n", max, (sum/count));

    return 0;
}
