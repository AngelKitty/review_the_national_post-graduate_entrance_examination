
#### 3.4.1 file & dir接口

file&dir接口层定义了进程在内核中直接访问的文件相关信息，这定义在file数据结构中，具体描述如下：

```
struct file {
    enum {
        FD_NONE, FD_INIT, FD_OPENED, FD_CLOSED,
    } status;                          //访问文件的执行状态
    bool readable;                     //文件是否可读
    bool writable;                     //文件是否可写
    int fd;                           //文件在filemap中的索引值
    off_t pos;                        //访问文件的当前位置
    struct inode *node;               //该文件对应的内存inode指针
    atomic_t open_count;              //打开此文件的次数
};
```

而在kern/process/proc.h中的proc\_struct结构中描述了进程访问文件的数据接口fs\_struct，其数据结构定义如下：

```
struct fs_struct {
    struct inode *pwd;                //进程当前执行目录的内存inode指针
    struct file *filemap;             //进程打开文件的数组
    atomic_t fs_count;                //访问此文件的线程个数？？
    semaphore_t fs_sem;                //确保对进程控制块中fs_struct的互斥访问
};
```

当创建一个进程后，该进程的fs\_struct将会被初始化或复制父进程的fs\_struct。当用户进程打开一个文件时，将从filemap数组中取得一个空闲file项，然后会把此file的成员变量node指针指向一个代表此文件的inode的起始地址。
