#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

#define maxNames 100
#define maxLength 31

typedef struct name {
    char input[maxLength];
    int count;
} name;

name nameCount[maxNames] = { {'\0',0} };

int main(int argc, char *argv[]) {

    int num = 0;
    pid_t childPID;
    pid_t child[argc - 1];

    while (num < argc - 1 && (childPID = fork()) > 0) // 1 parent and one children for each file

//         static pid_t child[argc - 1];
        child[num++] = childPID;


    if (childPID < 0)
        fprintf(stderr, "error occurred in forking");

    else if (childPID == 0) {
        printf("childPID %d + PID %d: %d with num %d\n ", childPID, getpid(),child[num], num);
    }
    else if (childPID > 0) {
        printf("%d", num);

        for (int i = 1; i < num; i++)
            waitpid(child[i], NULL, 0);


        printf("\nran\n");
    }



    return 0;

}
