
#### 3.5.2 stdout设备文件

**初始化**

既然stdout设备是设备文件系统的文件，自然有自己的inode结构。在系统初始化时，即只需如下处理过程

```
kern_init-->fs_init-->dev_init-->dev_init_stdout --> dev_create_inode
                 --> stdout_device_init
                 --> vfs_add_dev
```

在dev\_init\_stdout中完成了对stdout设备文件的初始化。即首先创建了一个inode，然后通过stdout\_device\_init完成对inode中的成员变量inode-\>\_\_device\_info进行初始：

这里的stdout设备文件实际上就是指的console外设（它其实是串口、并口和CGA的组合型外设）。这个设备文件是一个只写设备，如果读这个设备，就会出错。接下来我们看看stdout设备的相关处理过程。

**初始化**

stdout设备文件的初始化过程主要由stdout\_device\_init完成，其具体实现如下：

```
static void
stdout_device_init(struct device *dev) {
    dev->d_blocks = 0;
    dev->d_blocksize = 1;
    dev->d_open = stdout_open;
    dev->d_close = stdout_close;
    dev->d_io = stdout_io;
    dev->d_ioctl = stdout_ioctl;
}
```

可以看到，stdout\_open函数完成设备文件打开工作，如果发现用户进程调用open函数的参数flags不是只写（O\_WRONLY），则会报错。

**访问操作实现**

stdout\_io函数完成设备的写操作工作，具体实现如下：

```
static int
stdout_io(struct device *dev, struct iobuf *iob, bool write) {
    if (write) {
        char *data = iob->io_base;
        for (; iob->io_resid != 0; iob->io_resid --) {
            cputchar(*data ++);
        }
        return 0;
    }
    return -E_INVAL;
}
```

可以看到，要写的数据放在iob-\>io\_base所指的内存区域，一直写到iob-\>io\_resid的值为0为止。每次写操作都是通过cputchar来完成的，此函数最终将通过console外设驱动来完成把数据输出到串口、并口和CGA显示器上过程。另外，也可以注意到，如果用户想执行读操作，则stdout\_io函数直接返回错误值**-**E\_INVAL。
