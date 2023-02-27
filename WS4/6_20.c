/**
 * Description: This takes in miles and outputs the corresponding number of laps.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/27/2023
 * Creation date: 2/14/2023
 **/

#include <stdio.h>

/**
 * This function translates miles to laps.
 * Assumption: N/A
 * Input parameters: double
 * Returns: double
**/
double MilesToLaps(double userMiles){
    return userMiles * 4;
}

int main(){

    double miles;
    scanf("%lf",&miles);

    printf("%0.2lf\n",MilesToLaps(miles));

    return 0;
}
