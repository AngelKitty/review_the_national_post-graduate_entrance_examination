**实现物理内存探测**

物理内存探测是在bootasm.S中实现的，相关代码很短，如下所示：
```
probe_memory:
//对0x8000处的32位单元清零,即给位于0x8000处的
//struct e820map的成员变量nr_map清零
           movl $0, 0x8000
                  xorl %ebx, %ebx
//表示设置调用INT 15h BIOS中断后，BIOS返回的映射地址描述符的起始地址
                  movw $0x8004, %di
start_probe:
                  movl $0xE820, %eax // INT 15的中断调用参数
//设置地址范围描述符的大小为20字节，其大小等于struct e820map的成员变量map的大小
                  movl $20, %ecx
//设置edx为534D4150h (即4个ASCII字符“SMAP”)，这是一个约定
                  movl $SMAP, %edx
//调用int 0x15中断，要求BIOS返回一个用地址范围描述符表示的内存段信息
                  int $0x15
//如果eflags的CF位为0，则表示还有内存段需要探测
                  jnc cont
//探测有问题，结束探测
                  movw $12345, 0x8000
                  jmp finish_probe
cont:
//设置下一个BIOS返回的映射地址描述符的起始地址
                  addw $20, %di
//递增struct e820map的成员变量nr_map
                  incl 0x8000
//如果INT0x15返回的ebx为零，表示探测结束，否则继续探测
                  cmpl $0, %ebx
                  jnz start_probe
finish_probe:
```
上述代码正常执行完毕后，在0x8000地址处保存了从BIOS中获得的内存分布信息，此信息按照struct
e820map的设置来进行填充。这部分信息将在bootloader启动ucore后，由ucore的page\_init函数来根据struct
e820map的memmap（定义了起始地址为0x8000）来完成对整个机器中的物理内存的总体管理。
