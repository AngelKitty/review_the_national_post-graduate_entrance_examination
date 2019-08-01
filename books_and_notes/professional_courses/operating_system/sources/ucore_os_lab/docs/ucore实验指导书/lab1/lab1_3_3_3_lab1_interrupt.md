
#### 3.3.3 lab1中对中断的处理实现

(1)	外设基本初始化设置

Lab1实现了中断初始化和对键盘、串口、时钟外设进行中断处理。串口的初始化函数serial_init（位于/kern/driver/console.c）中涉及中断初始化工作的很简单：

```
......
// 使能串口1接收字符后产生中断
	outb(COM1 + COM_IER, COM_IER_RDI);
......
// 通过中断控制器使能串口1中断
pic_enable(IRQ_COM1);
```
键盘的初始化函数kbd_init（位于kern/driver/console.c中）完成了对键盘的中断初始化工作，具体操作更加简单：

```
......
// 通过中断控制器使能键盘输入中断
pic_enable(IRQ_KBD);
```
时钟是一种有着特殊作用的外设，其作用并不仅仅是计时。在后续章节中将讲到，正是由于有了规律的时钟中断，才使得无论当前CPU运行在哪里，操作系统都可以在预先确定的时间点上获得CPU控制权。这样当一个应用程序运行了一定时间后，操作系统会通过时钟中断获得CPU控制权，并可把CPU资源让给更需要CPU的其他应用程序。时钟的初始化函数clock_init（位于kern/driver/clock.c中）完成了对时钟控制器8253的初始化：

```
	......
//设置时钟每秒中断100次
	outb(IO_TIMER1, TIMER_DIV(100) % 256);
	outb(IO_TIMER1, TIMER_DIV(100) / 256);
// 通过中断控制器使能时钟中断
	pic_enable(IRQ_TIMER);
```
(2)	中断初始化设置

操作系统如果要正确处理各种不同的中断事件，就需要安排应该由哪个中断服务例程负责处理特定的中断事件。系统将所有的中断事件统一进行了编号（0～255），这个编号称为中断向量。以ucore为例，操作系统内核启动以后，会通过 idt_init 函数初始化 idt 表 (参见trap.c)，而其中 vectors 中存储了中断处理程序的入口地址。vectors 定义在 vector.S 文件中，通过一个工具程序 vector.c 生成。其中仅有 System call 中断的权限为用户权限 (DPL_USER)，即仅能够使用 int 0x30 指令。此外还有对 tickslock 的初始化，该锁用于处理时钟中断。

vector.S 文件通过 vectors.c 自动生成，其中定义了每个中断的入口程序和入口地址 (保存在 vectors 数组中)。其中，中断可以分成两类：一类是压入错误编码的 (error code)，另一类不压入错误编码。对于第二类， vector.S 自动压入一个 0。此外，还会压入相应中断的中断号。在压入两个必要的参数之后，中断处理函数跳转到统一的入口 alltraps 处。

(3)	中断的处理过程

trap函数（定义在trap.c中）是对中断进行处理的过程，所有的中断在经过中断入口函数__alltraps预处理后 (定义在 trapasm.S中) ，都会跳转到这里。在处理过程中，根据不同的中断类型，进行相应的处理。在相应的处理过程结束以后，trap将会返回，被中断的程序会继续运行。整个中断处理流程大致如下：

<table>
<tr><td>trapasm.S</td><td>trap.c</td></tr>
<tr>
<td width="50%">
1)产生中断后，CPU 跳转到相应的中断处理入口 (vectors)，并在桟中压入相应的 error_code（是否存在与异常号相关） 以及 trap_no，然后跳转到 alltraps 函数入口：
<br>
注意：此处的跳转是 jmp 过程
<table>
<tr><td>(high)</td><td>...</td></tr>
<tr><td>产生中断时的 eip →</td><td>eip</td></tr>
<tr><td> </td><td>error_code</td></tr>
<tr><td>esp →</td><td>trap_no</td></tr>
<tr><td>(low)</td><td>...</td></tr>
</table>
<br>
在栈中保存当前被打断程序的 trapframe 结构(参见过程trapasm.S)。设置 kernel (内核) 的数据段寄存器，最后压入 esp，作为 trap 函数参数(struct trapframe * tf) 并跳转到中断处理函数 trap 处：
<table>
<tr>
<td width="50%">
Struct trapframe<br>
{<br>
uint edi;<br>
uint esi;<br>
uint ebp;<br>
…<br>
ushort es;<br>
ushort padding1;<br>
ushort ds;<br>
ushort padding2;<br>
uint trapno;<br>
uint err;<br>
uint eip;<br>
...<br>
}
</td>
<td width="50%">
观察 trapframe 结构与中断产生过程的压桟顺序。<br>
需要明确 pushal 指令都保存了哪些寄存器，按照什么顺序？<br>
<br>
<br>
<br>
← trap_no<br>
← trap_error<br>
← 产生中断处的 eip<br>
</td></tr>
</table>
<br>
注意：此时的跳转是 call 调用，会压入返回地址 eip，注意区分此处eip与trapframe中eip：
<br>
trapframe的结构为：
<br>
进入 trap 函数，对中断进行相应的处理：
</td>
<td></td>
</tr>
<td></td>
<td>
2)详细的中断分类以及处理流程如下：
<br>
根据中断号对不同的中断进行处理。其中，若中断号是IRQ_OFFSET + IRQ_TIMER 为时钟中断，则把ticks 将增加一。
<br>
若中断号是IRQ_OFFSET + IRQ_COM1 为串口中断，则显示收到的字符。
<br>
若中断号是IRQ_OFFSET + IRQ_KBD 为键盘中断，则显示收到的字符。
<br>
若为其他中断且产生在内核状态，则挂起系统；
</td>
<tr>
<td>
3)结束 trap 函数的执行后，通过 ret 指令返回到 alltraps 执行过程。
<br>
从栈中恢复所有寄存器的值。
<br>
调整 esp 的值：跳过栈中的 trap_no 与 error_code，使esp指向中断返回 eip，通过 iret 调用恢复 cs、eflag以及 eip，继续执行。
</td>
<td></td>
</tr>
<tr>
</tr>
</table>

图13 ucore中断处理流程

至此，对整个lab1中的主要部分的背景知识和实现进行了阐述。请大家能够根据前面的练习要求完成所有的练习。
