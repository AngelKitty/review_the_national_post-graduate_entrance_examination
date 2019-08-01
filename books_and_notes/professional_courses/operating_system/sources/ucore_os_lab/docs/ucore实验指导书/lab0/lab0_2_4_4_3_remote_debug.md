
##### 2.4.4.3 使用远程调试

为了与qemu配合进行源代码级别的调试，需要先让qemu进入等待gdb调试器的接入并且还不能让qemu中的CPU执行，因此启动qemu的时候，我们需要使用参数-S –s这两个参数来做到这一点。在使用了前面提到的参数启动qemu之后，qemu中的CPU并不会马上开始执行，这时我们启动gdb，然后在gdb命令行界面下，使用下面的命令连接到qemu：

	(gdb)  target remote 127.0.0.1:1234

然后输入c（也就是continue）命令之后，qemu会继续执行下去，但是gdb由于不知道任何符号信息，并且也没有下断点，是不能进行源码级的调试的。为了让gdb获知符号信息，需要指定调试目标文件，gdb中使用file命令：

	(gdb)  file ./bin/kernel

之后gdb就会载入这个文件中的符号信息了。

通过gdb可以对ucore代码进行调试，以lab1中调试memset函数为例：

(1)  运行 `qemu -S -s -hda ./bin/ucore.img -monitor stdio`

(2)  运行 gdb并与qemu进行连接

(3)  设置断点并执行

(4)  qemu 单步调试。

运行过程以及结果如下：

<table>
<tr><td>窗口一</td><td>窗口二</td>
<tr>
<td>
chy@laptop: ~/lab1$ qemu -S  -s -hda ./bin/ucore.img 
</td>
<td>
chy@laptop: ~/lab1$ gdb ./bin/kernel<br>
(gdb) target remote:1234<br>
Remote debugging using :1234<br>
0x0000fff0 in ?? ()<br>
(gdb) file obj/kernel/kernel.elf<br>
(gdb) break memset<br>
Breakpoint 1 at 0x100d9f: file libs/string.c, line 54. <br>
(gdb) run<br>
Starting program: /home/chenyu/oscourse/develop/ucore/lab1/bin/kernel <br>
 <br>
Breakpoint 1, memset (s=0x1020fc, c=0 '\000', n=12) at libs/string.c:54<br>
54                return __memset(s, c, n); <br>
(gdb)
</td>
</tr></table>
