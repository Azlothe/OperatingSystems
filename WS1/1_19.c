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
