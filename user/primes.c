#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 36
#define ONE '1'
#define ZERO '0'

void prime(int pipe_read, int pipe_write) {
    char buf[MSGSIZE];
    int val = 0;
    read(pipe_read, buf, MSGSIZE);
    for (int i = 0; i < MSGSIZE; i++) {
        if (buf[i] == ONE) {
            val = i;
            break;
        }
    }
    if (val == 0)
        exit(0);
    printf("prime %d\n", val);
    buf[val] = ZERO;

    for (int i = val; i < MSGSIZE; i++) {
        if (i % val == 0) {
            buf[i] = ZERO;
        }
    }

    int pid = fork();

    if (pid > 0) { // 子节点
        write(pipe_write, buf, MSGSIZE);
    }
    if (pid == 0) { // 孙节点
        prime(pipe_read, pipe_write);
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);

    char nums[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++) {
        nums[i] = ONE;
    }

    int pid = fork();

    if (pid > 0) {
        nums[0] = ZERO;
        nums[1] = ZERO;
        write(fd[1], nums, MSGSIZE);
        wait(0);
    }

    if (pid == 0) {
        prime(fd[0], fd[1]);
        wait(0);
    }

    exit(0);
}