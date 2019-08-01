
##### 2.4.4.2 ucore 代码编译

(1) 编译过程：在解压缩后的 ucore 源码包中使用 make 命令即可。例如 lab1中：
```
	chy@laptop: ~/lab1$  make
```
在lab1目录下的bin目录中，生成一系列的目标文件：
 - ucore.img：被qemu访问的虚拟硬盘文件
 - kernel: ELF格式的toy ucore kernel执行文，被嵌入到了ucore.img中
 - bootblock: 虚拟的硬盘主引导扇区（512字节），包含了bootloader执行代码，被嵌入到了ucore.img中
 - sign：外部执行程序，用来生成虚拟的硬盘主引导扇区

还生成了其他很多文件，这里就不一一列举了。

(2) 保存修改：

使用 diff 命令对修改后的 ucore 代码和 ucore 源码进行比较，比较之前建议使用 make clean 命令清除不必要文件。(如果有ctags 文件，需要手工清除。)

(3)应用修改：参见 patch 命令说明。

