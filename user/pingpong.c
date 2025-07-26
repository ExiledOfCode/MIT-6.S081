#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 1

int main() {
    int parent_to_child[2]; // 父进程→子进程的管道
    int child_to_parent[2]; // 子进程→父进程的管道
    char buf[BUF_SIZE];     // 读写缓冲区

    // 创建管道
    if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0) {
        fprintf(2, "pipe() failed\n");
        exit(1);
    }

    // 创建子进程
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork() failed\n");
        exit(1);
    }

    if (pid == 0) {                // 子进程
        close(parent_to_child[1]); // 关闭父进程的写端
        close(child_to_parent[0]); // 关闭自己的读端

        // 从父进程读取数据
        if (read(parent_to_child[0], buf, BUF_SIZE) != BUF_SIZE) {
            fprintf(2, "child read() error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        // 写回父进程
        if (write(child_to_parent[1], buf, BUF_SIZE) != BUF_SIZE) {
            fprintf(2, "child write() error\n");
            exit(1);
        }
        close(child_to_parent[1]);
        exit(0);

    } else {                       // 父进程
        close(parent_to_child[0]); // 关闭子进程的读端
        close(child_to_parent[1]); // 关闭自己的写端

        // 发送数据给子进程
        buf[0] = 'a';
        if (write(parent_to_child[1], buf, BUF_SIZE) != BUF_SIZE) {
            fprintf(2, "parent write() error\n");
            exit(1);
        }
        close(parent_to_child[1]);

        // 等待子进程返回
        if (read(child_to_parent[0], buf, BUF_SIZE) != BUF_SIZE) {
            fprintf(2, "parent read() error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}