
#### 3.5.1 关键数据结构 

为了表示一个设备，需要有对应的数据结构，ucore为此定义了struct device，其描述如下：

```
struct device {
    size_t d_blocks;    //设备占用的数据块个数            
    size_t d_blocksize;  //数据块的大小
    int (*d_open)(struct device *dev, uint32_t open_flags);  //打开设备的函数指针
    int (*d_close)(struct device *dev); //关闭设备的函数指针
    int (*d_io)(struct device *dev, struct iobuf *iob, bool write); //读写设备的函数指针
    int (*d_ioctl)(struct device *dev, int op, void *data); //用ioctl方式控制设备的函数指针
};
```

这个数据结构能够支持对块设备（比如磁盘）、字符设备（比如键盘、串口）的表示，完成对设备的基本操作。ucore虚拟文件系统为了把这些设备链接在一起，还定义了一个设备链表，即双向链表vdev\_list，这样通过访问此链表，可以找到ucore能够访问的所有设备文件。

但这个设备描述没有与文件系统以及表示一个文件的inode数据结构建立关系，为此，还需要另外一个数据结构把device和inode联通起来，这就是vfs\_dev\_t数据结构：

```
// device info entry in vdev_list 
typedef struct {
    const char *devname;
    struct inode *devnode;
    struct fs *fs;
    bool mountable;
    list_entry_t vdev_link;
} vfs_dev_t;
```

利用vfs\_dev\_t数据结构，就可以让文件系统通过一个链接vfs\_dev\_t结构的双向链表找到device对应的inode数据结构，一个inode节点的成员变量in\_type的值是0x1234，则此 inode的成员变量in\_info将成为一个device结构。这样inode就和一个设备建立了联系，这个inode就是一个设备文件。
