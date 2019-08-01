
#### 3.3.2 索引节点 

**磁盘索引节点**

SFS中的磁盘索引节点代表了一个实际位于磁盘上的文件。首先我们看看在硬盘上的索引节点的内容：

```
struct sfs_disk_inode {
    uint32_t size;                              如果inode表示常规文件，则size是文件大小
    uint16_t type;                                  inode的文件类型
    uint16_t nlinks;                               此inode的硬链接数
    uint32_t blocks;                              此inode的数据块数的个数
    uint32_t direct[SFS_NDIRECT];                此inode的直接数据块索引值（有SFS_NDIRECT个）
    uint32_t indirect;                            此inode的一级间接数据块索引值
};
```

通过上表可以看出，如果inode表示的是文件，则成员变量direct[]直接指向了保存文件内容数据的数据块索引值。indirect间接指向了保存文件内容数据的数据块，indirect指向的是间接数据块（indirect
block），此数据块实际存放的全部是数据块索引，这些数据块索引指向的数据块才被用来存放文件内容数据。

默认的，ucore 里 SFS\_NDIRECT 是 12，即直接索引的数据页大小为 12 \* 4k = 48k；当使用一级间接数据块索引时，ucore 支持最大的文件大小为 12 \* 4k + 1024 \* 4k = 48k + 4m。数据索引表内，0 表示一个无效的索引，inode 里 blocks 表示该文件或者目录占用的磁盘的 block 的个数。indiret 为 0 时，表示不使用一级索引块。（因为 block 0 用来保存 super block，它不可能被其他任何文件或目录使用，所以这么设计也是合理的）。

对于普通文件，索引值指向的 block
中保存的是文件中的数据。而对于目录，索引值指向的数据保存的是目录下所有的文件名以及对应的索引节点所在的索引块（磁盘块）所形成的数组。数据结构如下：

```
/* file entry (on disk) */
struct sfs_disk_entry {
    uint32_t ino;                                   索引节点所占数据块索引值
    char name[SFS_MAX_FNAME_LEN + 1];               文件名
};
```

操作系统中，每个文件系统下的 inode 都应该分配唯一的 inode 编号。SFS 下，为了实现的简便（偷懒），每个 inode 直接用他所在的磁盘 block 的编号作为 inode 编号。比如，root block 的 inode 编号为 1；每个 sfs\_disk\_entry 数据结构中，name 表示目录下文件或文件夹的名称，ino 表示磁盘 block 编号，通过读取该 block 的数据，能够得到相应的文件或文件夹的 inode。ino 为0时，表示一个无效的 entry。

此外，和 inode 相似，每个 sfs\_dirent\_entry 也占用一个 block。

**内存中的索引节点**

```
/* inode for sfs */
struct sfs_inode {
    struct sfs_disk_inode *din;                     /* on-disk inode */
    uint32_t ino;                                   /* inode number */
    uint32_t flags;                                 /* inode flags */
    bool dirty;                                     /* true if inode modified */
    int reclaim_count;                              /* kill inode if it hits zero */
    semaphore_t sem;                                /* semaphore for din */
    list_entry_t inode_link;                        /* entry for linked-list in sfs_fs */
    list_entry_t hash_link;                         /* entry for hash linked-list in sfs_fs */
};
```

可以看到SFS中的内存inode包含了SFS的硬盘inode信息，而且还增加了其他一些信息，这属于是便于进行是判断否改写、互斥操作、回收和快速地定位等作用。需要注意，一个内存inode是在打开一个文件后才创建的，如果关机则相关信息都会消失。而硬盘inode的内容是保存在硬盘中的，只是在进程需要时才被读入到内存中，用于访问文件或目录的具体内容数据

为了方便实现上面提到的多级数据的访问以及目录中 entry 的操作，对 inode
SFS实现了一些辅助的函数：

