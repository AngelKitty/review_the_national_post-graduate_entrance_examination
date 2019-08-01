
### 2.2 项目组成 

此次实验中，主要有如下一些需要关注的文件：

```
.  
├── boot  
├── kern   
│ ├── driver   
│ ├── fs   
│ ├── init  
│ ├── libs   
│ ├── mm   
│ │ ├── ......   
│ │ ├── vmm.c  
│ │ └── vmm.h   
│ ├── process   
│ │ ├── proc.c   
│ │ ├── proc.h  
│ │ └──......   
│ ├── schedule     
│ ├── sync  
│ │ ├── check\_sync.c  
│ │ ├── monitor.c   
│ │ ├── monitor.h    
│ │ ├── sem.c   
│ │ ├── sem.h   
│ │ ├── sync.h  
│ │ ├── wait.c   
│ │ └── wait.h   
│ ├── syscall   
│ │ ├── syscall.c    
│ │ └──......    
│ └── trap   
├── libs   
└── user   
├── forktree.c  
├── libs  
│ ├── syscall.c   
│ ├── syscall.h   
│ ├── ulib.c   
│ ├── ulib.h  
│ └── ......  
├── priority.c  
├── sleep.c  
├── sleepkill.c      
├── softint.c  
├── spin.c  
└── ......  
```

简单说明如下：

* kern/sync/sync.h: 去除了lock实现（这对于不抢占内核没用）。
* kern/sync/wait.[ch]:
定了为wait结构和waitqueue结构以及在此之上的函数，这是ucore中的信号量semophore机制和条件变量机制的基础，在本次实验中你需要了解其实现。
* kern/sync/sem.[ch]:定义并实现了ucore中内核级信号量相关的数据结构和函数，本次试验中你需要了解其中的实现，并基于此完成内核级条件变量的设计与实现。
* user/ libs/ {syscall.[ch],ulib.[ch]
}与kern/sync/syscall.c：实现了进程sleep相关的系统调用的参数传递和调用关系。
* user/{ sleep.c,sleepkill.c}: 进程睡眠相关的一些测试用户程序。
* kern/sync/monitor.[ch]:基于管程的条件变量的实现程序，在本次实验中是练习的一部分，要求完成。
* kern/sync/check\_sync.c：实现了基于管程的哲学家就餐问题，在本次实验中是练习的一部分，要求完成基于管程的哲学家就餐问题。
* kern/mm/vmm.[ch]：用信号量mm\_sem取代mm\_struct中原有的mm\_lock。（本次实验不用管）
