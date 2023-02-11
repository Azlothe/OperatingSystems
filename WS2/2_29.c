/**
 * Description: This takes 3 floating points and prints out calculations
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/11/2023
 * Creation date: 2/10/2023
 **/

#include <stdio.h>
#include <math.h>

int main(){
    double x;
    double y;
    double z;

    scanf("%lf %lf %lf", &x, &y, &z);

    printf("%0.2lf %0.2lf %0.2lf %0.2lf\n",
           pow(x,z),
           pow(x,pow(y,2)),
           fabs(y),
           sqrt(pow(x*y,z))
    );

    return 0;
}
