
## 4 实验报告要求 

从git server网站上取得ucore_lab后，进入目录labcodes/lab6，完成实验要求的各个练习。在实验报告中回答所有练习中提出的问题。在目录labcodes/lab6下存放实验报告，实验报告文档命名为lab6-学堂在线ID.md。推荐用**markdown**格式。对于lab6中编程任务，完成编写之后，再通过git  push命令把代码同步回git server网站。最后请一定提前或按时提交到git server网站。

注意有“LAB6”的注释，主要是修改default\_sched\_swide\_c中的内容。代码中所有需要完成的地方（challenge除外）都有“LAB6”和“YOUR CODE”的注释，请在提交时特别注意保持注释，并将“YOUR CODE”替换为自己的学号，并且将所有标有对应注释的部分填上正确的代码。

## 附录：执行 priority大致的显示输出 

```
$ make run-priority
......
check_swap() succeeded!
++ setup timer interrupts
kernel_execve: pid = 2, name = "priority".
main: fork ok,now need to wait pids.
child pid 7, acc 2492000, time 2001
child pid 6, acc 1944000, time 2001
child pid 4, acc 960000, time 2002
child pid 5, acc 1488000, time 2003
child pid 3, acc 540000, time 2004
main: pid 3, acc 540000, time 2004
main: pid 4, acc 960000, time 2004
main: pid 5, acc 1488000, time 2004
main: pid 6, acc 1944000, time 2004
main: pid 7, acc 2492000, time 2004
main: wait pids over
stride sched correct result: 1 2 3 4 5
all user-mode processes have quit.
init check memory pass.
kernel panic at kern/process/proc.c:426:
    initproc exit.

Welcome to the kernel debug monitor!!
Type 'help' for a list of commands.
K> 
```
