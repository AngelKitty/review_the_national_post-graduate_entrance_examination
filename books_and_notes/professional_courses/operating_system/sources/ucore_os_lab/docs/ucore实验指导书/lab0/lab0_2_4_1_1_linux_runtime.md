
##### 2.4.1.1 Linux运行环境

QEMU用于模拟一台x86计算机，让ucore能够运行在QEMU上。为了能够正确的编译和安装 qemu，尽量使用最新版本的qemu（http://wiki.qemu.org/Download），或者os ftp服务器上提供的qemu源码：qemu-1.1.0.tar.gz）。目前 qemu 能够支持最新的 gcc-4.x 编译器。例如：在 Ubuntu 12.04 系统中，默认得版本是 gcc-4.6.x (可以通过 gcc -v 或者 gcc --version 进行查看)。

可直接使用ubuntu中提供的qemu，只需执行如下命令即可。

	sudo apt-get install qemu-system
 
也可采用下面描述的方法对qemu进行源码级安装。
 