#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// borrow from sh.c
void panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int main(int argc, char *argv[])
{
    char c;

    // create a pair of pipes
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) < 0){
        panic("pipe1");
    }
     if (pipe(pipe2) < 0){
        panic("pipe2");
    }

    // fork a child
    int pid = fork();
    if (pid == 0) {
        // child
        // read a byte and reply
        read(pipe1[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(pipe2[1], "o", 1);
        exit(0);
    } 

    // write a byte and wait
    write(pipe1[1], "i" , 1);

    read(pipe2[0], &c, 1);
    printf("%d: received pong\n", getpid());

    exit(0);
}
