
#### 3.7.2 读文件

读文件其实就是读出目录中的目录项，首先假定文件在磁盘上且已经打开。用户进程有如下语句：

```
read(fd, data, len);
```

即读取fd对应文件，读取长度为len，存入data中。下面来分析一下读文件的实现。

**通用文件访问接口层的处理流程**

先进入通用文件访问接口层的处理流程，即进一步调用如下用户态函数：read-\>sys\_read-\>syscall，从而引起系统调用进入到内核态。到了内核态以后，通过中断处理例程，会调用到sys\_read内核函数，并进一步调用sysfile\_read内核函数，进入到文件系统抽象层处理流程完成进一步读文件的操作。

**文件系统抽象层的处理流程**

1) 检查错误，即检查读取长度是否为0和文件是否可读。

2) 分配buffer空间，即调用kmalloc函数分配4096字节的buffer空间。

3) 读文件过程

[1] 实际读文件

循环读取文件，每次读取buffer大小。每次循环中，先检查剩余部分大小，若其小于4096字节，则只读取剩余部分的大小。然后调用file\_read函数（详细分析见后）将文件内容读取到buffer中，alen为实际大小。调用copy\_to\_user函数将读到的内容拷贝到用户的内存空间中，调整各变量以进行下一次循环读取，直至指定长度读取完成。最后函数调用层层返回至用户程序，用户程序收到了读到的文件内容。

[2] file\_read函数

这个函数是读文件的核心函数。函数有4个参数，fd是文件描述符，base是缓存的基地址，len是要读取的长度，copied\_store存放实际读取的长度。函数首先调用fd2file函数找到对应的file结构，并检查是否可读。调用filemap\_acquire函数使打开这个文件的计数加1。调用vop\_read函数将文件内容读到iob中（详细分析见后）。调整文件指针偏移量pos的值，使其向后移动实际读到的字节数iobuf\_used(iob)。最后调用filemap\_release函数使打开这个文件的计数减1，若打开计数为0，则释放file。

**SFS文件系统层的处理流程**

vop\_read函数实际上是对sfs\_read的包装。在sfs\_inode.c中sfs\_node\_fileops变量定义了.vop\_read = sfs\_read，所以下面来分析sfs\_read函数的实现。

sfs\_read函数调用sfs\_io函数。它有三个参数，node是对应文件的inode，iob是缓存，write表示是读还是写的布尔值（0表示读，1表示写），这里是0。函数先找到inode对应sfs和sin，然后调用sfs\_io\_nolock函数进行读取文件操作，最后调用iobuf\_skip函数调整iobuf的指针。

在sfs\_io\_nolock函数中，先计算一些辅助变量，并处理一些特殊情况（比如越界），然后有sfs\_buf\_op = sfs\_rbuf,sfs\_block\_op = sfs\_rblock，设置读取的函数操作。接着进行实际操作，先处理起始的没有对齐到块的部分，再以块为单位循环处理中间的部分，最后处理末尾剩余的部分。每部分中都调用sfs\_bmap\_load\_nolock函数得到blkno对应的inode编号，并调用sfs\_rbuf或sfs\_rblock函数读取数据（中间部分调用sfs\_rblock，起始和末尾部分调用sfs\_rbuf），调整相关变量。完成后如果offset + alen \> din-\>fileinfo.size（写文件时会出现这种情况，读文件时不会出现这种情况，alen为实际读写的长度），则调整文件大小为offset + alen并设置dirty变量。

sfs\_bmap\_load\_nolock函数将对应sfs\_inode的第index个索引指向的block的索引值取出存到相应的指针指向的单元（ino\_store）。它调用sfs\_bmap\_get\_nolock来完成相应的操作。sfs\_rbuf和sfs\_rblock函数最终都调用sfs\_rwblock\_nolock函数完成操作，而sfs\_rwblock\_nolock函数调用dop\_io-\>disk0\_io-\>disk0\_read\_blks\_nolock-\>ide\_read\_secs完成对磁盘的操作。
