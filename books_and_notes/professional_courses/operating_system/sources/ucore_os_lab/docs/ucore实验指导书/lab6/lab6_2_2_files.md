
### 2.2 项目组成 

```
├── boot  
├── kern  
│ ├── debug  
│ ├── driver  
│ ├── fs  
│ ├── init  
│ ├── libs  
│ ├── mm  
│ ├── process  
│ │ ├── .....  
│ │ ├── proc.c  
│ │ ├── proc.h   
│ │ └── switch.S  
│ ├── schedule  
│ │ ├── default\_sched.c  
│ │ ├── default\_sched.h  
│ │ ├── default\_sched\_stride\_c  
│ │ ├── sched.c  
│ │ └── sched.h   
│ ├── syscall   
│ │ ├── syscall.c   
│ │ └── syscall.h  
…   
```

相对与实验五，实验六主要增加的文件如上表红色部分所示，主要修改的文件如上表紫色部分所示。主要改动如下：
简单说明如下：
* libs/skew\_heap.h:
提供了基本的优先队列数据结构，为本次实验提供了抽象数据结构方面的支持。
* kern/process/proc.[ch]：proc.h中扩展了proc\_struct的成员变量，用于RR和stride调度算法。proc.c中实现了lab6\_set\_priority，用于设置进程的优先级。
* kern/schedule/{sched.h,sched.c}: 定义了 ucore 的调度器框架，其中包括相关的数据结构（包括调度器的接口和运行队列的结构），和具体的运行时机制。
* kern/schedule/{default\_sched.h,default\_sched.c}: 具体的 round-robin 算法，在本次实验中你需要了解其实现。
* kern/schedule/default\_sched\_stride\_c: Stride Scheduling调度器的基本框架，在此次实验中你需要填充其中的空白部分以实现一个完整的 Stride 调度器。
* kern/syscall/syscall.[ch]: 增加了sys\_gettime系统调用，便于用户进程获取当前时钟值；增加了sys\_lab6\_set\_priority系统调用，便于用户进程设置进程优先级（给priority.c用）
* user/{matrix.c,priority.c,. . . }: 相关的一些测试用户程序，测试调度算法的正确性，user目录下包含但不限于这些程序。在完成实验过程中，建议阅读这些测试程序，以了解这些程序的行为，便于进行调试。
