代码在Mac上验证通过。由于原书是在Win上面编译的，这里也给一下Mac上编译的步骤。

首先使用nasm编译，编译的格式为bin格式，比如：

```
nasm -f bin c06_mbr.asm -o mbr.bin
```

原书中使用自带的工具将上面生成的bin文件转换成VHD格式，实际上没有必要，可以使用VirtualBox自带的工具VBoxManage来生成，如：

```
VBoxManage convertfromraw mbr.bin myfile.vhd --format VHD
```

这样在virtualbox中就可以加载这个生成的VHD文件看到效果了。

> **关于实验环境配置的一些问题**

64位环境下可能无法使用配书软件，解决方案如下：https://blog.csdn.net/OneTrianee/article/details/80410816

