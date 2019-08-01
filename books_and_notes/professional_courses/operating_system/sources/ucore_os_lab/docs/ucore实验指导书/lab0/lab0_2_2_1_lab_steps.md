
#### 2.2.1 开发OS lab实验的简单步骤

在某git server，比如 https://github.com/chyyuu/ucore_lab 可下载我们提供的lab1~lab8实验软件中，大致经过如下过程就可以完成使用。

1. 在学堂在线查看OS相关原理和labX的课程视频
1. 如果第一次做lab，需要建立lab试验环境，可采用基于virtualbox虚拟机的最简单方式完成
1. 阅读本次lab的[实验指导书](http://objectkuan.gitbooks.io/ucore-docs/)，了解本次lab的试验要求
1. 下载源码(可以直接在github下载，或通过git pull下载)
2. 进入各个OS实验工程目录 例如： cd labcodes/lab1
3. 根据实验要求阅读源码并修改代码（用各种代码分析工具和文本编辑器）
4. 并编译源码 例如执行：make
5. 如编译不过则返回步骤3
6. 如编译通过则测试是否基本正确，例如执行：make grade
7. 如果实现基本正确（即看到步骤6的输出存在不是 OK的情况）则返回步骤3
8. 如果实现基本正确（即看到步骤6的输出都是 OK）则生成实验提交软件包，例如执行：make handin
9. 对于本校学生，把生成的使用提交软件包和实验报告上传到指定的git server，便于助教和老师查看。
 
> 另外，可以通过”make qemu”让OS实验工程在qemu上运行；可以通过”make debug”或“make debug-nox “命令实现通过gdb远程调试 OS实验工程；通过"make grade"可以看自己完成的对错情况。
