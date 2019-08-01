
##### 2.4.2.1 运行参数

如果 qemu 使用的是默认 /usr/local/bin 安装路径，则在命令行中可以直接使用 qemu 命令运行程序。qemu 运行可以有多参数，格式如：

	qemu [options] [disk_image]

其中 disk_image 即硬盘镜像文件。

部分参数说明：

	`-hda file'        `-hdb file' `-hdc file' `-hdd file'
		使用 file  作为硬盘0、1、2、3镜像。
	`-fda file'  `-fdb file'
		使用 file  作为软盘镜像，可以使用 /dev/fd0 作为 file 来使用主机软盘。
	`-cdrom file'
		使用 file  作为光盘镜像，可以使用 /dev/cdrom 作为 file 来使用主机 cd-rom。
	`-boot [a|c|d]'
		从软盘(a)、光盘(c)、硬盘启动(d)，默认硬盘启动。
	`-snapshot'
		写入临时文件而不写回磁盘镜像，可以使用 C-a s 来强制写回。
	`-m megs'
		设置虚拟内存为 msg M字节，默认为 128M 字节。
	`-smp n'
		设置为有 n 个 CPU 的 SMP 系统。以 PC 为目标机，最多支持 255 个 CPU。
	`-nographic'
		禁止使用图形输出。
	其他：
		可用的主机设备 dev 例如：
			vc
				虚拟终端。
			null
				空设备
			/dev/XXX
				使用主机的 tty。
			file: filename
				将输出写入到文件 filename 中。
			stdio
				标准输入/输出。
			pipe：pipename
				命令管道 pipename。
			等。
		使用 dev 设备的命令如：
			`-serial dev'
				重定向虚拟串口到主机设备 dev 中。
			`-parallel dev'
				重定向虚拟并口到主机设备 dev 中。
			`-monitor dev'
				重定向 monitor 到主机设备 dev 中。
		其他参数：
			`-s'
				等待 gdb 连接到端口 1234。
			`-p port'
				改变 gdb 连接端口到 port。
			`-S'
				在启动时不启动 CPU， 需要在 monitor 中输入 'c'，才能让qemu继续模拟工作。
			`-d'
				输出日志到 qemu.log 文件。

其他参数说明可以参考：http://bellard.org/qemu/qemu-doc.html#SEC15 。其他qemu的安装和使用的说明可以参考http://bellard.org/qemu/user-doc.html。

或者在命令行收入 qemu (没有参数) 显示帮助。

在实验中，例如 lab1，可能用到的命令如：

	qemu -hda ucore.img -parallel stdio		# 让ucore在qemu模拟的x86硬件环境中执行

或

	qemu -S -s -hda ucore.img -monitor stdio	# 用于与gdb配合进行源码调试