1. sfs\_bmap\_load\_nolock：将对应 sfs\_inode 的第 index 个索引指向的 block 的索引值取出存到相应的指针指向的单元（ino\_store）。该函数只接受 index <= inode-\>blocks 的参数。当 index == inode-\>blocks 时，该函数理解为需要为 inode 增长一个 block。并标记 inode 为 dirty（所有对 inode 数据的修改都要做这样的操作，这样，当 inode 不再使用的时候，sfs 能够保证 inode 数据能够被写回到磁盘）。sfs\_bmap\_load\_nolock 调用的 sfs\_bmap\_get\_nolock 来完成相应的操作，阅读 sfs\_bmap\_get\_nolock，了解他是如何工作的。（sfs\_bmap\_get\_nolock 只由 sfs\_bmap\_load\_nolock 调用）   
2. sfs\_bmap\_truncate\_nolock：将多级数据索引表的最后一个 entry 释放掉。他可以认为是 sfs\_bmap\_load\_nolock 中，index == inode-\>blocks 的逆操作。当一个文件或目录被删除时，sfs 会循环调用该函数直到 inode-\>blocks 减为 0，释放所有的数据页。函数通过 sfs\_bmap\_free\_nolock 来实现，他应该是 sfs\_bmap\_get\_nolock 的逆操作。和 sfs\_bmap\_get\_nolock 一样，调用 sfs\_bmap\_free\_nolock 也要格外小心。    
3. sfs\_dirent\_read\_nolock：将目录的第 slot 个 entry 读取到指定的内存空间。他通过上面提到的函数来完成。   
4. sfs\_dirent\_write\_nolock：用指定的 entry 来替换某个目录下的第 slot 个entry。他通过调用 sfs\_bmap\_load\_nolock保证，当第 slot 个entry 不存在时（slot == inode-\>blocks），SFS 会分配一个新的entry，即在目录尾添加了一个 entry。   
5. sfs\_dirent\_search\_nolock：是常用的查找函数。他在目录下查找 name，并且返回相应的搜索结果（文件或文件夹）的 inode 的编号（也是磁盘编号），和相应的 entry 在该目录的 index 编号以及目录下的数据页是否有空闲的 entry。（SFS 实现里文件的数据页是连续的，不存在任何空洞；而对于目录，数据页不是连续的，当某个 entry 删除的时候，SFS 通过设置 entry-\>ino 为0将该 entry 所在的 block 标记为 free，在需要添加新 entry 的时候，SFS 优先使用这些 free 的 entry，其次才会去在数据页尾追加新的 entry。    

注意，这些后缀为 nolock 的函数，只能在已经获得相应 inode 的semaphore才能调用。

**Inode的文件操作函数**

```
static const struct inode_ops sfs_node_fileops = {
    .vop_magic                      = VOP_MAGIC,
    .vop_open                       = sfs_openfile,
    .vop_close                      = sfs_close,
    .vop_read                       = sfs_read,
    .vop_write                      = sfs_write,
    ……
};
```

上述sfs\_openfile、sfs\_close、sfs\_read和sfs\_write分别对应用户进程发出的open、close、read、write操作。其中sfs\_openfile不用做什么事；sfs\_close需要把对文件的修改内容写回到硬盘上，这样确保硬盘上的文件内容数据是最新的；sfs\_read和sfs\_write函数都调用了一个函数sfs\_io，并最终通过访问硬盘驱动来完成对文件内容数据的读写。

**Inode的目录操作函数**

```
static const struct inode_ops sfs_node_dirops = {
    .vop_magic                      = VOP_MAGIC,
    .vop_open                       = sfs_opendir,
    .vop_close                      = sfs_close,
    .vop_getdirentry                = sfs_getdirentry,
    .vop_lookup                     = sfs_lookup,                           
    ……
};
```

对于目录操作而言，由于目录也是一种文件，所以sfs\_opendir、sys\_close对应户进程发出的open、close函数。相对于sfs\_open，sfs\_opendir只是完成一些open函数传递的参数判断，没做其他更多的事情。目录的close操作与文件的close操作完全一致。由于目录的内容数据与文件的内容数据不同，所以读出目录的内容数据的函数是sfs\_getdirentry，其主要工作是获取目录下的文件inode信息。
