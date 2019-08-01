
#### 3.7.1 打开文件 

有了上述分析后，我们可以看看如果一个用户进程打开文件会做哪些事情？首先假定用户进程需要打开的文件已经存在在硬盘上。以user/sfs\_filetest1.c为例，首先用户进程会调用在main函数中的如下语句：

```
int fd1 = safe_open("/test/testfile", O_RDWR | O_TRUNC);
```

从字面上可以看出，如果ucore能够正常查找到这个文件，就会返回一个代表文件的文件描述符fd1，这样在接下来的读写文件过程中，就直接用这样fd1来代表就可以了。那这个打开文件的过程是如何一步一步实现的呢？

**通用文件访问接口层的处理流程**

首先进入通用文件访问接口层的处理流程，即进一步调用如下用户态函数： open-\>sys\_open-\>syscall，从而引起系统调用进入到内核态。到了内核态后，通过中断处理例程，会调用到sys\_open内核函数，并进一步调用sysfile\_open内核函数。到了这里，需要把位于用户空间的字符串"/test/testfile"拷贝到内核空间中的字符串path中，并进入到文件系统抽象层的处理流程完成进一步的打开文件操作中。

**文件系统抽象层的处理流程**

1. 分配一个空闲的file数据结构变量file在文件系统抽象层的处理中，首先调用的是file\_open函数，它要给这个即将打开的文件分配一个file数据结构的变量，这个变量其实是当前进程的打开文件数组current-\>fs\_struct-\>filemap[]中的一个空闲元素（即还没用于一个打开的文件），而这个元素的索引值就是最终要返回到用户进程并赋值给变量fd1。到了这一步还仅仅是给当前用户进程分配了一个file数据结构的变量，还没有找到对应的文件索引节点。

为此需要进一步调用vfs\_open函数来找到path指出的文件所对应的基于inode数据结构的VFS索引节点node。vfs\_open函数需要完成两件事情：通过vfs\_lookup找到path对应文件的inode；调用vop\_open函数打开文件。

2. 找到文件设备的根目录“/”的索引节点需要注意，这里的vfs\_lookup函数是一个针对目录的操作函数，它会调用vop\_lookup函数来找到SFS文件系统中的“/test”目录下的“testfile”文件。为此，vfs\_lookup函数首先调用get\_device函数，并进一步调用vfs\_get\_bootfs函数（其实调用了）来找到根目录“/”对应的inode。这个inode就是位于vfs.c中的inode变量bootfs\_node。这个变量在init\_main函数（位于kern/process/proc.c）执行时获得了赋值。

3. 找到根目录“/”下的“test”子目录对应的索引节点，在找到根目录对应的inode后，通过调用vop\_lookup函数来查找“/”和“test”这两层目录下的文件“testfile”所对应的索引节点，如果找到就返回此索引节点。

4. 把file和node建立联系。完成第3步后，将返回到file\_open函数中，通过执行语句“file-\>node=node;”，就把当前进程的current-\>fs\_struct-\>filemap[fd]（即file所指变量）的成员变量node指针指向了代表“/test/testfile”文件的索引节点node。这时返回fd。经过重重回退，通过系统调用返回，用户态的syscall-\>sys\_open-\>open-\>safe\_open等用户函数的层层函数返回，最终把把fd赋值给fd1。自此完成了打开文件操作。但这里我们还没有分析第2和第3步是如何进一步调用SFS文件系统提供的函数找位于SFS文件系统上的“/test/testfile”所对应的sfs磁盘inode的过程。下面需要进一步对此进行分析。

**SFS文件系统层的处理流程**

这里需要分析文件系统抽象层中没有彻底分析的vop\_lookup函数到底做了啥。下面我们来看看。在sfs\_inode.c中的sfs\_node\_dirops变量定义了“.vop\_lookup = sfs\_lookup”，所以我们重点分析sfs\_lookup的实现。

sfs\_lookup有三个参数：node，path，node\_store。其中node是根目录“/”所对应的inode节点；path是文件“testfile”的绝对路径“/test/testfile”，而node\_store是经过查找获得的“testfile”所对应的inode节点。

Sfs\_lookup函数以“/”为分割符，从左至右逐一分解path获得各个子目录和最终文件对应的inode节点。在本例中是分解出“test”子目录，并调用sfs\_lookup\_once函数获得“test”子目录对应的inode节点subnode，然后循环进一步调用sfs\_lookup\_once查找以“test”子目录下的文件“testfile1”所对应的inode节点。当无法分解path后，就意味着找到了testfile1对应的inode节点，就可顺利返回了。

当然这里讲得还比较简单，sfs\_lookup\_once将调用sfs\_dirent\_search\_nolock函数来查找与路径名匹配的目录项，如果找到目录项，则根据目录项中记录的inode所处的数据块索引值找到路径名对应的SFS磁盘inode，并读入SFS磁盘inode对的内容，创建SFS内存inode。
