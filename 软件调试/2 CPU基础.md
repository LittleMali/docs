# 2 CPU基础
## 2.1 指令和指令集
CPU支持的指令集合称为称为指令集。指令集一般分为两种：RISC（Reduced Instruction Set Computer）和CISC（Complex Instruction Set Computer）。典型的ARM是RISC，Intel X86是CISC。

RISC与CISC的差异主要有：
* RISC是4字节定长的，CISC是变长字节的，所以，RISC相对而言更好优化。
* RISC的指令比较少，比如，寻址方式，跳转指令简单等。
* RISC的通用寄存器更多，32位X86 CPU只有8个通用寄存器，而CISC通常有32个寄存器，所以，RISC的函数调用通常都使用寄存器传参，而CISC的函数调用更多使用栈传参，即使是fast call才仅仅使用了2个寄存器。
* Intel X86对CISC指令进行优化，汇编指令会先被翻译成等长的微操作，然后再执行。

### 2.1.1 CISI寻址方式
