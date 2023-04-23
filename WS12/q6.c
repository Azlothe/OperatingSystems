/*
 Create a program "q6.c" that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?


 Tbe program does run, and I got lucky as it printed out 0 for the value.

 Using valgrind reveals an invalid read, but it reports that all heap blocks have been freed and there is no memory leak. The invalid read comes from the memory access into data when it had already been freed.

 */

#include <stdlib.h>
#include <stdio.h>

int main(){
    int *data = malloc(100 * sizeof(int));
    free(data);

    printf("%d", data[10]);

    return 0;
}
