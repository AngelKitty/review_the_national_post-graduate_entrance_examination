
###### 2.4.1.2.2 配置、编译和安装

编译以及安装 qemu 前需要使用 <qemu>(表示qemu解压缩路径)下面的 configure 脚本生成相应的配置文件等。而 configure 脚本有较多的参数可供选择，可以通过如下命令进行查看：
	
	configure  --help 

实验中可能会用到的命令例如：

	configure --target-list="i386-softmmu"  # 配置qemu，可模拟X86-32硬件环境
	make                                    # 编译qemu
	sudo make install                       # 安装qemu

qemu执行程序将缺省安装到 /usr/local/bin 目录下。
 
如果使用的是默认的安装路径，那么在 “/usr/local/bin” 下面即可看到安装结果：

	qemu-system-i386 qemu-img qemu-nbd ……
 
建立符号链接文件qemu

	sudo ln –s /usr/local/bin/qemu-system-i386  /usr/local/bin/qemu
 
