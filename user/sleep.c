#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("usage: sleep seconds\n");
        exit(1);
    } 
    
    // sleep system call error
    if (sleep(atoi(argv[1])) != 0) {
        printf("sys call sleep error\n");
        exit(1);
    }
    exit(0);
}

