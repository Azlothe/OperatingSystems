#include <stdio.h>

double MilesToLaps(double userMiles){
    return userMiles * 4;
}

int main(){

    double miles;
    scanf("%lf",&miles);

    printf("%0.2lf\n",MilesToLaps(miles));

    return 0;
}
