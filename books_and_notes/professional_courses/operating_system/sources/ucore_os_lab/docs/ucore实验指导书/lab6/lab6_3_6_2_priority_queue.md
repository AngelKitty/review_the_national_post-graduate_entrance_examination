
#### 3.6.2 使用优先队列实现 Stride Scheduling 

在上述的实现描述中，对于每一次pick\_next函数，我们都需要完整地扫描来获得当前最小的stride及其进程。这在进程非常多的时候是非常耗时和低效的，有兴趣的同学可以在实现了基于列表扫描的Stride调度器之后比较一下priority程序在Round-Robin及Stride调度器下各自的运行时间。考虑到其调度选择于优先队列的抽象逻辑一致，我们考虑使用优化的优先队列数据结构实现该调度。

优先队列是这样一种数据结构：使用者可以快速的插入和删除队列中的元素，并且在预先指定的顺序下快速取得当前在队列中的最小（或者最大）值及其对应元素。可以看到，这样的数据结构非常符合 Stride 调度器的实现。

本次实验提供了libs/skew\_heap.h
作为优先队列的一个实现，该实现定义相关的结构和接口，其中主要包括：

```
1   // 优先队列节点的结构
2   typedef struct skew_heap_entry  skew_heap_entry_t;
3   // 初始化一个队列节点
4   void skew_heap_init(skew_heap_entry_t *a);
5   // 将节点 b 插入至以节点 a 为队列头的队列中去，返回插入后的队列
6   skew_heap_entry_t  *skew_heap_insert(skew_heap_entry_t  *a,
7                                        skew_heap_entry_t  *b,
8                                        compare_f comp);
9   // 将节点 b 插入从以节点 a 为队列头的队列中去，返回删除后的队列
10      skew_heap_entry_t  *skew_heap_remove(skew_heap_entry_t  *a,
11                                           skew_heap_entry_t  *b,
12                                           compare_f comp);
```

其中优先队列的顺序是由比较函数comp决定的，sched\_stride.c中提供了proc\_stride\_comp\_f比较器用来比较两个stride的大小，你可以直接使用它。当使用优先队列作为Stride调度器的实现方式之后，运行队列结构也需要作相关改变，其中包括：

* struct
run\_queue中的lab6\_run\_pool指针，在使用优先队列的实现中表示当前优先队列的头元素，如果优先队列为空，则其指向空指针（NULL）。

* struct
proc\_struct中的lab6\_run\_pool结构，表示当前进程对应的优先队列节点。本次实验已经修改了系统相关部分的代码，使得其能够很好地适应LAB6新加入的数据结构和接口。而在实验中我们需要做的是用优先队列实现一个正确和高效的Stride调度器，如果用较简略的伪代码描述，则有：

* init(rq):   
– Initialize rq-\>run\_list   
– Set rq-\>lab6\_run\_pool to NULL   
– Set rq-\>proc\_num to 0   

* enqueue(rq, proc)   
– Initialize proc-\>time\_slice    
– Insert proc-\>lab6\_run\_pool into rq-\>lab6\_run\_pool   
– rq-\>proc\_num ++   

* dequeue(rq, proc)   
– Remove proc-\>lab6\_run\_pool from rq-\>lab6\_run\_pool    
– rq-\>proc\_num --   

* pick\_next(rq)   
– If rq-\>lab6\_run\_pool == NULL, return NULL   
– Find the proc corresponding to the pointer rq-\>lab6\_run\_pool   
– proc-\>lab6\_stride += BIG\_STRIDE / proc-\>lab6\_priority   
– Return proc   

* proc\_tick(rq, proc):   
– If proc-\>time\_slice \> 0, proc-\>time\_slice --   
– If proc-\>time\_slice == 0, set the flag proc-\>need\_resched    
