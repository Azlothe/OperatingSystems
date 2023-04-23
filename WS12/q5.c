/*
 Write a program "q5.c" that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?


 The program runs and terminates normally when I run it.

 Running this program with valgrind points out an invalid write and the memory leak of the entire array block.

 The program is technically correct since data[100] will be set to 0. It's not necessarily prohibited and will go through. The issue arises in that it is overwriting memory that we have not allocated for it, possibly roaming into other allocated memory. If what was overwritten is important or in use, this could crash the program or be a security danger.
 */

#include <stdlib.h>

int main(){
    int *data = (int *) malloc(100 * sizeof(int));
    data[100] = 0;

    return 0;
}
