#include <stdio.h>

int maxNumber(int num1, int num2, int num3, int num4){
    int nums[] = {num1, num2, num3, num4};
    int max = num1;
    for (int i = 0; (i*4) < sizeof(nums); i++)
        if (nums[i] > max)
            max = nums[i];

    return max;
}

int minNumber(int num1, int num2, int num3, int num4){
    int nums[] = {num1, num2, num3, num4};
    int min = num1;
    for (int i = 0; (i*4) < sizeof(nums); i++)
        if (nums[i] < min)
            min = nums[i];

    return min;
}

int main(){
    int num1;
    int num2;
    int num3;
    int num4;

    scanf("%d %d %d %d", &num1, &num2, &num3, &num4);


    printf("Maximum is %d\n", maxNumber(num1, num2, num3, num4));

    printf("Minimum is %d\n", minNumber(num1, num2, num3, num4));

    return 0;
}
