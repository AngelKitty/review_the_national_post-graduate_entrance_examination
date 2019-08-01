**链接地址/虚地址/物理地址/加载地址以及edata/end/text的含义**

**链接脚本简介**

ucore
kernel各个部分由组成kernel的各个.o或.a文件构成，且各个部分在内存中地址位置由ld工具根据kernel.ld链接脚本（linker
script）来设定。ld工具使用命令-T指定链接脚本。链接脚本主要用于规定如何把输入文件（各个.o或.a文件）内的section放入输出文件（lab2/bin/kernel，即ELF格式的ucore内核）内，
并控制输出文件内各部分在程序地址空间内的布局。下面简单分析一下/lab2/tools/kernel.ld，来了解一下ucore内核的地址布局情况。kernel.ld的内容如下所示：
```
/* Simple linker script for the ucore kernel.
   See the GNU ld 'info' manual ("info ld") to learn the syntax. */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kern_entry)

SECTIONS {
    /* Load the kernel at this address: "." means the current address */
    . = 0xC0100000;

    .text : {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    PROVIDE(etext = .); /* Define the 'etext' symbol to this value */

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /* Include debugging information in kernel memory */
    .stab : {
        PROVIDE(__STAB_BEGIN__ = .);
        *(.stab);
        PROVIDE(__STAB_END__ = .);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }

    .stabstr : {
        PROVIDE(__STABSTR_BEGIN__ = .);
        *(.stabstr);
        PROVIDE(__STABSTR_END__ = .);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }

    /* Adjust the address for the data segment to the next page */
    . = ALIGN(0x1000);

    /* The data segment */
    .data : {
        *(.data)
    }

    PROVIDE(edata = .);

    .bss : {
        *(.bss)
    }

    PROVIDE(end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}
```
其实从链接脚本的内容，可以大致猜出它指定告诉链接器的各种信息：

* 内核加载地址：0xC0100000
* 入口（起始代码）地址： ENTRY(kern\_entry)
* cpu机器类型：i386

其最主要的信息是告诉链接器各输入文件的各section应该怎么组合：应该从哪个地址开始放，各个section以什么顺序放，分别怎么对齐等等，最终组成输出文件的各section。除此之外，linker
script还可以定义各种符号（如.text、.data、.bss等），形成最终生成的一堆符号的列表（符号表），每个符号包含了符号名字，符号所引用的内存地址，以及其他一些属性信息。符号实际上就是一个地址的符号表示，其本身不占用的程序运行的内存空间。

**链接地址/加载地址/虚地址/物理地址**

ucore 设定了ucore运行中的虚地址空间，具体设置可看
lab2/kern/mm/memlayout.h 中描述的"Virtual memory map
"图，可以了解虚地址和物理地址的对应关系。lab2/tools/kernel.ld描述的是执行代码的链接地址（link\_addr），比如内核起始地址是0xC0100000，这是一个虚地址。所以我们可以认为链接地址等于虚地址。在ucore建立内核页表时，设定了物理地址和虚地址的虚实映射关系是：

phy addr + 0xC0000000 = virtual addr

即虚地址和物理地址之间有一个偏移。但boot loader把ucore
kernel加载到内存时，采用的是加载地址（load
addr），这是由于ucore还没有运行，即还没有启动页表映射，导致这时采用的寻址方式是段寻址方式，用的是boot
loader在初始化阶段设置的段映射关系，其映射关系（可参看bootasm.S的末尾处有关段描述符表的内容）是：

linear addr = phy addr = virtual addr

查看 bootloader的实现代码 bootmain::bootmain.c

readseg(ph-\>p\_va & 0xFFFFFF, ph-\>p\_memsz, ph-\>p\_offset);

这里的ph-\>p\_va=0xC0XXXXXX，就是ld工具根据kernel.ld设置的链接地址，且链接地址等于虚地址。考虑到ph-\>p\_va
& 0xFFFFFF == 0x0XXXXXX，所以bootloader加载ucore
kernel的加载地址是0x0XXXXXX, 这实际上是ucore内核所在的物理地址。简言之：
OS的链接地址（link addr） 在tools/kernel.ld中设置好了，是一个虚地址（virtual
addr）；而ucore kernel的加载地址（load addr）在boot
loader中的bootmain函数中指定，是一个物理地址。

小结一下，ucore内核的链接地址==ucore内核的虚拟地址；boot
loader加载ucore内核用到的加载地址==ucore内核的物理地址。

**edata/end/text的含义**

在基于ELF执行文件格式的代码中，存在一些对代码和数据的表述，基本概念如下：

* BSS段（bss
segment）：指用来存放程序中未初始化的全局变量的内存区域。BSS是英文Block
Started by Symbol的简称。BSS段属于静态内存分配。
* 数据段（data
segment）：指用来存放程序中已初始化的全局变量的一块内存区域。数据段属于静态内存分配。
* 代码段（code segment/text
segment）：指用来存放程序执行代码的一块内存区域。这部分区域的大小在程序运行前就已经确定，并且内存区域通常属于只读,
某些架构也允许代码段为可写，即允许修改程序。在代码段中，也有可能包含一些只读的常数变量，例如字符串常量等。

在lab2/kern/init/init.c的kern\_init函数中，声明了外部全局变量：
```
extern char edata[], end[];
```
但搜寻所有源码文件\*.[ch]，没有发现有这两个变量的定义。那这两个变量从哪里来的呢？其实在lab2/tools/kernel.ld中，可以看到如下内容：
```
…
.text : {
        *(.text .stub .text.* .gnu.linkonce.t.*)
}
…
    .data : {
        *(.data)
}
…
PROVIDE(edata = .);
…
    .bss : {
        *(.bss)
}
…
PROVIDE(end = .);
…
```
这里的“.”表示当前地址，“.text”表示代码段起始地址，“.data”也是一个地址，可以看出，它即代表了代码段的结束地址，也是数据段的起始地址。类推下去，“edata”表示数据段的结束地址，“.bss”表示数据段的结束地址和BSS段的起始地址，而“end”表示BSS段的结束地址。

这样回头看kerne\_init中的外部全局变量，可知edata[]和
end[]这些变量是ld根据kernel.ld链接脚本生成的全局变量，表示相应段的起始地址或结束地址等，它们不在任何一个.S、.c或.h文件中定义。



