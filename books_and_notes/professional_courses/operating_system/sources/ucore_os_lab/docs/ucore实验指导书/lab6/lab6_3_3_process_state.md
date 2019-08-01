
### 3.3 进程状态 

在此次实验中，进程的状态之间的转换需要有一个更为清晰的表述，在 ucore中，runnable的进程会被放在运行队列中。值得注意的是，在具体实现中，ucore定义的进程控制块struct proc\_struct包含了成员变量state,用于描述进程的运行状态，而running和runnable共享同一个状态(state)值(PROC\_RUNNABLE。不同之处在于处于running态的进程不会放在运行队列中。进程的正常生命周期如下：

* 进程首先在 cpu 初始化或者 sys\_fork 的时候被创建，当为该进程分配了一个进程描 述符之后，该进程进入 uninit态(在proc.c 中 alloc\_proc)。
* 当进程完全完成初始化之后，该进程转为runnable态。
* 当到达调度点时，由调度器 sched\_class 根据运行队列rq的内容来判断一个进程是否应该被运行，即把处于runnable态的进程转换成running状态，从而占用CPU执行。
* running态的进程通过wait等系统调用被阻塞，进入sleeping态。
* sleeping态的进程被wakeup变成runnable态的进程。
* running态的进程主动 exit 变成zombie态，然后由其父进程完成对其资源的最后释放，子进程的进程控制块成为unused。
* 所有从runnable态变成其他状态的进程都要出运行队列，反之，被放入某个运行队列中。
