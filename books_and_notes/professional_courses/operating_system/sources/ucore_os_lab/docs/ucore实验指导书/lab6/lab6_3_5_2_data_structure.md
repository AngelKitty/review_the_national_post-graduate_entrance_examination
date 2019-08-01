
#### 3.5.2 数据结构 

在理解框架之前，需要先了解一下调度器框架所需要的数据结构。

* 通常的操作系统中，进程池是很大的（虽然在 ucore 中，MAX\_PROCESS 很小）。在 ucore 中，调度器引入 run-queue（简称rq,即运行队列）的概念，通过链表结构管理进程。
* 由于目前 ucore 设计运行在单CPU上，其内部只有一个全局的运行队列，用来管理系统内全部的进程。
* 运行队列通过链表的形式进行组织。链表的每一个节点是一个list\_entry\_t,每个list\_entry\_t 又对应到了 struct proc\_struct \*,这其间的转换是通过宏 le2proc 来完成 的。具体来说，我们知道在 struct proc\_struct 中有一个叫 run\_link 的 list\_entry\_t，因此可以通过偏移量逆向找到对因某个 run\_list的 struct proc\_struct。即进程结构指针 proc = le2proc(链表节点指针, run\_link)。
* 为了保证调度器接口的通用性，ucore调度框架定义了如下接口，该接口中，几乎全部成员变量均为函数指针。具体的功能会在后面的框架说明中介绍。

```
1 	struct sched_class {
2		// 调度器的名字
3		const char *name;
4		// 初始化运行队列
5		void (*init) (struct run_queue *rq);
6		// 将进程 p 插入队列 rq
7		void (*enqueue) (struct run_queue *rq, struct proc_struct *p);
8		// 将进程 p 从队列 rq 中删除
9		void (*dequeue) (struct run_queue *rq, struct proc_struct *p);
10		// 返回 运行队列 中下一个可执行的进程
11		struct proc_struct* (*pick_next) (struct run_queue *rq);
12		// timetick 处理函数
13		void (*proc_tick)(struct  run_queue* rq, struct proc_struct* p);
14	};
```

* 此外，proc.h 中的 struct proc\_struct 中也记录了一些调度相关的信息：

```
1	struct proc_struct {
2		// . . .
3		// 该进程是否需要调度，只对当前进程有效
4		volatile bool need_resched;
5		// 该进程的调度链表结构，该结构内部的连接组成了 运行队列 列表
6		list_entry_t run_link;
7		// 该进程剩余的时间片，只对当前进程有效
8		int time_slice;
9		// round-robin 调度器并不会用到以下成员
10		// 该进程在优先队列中的节点，仅在 LAB6 使用
11		skew_heap_entry_t  lab6_run_pool;
12		// 该进程的调度优先级，仅在 LAB6 使用
13		uint32_t lab6_priority;
14		// 该进程的调度步进值，仅在 LAB6 使用
15		uint32_t lab6_stride;
16	};
```

在此次实验中，你需要了解 default\_sched.c中的实现RR调度算法的函数。在该文件中，你可以看到ucore 已经为 RR 调度算法创建好了一个名为 RR\_sched\_class 的调度策略类。

通过数据结构 struct run\_queue 来描述完整的 run\_queue（运行队列）。它的主要结构如下：

```
1	struct run_queue {
2		//其运行队列的哨兵结构，可以看作是队列头和尾
3		list_entry_t run_list;
4		//优先队列形式的进程容器，只在 LAB6 中使用
5		skew_heap_entry_t  *lab6_run_pool;
6		//表示其内部的进程总数
7		unsigned int proc_num;
8		//每个进程一轮占用的最多时间片
9		int max_time_slice;
10	};
```

在 ucore 框架中，运行队列存储的是当前可以调度的进程，所以，只有状态为runnable的进程才能够进入运行队列。当前正在运行的进程并不会在运行队列中，这一点需要注意。
