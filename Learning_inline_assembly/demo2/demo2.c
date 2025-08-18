#include <stdio.h>

int main() {

    int a = 15, b = 25, result = 0;
    // result = (a > b) ? (a + 10) : (b - 5)
    __asm__ volatile("movl %[a], %%eax;"
                     "cmpl %[b], %%eax;"
                     "jg greater;"
                     "subl $5, %%eax;"
                     "jmp end;"
                     "greater:"
                     "addl $10, %%eax;"
                     "end:"
                     "movl %%eax, %[result];"
                     : [result] "=r"(result)  // 输出：result
                     : [a] "r"(a), [b] "r"(b) // 输入：a, b
                     : "%eax", "cc"           // 破坏描述
    );

    return 0;
}

/*
修饰符：
"r"：通用寄存器（register）。编译器选择合适的寄存器（如%eax、%ebx）。适合高效操作，常用在输入/输出。
"m"：内存地址（memory）。操作数存储在内存中，适合直接访问变量地址。
"i"：立即数（immediate），如常量值。用于输入常量，如"i"(5)。
"g"：通用约束，允许编译器选择寄存器、内存或立即数。灵活但可能牺牲性能。
"f"：浮点寄存器（如x87浮点栈）。
"=r"：输出寄存器，只写。
"+r"：读写寄存器，允许修改输入变量。
*/

/*
CF（Carry Flag，进位标志）：

表示无符号数运算是否发生进位（加法）或借位（减法）。
置1场景：加法结果超出寄存器范围，或减法需要借位。
示例：5 - 7（无符号）会导致借位，CF=1。
用途：用于无符号数比较（如ja、jb跳转）或多字节算术运算。


ZF（Zero Flag，零标志）：

表示运算结果是否为0。
置1场景：结果为0（如cmp 5, 5）。
用途：用于检查相等性（如je跳转）。


SF（Sign Flag，符号标志）：

表示运算结果的符号（最高位）。
置1场景：结果为负数（最高位为1）。
用途：用于有符号数比较（如jge、jl）。


OF（Overflow Flag，溢出标志）：

表示有符号数运算是否溢出。
置1场景：加法/减法导致结果超出有符号数范围（如INT_MAX + 1）。
用途：用于有符号数比较（如jge、jl）。


PF（Parity Flag，奇偶标志）：

表示结果低8位中1的个数是否为偶数。
置1场景：1的个数为偶数。
用途：较少使用，主要用于校验。


AF（Auxiliary Carry Flag，辅助进位标志）：

表示低4位（BCD运算）是否发生进位。
用途：BCD（二进制编码十进制）运算，较少使用。
*/

/*
2.1 cmp指令变体
cmp指令根据操作数大小有不同变体：

cmpb：比较8位（byte）操作数，用于char类型或字节操作。
cmpw：比较16位（word）操作数，用于short类型。
cmpl：比较32位（long）操作数，用于int类型（你的代码中使用的变体）。
cmpq：比较64位（quadword）操作数，用于64位架构的long或指针。

2.2 语法（AT&T）
ccmpl src, dst  // 计算 dst - src，更新EFLAGS

src：源操作数（寄存器、内存、立即数）。
dst：目标操作数（寄存器或内存）。
AT&T vs Intel：AT&T语法中，src在前，dst在后；Intel语法相反（cmp dst, src）。
*/

/*
跳转语句
3.2 条件跳转
条件跳转根据EFLAGS标志决定是否跳转。以下是常见条件跳转指令，分为有符号比较和无符号比较：
3.2.1 有符号比较（基于SF、OF、ZF）
用于有符号整数（如int），依赖SF（符号标志）和OF（溢出标志）。

je（jump if equal）：ZF=1（相等）。

条件：op1 == op2。
示例：cmp %eax, %ebx; je equal;（如果%eax == %ebx，跳转）。


jne（jump if not equal）：ZF=0（不相等）。
jg（jump if greater）：ZF=0 且 SF=OF（大于）。
jge（jump if greater or equal）：SF=OF（大于或等于）。

示例：你的代码中jge greater;检查%eax >= %2。


jl（jump if less）：SF≠OF（小于）。
jle（jump if less or equal）：ZF=1 或 SF≠OF（小于或等于）。

3.2.2 无符号比较（基于CF、ZF）
用于无符号整数（如unsigned int），依赖CF（进位标志）和ZF。

ja（jump if above）：CF=0 且 ZF=0（大于）。
jae（jump if above or equal）：CF=0（大于或等于）。
jb（jump if below）：CF=1（小于）。
jbe（jump if below or equal）：CF=1 或 ZF=1（小于或等于）。

3.2.3 其他跳转

jo（jump if overflow）：OF=1（有符号溢出）。
jno（jump if no overflow）：OF=0。
js（jump if sign）：SF=1（结果为负）。
jp（jump if parity）：PF=1（奇偶校验）。
*/