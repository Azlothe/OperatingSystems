/**
 * Description: This takes in a list of numbers and outputs if they are "all multiples of 10", "no multiples of 10", or "mixed values".
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 3/8/2023
 * Creation date: 3/8/2023
 **/

#include <stdio.h>
#include <stdbool.h>


/**
 * This function checks if the inputted array contains all multiples of 10.
 * Assumption: numVals should be equal to the length of inputVals[]
 * Input parameters: an array of integers, total number in the array
 * Returns: a boolean
 **/
bool IsArrayMult10(int inputVals[], int numVals) {
    for (int i = 0; i < numVals; i++)
        if ((inputVals[i] % 10) != 0)
            return false;

    return true;
}


/**
 * This function checks if the inputted array contains NO multiples of 10.
 * Assumption: numVals should be equal to the length of inputVals[]
 * Input parameters: an array of integers, total number in the array
 * Returns: a boolean
 **/
bool isArrayNoMult10(int inputVals[], int numVals){
    for (int i = 0; i < numVals; i++)
        if ((inputVals[i] % 10) == 0)
            return false;

    return true;
}



int main(){
    int size;

    scanf("%d", &size);

    int arr[size];

    for (int i = 0; i < size; i++)
        scanf("%d", arr+i);

    if (IsArrayMult10(arr, size))
        printf("all multiples of 10");
    else
        printf(isArrayNoMult10(arr,size) ? "no multiples of 10" : "mixed values");

    printf("\n");

    return 0;
}
