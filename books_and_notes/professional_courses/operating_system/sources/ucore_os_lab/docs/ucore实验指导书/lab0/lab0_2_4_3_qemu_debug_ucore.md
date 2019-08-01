
#### 2.4.3 基于qemu内建模式调试ucore

调试举例：调试 lab1，跟踪bootmain函数：

(1) 运行 qemu -S -hda ucore.img -monitor stdio

(2) 查看 bootblock.asm 得到 bootmain 函数地址为 0x7d60，并插入断点。

(3) 使用命令 c 连续执行到断点。

(4) 使用 xp 命令进行反汇编。

(5) 使用 s 命令进行单步执行。
运行结果如下：

	chy@laptop: ~/lab1$ qemu -S -hda ucore.img -monitor stdio
	(qemu) b 0x7d60
	insert breakpoint 0x7d60 success!
	(qemu) c
		working …
	(qemu)
		break:
			0x00007d60:  push   %ebp                 
	(qemu) xp /10i $pc
		0x00007d60:  push   %ebp
		0x00007d61:  mov    %esp,%ebp
		0x00007d63:  push   %esi
		0x00007d64:  push   %ebx
		0x00007d65:  sub   $0x4,%esp
		0x00007d68:  mov    0x7da8,%esi
		0x00007d6e:  mov    $0x0,%ebx
		0x00007d73:  movsbl (%esi,%ebx,1),%eax
		0x00007d77:  mov    %eax,(%esp,1)
		0x00007d7a:  call   0x7c6c
	(qemu) step
		0x00007d61:  mov    %esp,%ebp
	(qemu) step
		0x00007d63:  push   %esi
