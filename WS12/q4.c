/*
 Write a simple program "q4.c" that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (with the --leak-check=yes flag)?


 All that really happens is that there is memory allocated on the heap and then it exits.

 Using gdb did not find any problems. It only reported that the process exited normally.

 On the other hand, valgrind did find a memory leak problem with 4 bytes definitely lost. An integer on my system is 4 bytes. The 4 bytes I allocated were not freed and was caught by valgrind.
 */

#include <stdlib.h>

int main(){
    int *mem = (int *) malloc(sizeof(int));
}
