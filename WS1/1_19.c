/**
 * Description: This takes integer inputs and prints out math operations
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 2/1/2023
 * Creation date: 1/30/2023
 **/

#include <stdio.h>

int main(void) {
   int userNum;

   printf("Enter integer:\n");
   scanf("%d", &userNum);

   printf("You entered: %d\n", userNum);

   printf("%d squared is %d\n", userNum, userNum*userNum);

   printf("And %d cubed is %d!!\n", userNum, userNum*userNum*userNum);

   int newNum;
   printf("Enter another integer:\n");
   scanf("%d", &newNum);

   printf("%d + %d is %d\n", userNum, newNum, (userNum+newNum));
   printf("%d * %d is %d\n", userNum, newNum, (userNum*newNum));

    return 0;
}
