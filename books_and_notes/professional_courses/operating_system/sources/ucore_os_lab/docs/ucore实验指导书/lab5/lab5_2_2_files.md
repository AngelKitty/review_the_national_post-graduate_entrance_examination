
### 2.2 项目组成 

```
├── boot  
├── kern   
│ ├── debug  
│ │ ├── kdebug.c   
│ │ └── ……  
│ ├── mm  
│ │ ├── memlayout.h   
│ │ ├── pmm.c  
│ │ ├── pmm.h  
│ │ ├── ......  
│ │ ├── vmm.c  
│ │ └── vmm.h  
│ ├── process  
│ │ ├── proc.c  
│ │ ├── proc.h  
│ │ └── ......  
│ ├── schedule  
│ │ ├── sched.c  
│ │ └── ......  
│ ├── sync  
│ │ └── sync.h   
│ ├── syscall  
│ │ ├── syscall.c  
│ │ └── syscall.h  
│ └── trap  
│ ├── trap.c  
│ ├── trapentry.S  
│ ├── trap.h  
│ └── vectors.S  
├── libs  
│ ├── elf.h  
│ ├── error.h  
│ ├── printfmt.c  
│ ├── unistd.h  
│ └── ......  
├── tools  
│ ├── user.ld  
│ └── ......  
└── user  
├── hello.c  
├── libs  
│ ├── initcode.S  
│ ├── syscall.c  
│ ├── syscall.h  
│ └── ......  
└── ......  
```

相对与实验四，实验五主要增加的文件如上表红色部分所示，主要修改的文件如上表紫色部分所示。主要改动如下：

◆  kern/debug/

kdebug.c：修改：解析用户进程的符号信息表示（可不用理会）

◆  kern/mm/ （与本次实验有较大关系）

memlayout.h：修改：增加了用户虚存地址空间的图形表示和宏定义 （需仔细理解）。

pmm.[ch]：修改：添加了用于进程退出（do\_exit）的内存资源回收的page\_remove\_pte、unmap\_range、exit\_range函数和用于创建子进程（do\_fork）中拷贝父进程内存空间的copy\_range函数，修改了pgdir\_alloc\_page函数

vmm.[ch]：修改：扩展了mm\_struct数据结构，增加了一系列函数

* mm\_map/dup\_mmap/exit\_mmap：设定/取消/复制/删除用户进程的合法内存空间

* copy\_from\_user/copy\_to\_user：用户内存空间内容与内核内存空间内容的相互拷贝的实现

* user\_mem\_check：搜索vma链表，检查是否是一个合法的用户空间范围

◆  kern/process/ （与本次实验有较大关系）

proc.[ch]：修改：扩展了proc\_struct数据结构。增加或修改了一系列函数

* setup\_pgdir/put\_pgdir：创建并设置/释放页目录表

* copy\_mm：复制用户进程的内存空间和设置相关内存管理（如页表等）信息

* do\_exit：释放进程自身所占内存空间和相关内存管理（如页表等）信息所占空间，唤醒父进程，好让父进程收了自己，让调度器切换到其他进程

* load\_icode：被do\_execve调用，完成加载放在内存中的执行程序到进程空间，这涉及到对页表等的修改，分配用户栈

* do\_execve：先回收自身所占用户空间，然后调用load\_icode，用新的程序覆盖内存空间，形成一个执行新程序的新进程

* do\_yield：让调度器执行一次选择新进程的过程

* do\_wait：父进程等待子进程，并在得到子进程的退出消息后，彻底回收子进程所占的资源（比如子进程的内核栈和进程控制块）

* do\_kill：给一个进程设置PF\_EXITING标志（“kill”信息，即要它死掉），这样在trap函数中，将根据此标志，让进程退出

* KERNEL\_EXECVE/\_\_KERNEL\_EXECVE/\_\_KERNEL\_EXECVE2：被user\_main调用，执行一用户进程

◆  kern/trap/

trap.c：修改：在idt\_init函数中，对IDT初始化时，设置好了用于系统调用的中断门（idt[T\_SYSCALL]）信息。这主要与syscall的实现相关

◆  user/\*

新增的用户程序和用户库
