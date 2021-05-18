#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


// echo "1\n2" | xargs echo line
int main(int argc, char *argv[]) {
    char* child_argv[MAXARG];
    char c;
    char buf[1000];

    int j = 0;
    // 仍然有数据
    while (read(0, &c, 1) == 1) {
        if (c == '\n') {
            buf[j] = '\0';

            int i = 1;
            for (; i < argc; i++) {
                child_argv[i - 1] = argv[i];
            }   
            child_argv[i - 1] = buf;
            child_argv[i] = 0;
            if (fork() == 0) {
                exec(argv[1], child_argv);
            } 
            wait(0);
            j = 0; 
        } else {
            buf[j++] = c;
        }
    }
    exit(0);
}


