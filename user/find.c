#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// pathda
void find(char *path, char* cur, char *file);
char* strcat(char* a, char* b);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: find dir file\n");
        exit(1);
    }
    find(argv[1], "", argv[2]);
    exit(0);
}

void find(char *path, char* cur, char *file) 
{
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DIR:
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0) {
                continue;
            }
            if (de.inum == 0) 
                continue;
            find(strcat(strcat(path, "/"), de.name), de.name, file);
        }
        break;
    case T_FILE:
        // 如果当前文件名匹配，打印该文件路径
        if (strcmp(cur, file) == 0) {
            printf("%s\n", path);
        }
        break;
    default:
        break;
    }

    // 又是因为文件描述符不够导致错误，一定要记得关闭节省描述符
    close(fd);
}

// 把 b 拼接到 a 后面
char* strcat(char* a, char* b) {
    char* p = malloc(strlen(a) + strlen(b) + 1);
    char* ret = p;
    while ((*p = *a++) != '\0') {
        p++;
    }
    while ((*p++ = *b++) != '\0');
    return ret;
}





