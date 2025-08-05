#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LINE 512  // 定义每行最大长度

int main(int argc, char *argv[]) {
    char line[MAX_LINE];  // 存储输入行的缓冲区
    char *args[MAXARG];   // 存储命令参数的数组
    char ch;              // 当前读取的字符
    int pos = 0;          // 当前行中的位置
    int i;

    // 检查参数数量
    if (argc < 2) {
        fprintf(2, "用法: xargs 命令 [参数...]\n");
        exit(1);
    }

    // 设置初始参数（命令及其参数）
    for (i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }

    // 读取标准输入
    while (read(0, &ch, 1) > 0) {
        if (ch == '\n') {
            line[pos] = 0;       // 行尾添加终止符
            args[argc-1] = line; // 将行内容作为最后一个参数
            args[argc] = 0;      // 参数数组结尾置空

            // 创建子进程执行命令
            if (fork() == 0) {
                exec(args[0], args);
                fprintf(2, "执行 %s 失败\n", args[0]);
                exit(1);
            } else {
                wait(0);  // 父进程等待子进程结束
                pos = 0;  // 重置行位置
            }
        } else if (pos < MAX_LINE - 1) {
            line[pos++] = ch;  // 存储字符到行缓冲区
        }
    }

    exit(0);
}