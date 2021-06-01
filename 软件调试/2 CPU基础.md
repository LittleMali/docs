# 2 CPU基础
## 2.1 指令和指令集
CPU支持的指令集合称为称为指令集。指令集一般分为两种：RISC（Reduced Instruction Set Computer）和CISC（Complex Instruction Set Computer）。典型的ARM是RISC，Intel X86是CISC。

RISC与CISC的差异主要有：
* RISC是4字节定长的，CISC是变长字节的，所以，RISC相对而言更好优化。
* RISC的指令比较少，比如，寻址方式，跳转指令简单等。
* RISC的通用寄存器更多，32位X86 CPU只有8个通用寄存器，而CISC通常有32个寄存器，所以，RISC的函数调用通常都使用寄存器传参，而CISC的函数调用更多使用栈传参，即使是fast call才仅仅使用了2个寄存器。
* Intel X86对CISC指令进行优化，汇编指令会先被翻译成等长的微操作，然后再执行。

### 2.1.1 CISC寻址方式
指令由操作码和操作数两部分构成，寻址方式是指如何获取操作数。常见的寻址方式有：
* 立即寻址。操作数是常量，直接拿过来使用，例如：MOV AX, 5，那么MOV的结果是AX=5。
* 直接寻址。操作数放在有效地址（EA，Effective Address）指向的内存中。在实模式下，访问内存使用的是“EA=段基址：段内偏移”方式。默认情况下，数据段基址是ds寄存器。直接寻址的EA实际是段内偏移。
  
  例如，MOV AX, [1234H] 等价于 MOV AX, DS:[1234H]。若DS=2000H，那么，物理地址实际是 2000H<<4 + 1234H = 21234H（DS<<4 + 偏移)，若物理地址中21234H存放的是5，那mov之后的AX=5 。
* 寄存器直接寻址。操作数放在寄存器中，访问寄存器拿到真正的数字，例如：MOV AX, BX。若AX=1234H，BH=5678H，那么MOV之后，AX的结果是5678H。
* 寄存器间接寻址。 操作数放在EA指向的内存中，EA=段基址：段偏移。若指令中使用的是BX，SI，DI，那么对应的基址是DS。若指令中使用的是BP，那么段基址是SS。
  
  EA = DS<<4 + BX/SI/DI

  EA = SS<<4 + BP

  例如： MOV AX, [BX] 等价于 MOV AX, DS:[BX]，若DS=2000H，BX=1234H，那么，EA=21234H，EA内存中的值会装载进AX中。
* 基址寻址/变址寻址。基址寻址类似于寄存器间接寻址。当使用基址寄存器（BX/BP）时，称为基址寻址，当使用变址寄存器（SI/DI）时，称为变址寻址。
  
  EA = DS<<4 + BX/SI/DI + 偏移

  EA = SS<<4 + BP + 偏移

  例如，MOV AX, 10H[BX] 等价于 MOV AX, DS:[BX + 10H] 。若DS=2000H，BX=20H，那么，EA=2000H << 4 + 20H + 10H = 20030H，EA内存中的值会装载进AX中。
* 基址变址寻址。可以认为上面的 基址寻址/变址寻址是 基址变址寻址 的特例。

    MOV	AX, 10H[BX][SI] 等价于 MOV AX, DS:10H[BX][SI] 。 EA=DS << 4 + BX + SI + 10H 。
	MOV	20H[BP][DI], CX 等价于 MOV SS:20H[BP][DI], CX
* 串操作寻址。 隐含使用SI和DI，用于字符串遍历。源操作数由DS:[SI]提供，目的操作数由DS:[DI]提供，每次操作完成后，SI和DI指向下一个元素。
* IO寻址。 CPU与外设通信的方式。例如， IN AL, PORT ， OUT PORT, AL 。