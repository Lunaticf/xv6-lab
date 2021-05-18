#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void gen_seive(int p[]);

// concurrent version to get primes 2-35
int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    gen_seive(p);

    // this process feed 2 - 35 to the pipeline
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, 4);
    }

    close(p[1]);
    wait(0);
    exit(0);
}

void gen_seive(int p[]) {
    if (fork() == 0) {
        // read from left neighbor
        int data;
        int has_right_neighbor = 0;
        int next_pipe[2];
        close(p[1]);

        read(p[0], &data, 4);
        int prime = data;
        printf("prime %d\n", prime);

        while (read(p[0], &data, 4) > 0) {
            if (data % prime != 0) {
                // generate a seive and write to it
                if (!has_right_neighbor) {
                    pipe(next_pipe);
                    if (fork() == 0) {
                        gen_seive(next_pipe);
                    }
                    has_right_neighbor = 1;
                }
                write(next_pipe[1], &data, 4);
            }
        }
        if (has_right_neighbor) {
            close(next_pipe[1]);
            wait(0);
            close(next_pipe[0]);
        }
        exit(0);
    }
}


