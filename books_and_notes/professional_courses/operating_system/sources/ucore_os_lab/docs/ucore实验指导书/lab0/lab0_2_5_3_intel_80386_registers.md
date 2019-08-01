
#### 2.5.3 Intel 80386寄存器

这里假定读者对80386 CPU有一定的了解，所以只作简单介绍。80386的寄存器可以分为8组：通用寄存器，段寄存器，指令指针寄存器，标志寄存器，系统地址寄存器，控制寄存器，调试寄存器，测试寄存器，它们的宽度都是32位。一般程序员看到的寄存器包括通用寄存器，段寄存器，指令指针寄存器，标志寄存器。

General Register(通用寄存器)：EAX/EBX/ECX/EDX/ESI/EDI/ESP/EBP这些寄存器的低16位就是8086的 AX/BX/CX/DX/SI/DI/SP/BP，对于AX,BX,CX,DX这四个寄存器来讲,可以单独存取它们的高8位和低8位 (AH,AL,BH,BL,CH,CL,DH,DL)。它们的含义如下:
```
	EAX：累加器
	EBX：基址寄存器
	ECX：计数器
	EDX：数据寄存器
	ESI：源地址指针寄存器
	EDI：目的地址指针寄存器
	EBP：基址指针寄存器
	ESP：堆栈指针寄存器
```

![通用寄存器](../lab0_figs/image003.png "通用寄存器")

Segment Register(段寄存器，也称 Segment Selector，段选择符，段选择子)：除了8086的4个段外(CS,DS,ES,SS)，80386还增加了两个段FS，GS,这些段寄存器都是16位的，用于不同属性内存段的寻址，它们的含义如下：
```
	CS：代码段(Code Segment)
	DS：数据段(Data Segment)
	ES：附加数据段(Extra Segment)
	SS：堆栈段(Stack Segment)
	FS：附加段
	GS 附加段
```

![段寄存器](../lab0_figs/image004.png "段寄存器")

Instruction Pointer(指令指针寄存器)：EIP的低16位就是8086的IP，它存储的是下一条要执行指令的内存地址，在分段地址转换中，表示指令的段内偏移地址。
 
![状态和指令寄存器](../lab0_figs/image005.png "状态和指令寄存器")

Flag Register(标志寄存器)：EFLAGS,和8086的16位标志寄存器相比，增加了4个控制位，这20位控制/标志位的位置如下图所示：
 
![状态寄存器](../lab0_figs/image006.png "状态寄存器")

相关的控制/标志位含义是：
```
	CF(Carry Flag)：进位标志位；
	PF(Parity Flag)：奇偶标志位；
	AF(Assistant Flag)：辅助进位标志位；
	ZF(Zero Flag)：零标志位；
	SF(Singal Flag)：符号标志位；
	IF(Interrupt Flag)：中断允许标志位,由CLI，STI两条指令来控制；设置IF位使CPU可识别外部（可屏蔽）中断请求，复位IF位则禁止中断，IF位对不可屏蔽外部中断和故障中断的识别没有任何作用；
	DF(Direction Flag)：向量标志位，由CLD，STD两条指令来控制；
	OF(Overflow Flag)：溢出标志位；
	IOPL(I/O Privilege Level)：I/O特权级字段，它的宽度为2位,它指定了I/O指令的特权级。如果当前的特权级别在数值上小于或等于IOPL，那么I/O指令可执行。否则，将发生一个保护性故障中断；
	NT(Nested Task)：控制中断返回指令IRET，它宽度为1位。若NT=0，则用堆栈中保存的值恢复EFLAGS，CS和EIP从而实现中断返回；若NT=1，则通过任务切换实现中断返回。在ucore中，设置NT为0。
``` 

还有一些应用程序无法访问的控制寄存器，如CR0,CR2，CR3...，将在后续章节逐一讲解。
