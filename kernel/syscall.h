// System call numbers
#define SYS_fork 1
#define SYS_exit 2
#define SYS_wait 3
#define SYS_pipe 4
#define SYS_read 5
#define SYS_kill 6
#define SYS_exec 7
#define SYS_fstat 8
#define SYS_chdir 9
#define SYS_dup 10
#define SYS_getpid 11
#define SYS_sbrk 12
#define SYS_sleep 13
#define SYS_uptime 14
#define SYS_open 15
#define SYS_write 16
#define SYS_mknod 17
#define SYS_unlink 18
#define SYS_link 19
#define SYS_mkdir 20
#define SYS_close 21
#define SYS_trace 22

// 问题1:为什么系统调用使用二进制左移，而不是使用整数
/*
1. 位掩码组合：左移生成的数值（如 1 <<
n）可以独立占用二进制位，便于通过位操作（如按位或
|）组合多个标志位，避免数值冲突。

2. 扩展性：左移操作通过递增位移位数（如 1 << 0、1 <<
1）分配唯一值，新增系统调用时无需手动计算整数，代码可读性更强。

3.
性能高效：左移是硬件级指令（单时钟周期），比直接使用大整数更高效，适合高频调用的系统场景。

4.
硬件兼容性：某些架构（如x86）的系统调用指令要求参数为紧凑位字段，左移生成的数值天然适配。

5. 避免魔术数字：1 << n 明确表达数值生成逻辑，比直接写整数（如 15）更易维护。
*/