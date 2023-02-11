/**
 * Description: This takes in two inputs of price and prints out price, change, and zybooks mortgage
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/11/2023
 * Creation date: 2/10/2023
 **/

#include <stdio.h>

int main() {

    int currentPrice;
    int lastMonthPrice;

    scanf("%d %d", &currentPrice, &lastMonthPrice);

    printf("This house is $%d. The change is $%d since last month.\nThe estimated monthly mortgage is $%f.\n",
           currentPrice,
           (currentPrice - lastMonthPrice),
           ((currentPrice * 0.051)/12)
    );

    return 0;
}
