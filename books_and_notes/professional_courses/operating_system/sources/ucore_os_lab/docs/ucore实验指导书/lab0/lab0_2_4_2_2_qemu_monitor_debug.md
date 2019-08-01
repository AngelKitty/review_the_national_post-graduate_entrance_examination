
##### 2.4.2.2 常用调试命令

qemu中monitor的常用命令：

<table>
<tr><td>help</td><td>查看 qemu 帮助，显示所有支持的命令。</td></tr>
<tr><td>q|quit|exit</td><td>退出 qemu。</td></tr>
<tr><td>stop</td><td>停止 qemu。</td></tr>
<tr><td>c|cont|continue</td><td>连续执行。</td></tr>
<tr><td>x /fmt addr<br>xp /fmt addr</td><td>显示内存内容，其中 'x' 为虚地址，'xp' 为实地址。<br>参数 /fmt  i 表示反汇编，缺省参数为前一次参数。</td></tr>
<tr><td>p|print'</td><td>计算表达式值并显示，例如 $reg 表示寄存器结果。</td></tr>
<tr><td>memsave addr size file<br>pmemsave addr size file</td><td>将内存保存到文件，memsave 为虚地址，pmemsave 为实地址。</td></tr>
<tr><td>breakpoint 相关：</td><td>设置、查看以及删除 breakpoint，pc执行到 breakpoint，qemu 停止。（暂时没有此功能）</td></tr>
<tr><td>watchpoint 相关：</td><td>设置、查看以及删除 watchpoint, 当 watchpoint 地址内容被修改，停止。（暂时没有此功能）</td></tr>
<tr><td>s|step</td><td>单步一条指令，能够跳过断点执行。</td></tr>
<tr><td>r|registers</td><td>显示全部寄存器内容。</td></tr>
<tr><td>info 相关操作</td><td>查询 qemu 支持的关于系统状态信息的操作。</td></tr>
</table>

其他具体的命令格式以及说明，参见 qemu help 命令帮助。

注意：qemu 默认有 ‘singlestep arg’ 命令（arg 为 参数），该命令为设置单步标志命令。例如：'singlestep off' 运行结果为禁止单步，'singlestep on' 结果为允许单步。在允许单步条件下，使用 cont 命令进行单步操作。如：

	(qemu) xp /3i $pc
	0xfffffff0: ljmp $0xf000, $0xe05b
	0xfffffff5: xor    %bh, (%bx, %si)
	0xfffffff7: das
	(qemu) singlestep on
	(qemu) cont
	0x000fe05b: xor %ax, %ax

step命令为单步命令，即qemu执行一步，能够跳过 breakpoint 断点执行。如果此时使用cont命令，则qemu 运行改为连续执行。

log命令能够保存qemu模拟过程产生的信息（与qemu运行参数 `-d' 相同），具体参数可以参考命令帮助。产生的日志信息保存在 “/tmp/qemu.log” 中，例如使用 'log in_asm'命令以后，运行过程产生的的qemu.log 文件为：

	1  ----------------
	2  IN:
	3  0xfffffff0:  ljmp   $0xf000,$0xe05b
	4
	5  ----------------
	6  IN:
	7  0x000fe05b:  xor    %ax,%ax
	8  0x000fe05d:  out    %al,$0xd
	9  0x000fe05f:  out    %al,$0xda
	10 0x000fe061:  mov    $0xc0,%al
	11 0x000fe063:  out    %al,$0xd6
	12 0x000fe065:  mov    $0x0,%al
	13 0x000fe067:  out    %al,$0xd4
