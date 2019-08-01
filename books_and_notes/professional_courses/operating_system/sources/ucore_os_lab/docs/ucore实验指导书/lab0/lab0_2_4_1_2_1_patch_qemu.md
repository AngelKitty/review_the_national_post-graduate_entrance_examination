
###### 2.4.1.2.1 获得并应用修改

编译qemu还会用到的库文件有 libsdl1.2-dev 等。安装命令如下：

	sudo apt-get install libsdl1.2-dev	# 安装库文件 libsdl1.2-dev

获得 qemu 的安装包以后，对其进行解压缩(如果格式无法识别，请下载相应的解压缩软件)。

例如 qemu.tar.gz/qemu.tar.bz2 文件，在命令行中可以使用：

	tar zxvf qemu.tar.gz

或者

	tar jxvf qemu.tar.bz2
 
对 qemu 应用修改：如果实验中使用的 qemu 需要打patch，应用过程如下所示：

	chy@chyhome-PC:~$ls
	qemu.patch      qemu
	chy@chyhome-PC:~$cd qemu
	chy@chyhome-PC:~$patch -p1 -u < ../qemu.patch
 