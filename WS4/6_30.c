/**
 * Description: This takes in 4 integers and outputs the largest and smallest among them.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/27/2023
 * Creation date: 2/14/2023
 **/

#include <stdio.h>

/**
 * This function finds the max of the required 4 inputs
 * Assumption: Called with 4 integer parameter as in Zybooks
 * Input parameters: 4 integers
 * Returns: an integer
**/
int maxNumber(int num1, int num2, int num3, int num4){
    int nums[] = {num1, num2, num3, num4};
    int max = num1;
    for (int i = 0; (i*4) < sizeof(nums); i++)
        if (nums[i] > max)
            max = nums[i];

    return max;
}

/**
 * This function finds the min of the required 4 inputs
 * Assumption: Called with 4 integer parameter as in Zybooks
 * Input parameters: 4 integers
 * Returns: an integer
**/
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
