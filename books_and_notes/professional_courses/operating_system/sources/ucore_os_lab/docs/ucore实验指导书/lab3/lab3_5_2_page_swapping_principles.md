
### 5.2 页面置换机制

如果要实现页面置换机制，只考虑页替换算法的设计与实现是远远不够的，还需考虑其他问题：

* 哪些页可以被换出？
* 一个虚拟的页如何与硬盘上的扇区建立对应关系？
* 何时进行换入和换出操作？
* 如何设计数据结构以支持页替换算法？
* 如何完成页的换入换出操作？

这些问题在下面会逐一进行分析。注意，在实验三中仅实现了简单的页面置换机制，但现在还没有涉及实验四和实验五才实现的内核线程和用户进程，所以还无法通过内核线程机制实现一个完整意义上的虚拟内存页面置换功能。

#### 1. 可以被换出的页

在操作系统的设计中，一个基本的原则是：并非所有的物理页都可以交换出去的，只有映射到用户空间且被用户程序直接访问的页面才能被交换，而被内核直接使用的内核空间的页面不能被换出。这里面的原因是什么呢？操作系统是执行的关键代码，需要保证运行的高效性和实时性，如果在操作系统执行过程中，发生了缺页现象，则操作系统不得不等很长时间（硬盘的访问速度比内存的访问速度慢2\~3个数量级），这将导致整个系统运行低效。而且，不难想象，处理缺页过程所用到的内核代码或者数据如果被换出，整个内核都面临崩溃的危险。

但在实验三实现的ucore中，我们只是实现了换入换出机制，还没有设计用户态执行的程序，所以我们在实验三中仅仅通过执行check\_swap函数在内核中分配一些页，模拟对这些页的访问，然后通过do\_pgfault来调用swap\_map\_swappable函数来查询这些页的访问情况并间接调用相关函数，换出“不常用”的页到磁盘上。

#### 2. 虚存中的页与硬盘上的扇区之间的映射关系

如果一个页被置换到了硬盘上，那操作系统如何能简捷来表示这种情况呢？在ucore的设计上，充分利用了页表中的PTE来表示这种情况：当一个PTE用来描述一般意义上的物理页时，显然它应该维护各种权限和映射关系，以及应该有PTE\_P标记；但当它用来描述一个被置换出去的物理页时，它被用来维护该物理页与 swap 磁盘上扇区的映射关系，并且该 PTE 不应该由 MMU 将它解释成物理页映射(即没有 PTE\_P 标记)，与此同时对应的权限则交由 mm\_struct 来维护，当对位于该页的内存地址进行访问的时候，必然导致 page fault，然后ucore能够根据 PTE 描述的 swap 项将相应的物理页重新建立起来，并根据虚存所描述的权限重新设置好 PTE 使得内存访问能够继续正常进行。

如果一个页（4KB/页）被置换到了硬盘某8个扇区（0.5KB/扇区），该PTE的最低位--present位应该为0 （即 PTE\_P 标记为空，表示虚实地址映射关系不存在），接下来的7位暂时保留，可以用作各种扩展；而原来用来表示页帧号的高24位地址，恰好可以用来表示此页在硬盘上的起始扇区的位置（其从第几个扇区开始）。为了在页表项中区别 0 和 swap 分区的映射，将 swap 分区的一个 page 空出来不用，也就是说一个高24位不为0，而最低位为0的PTE表示了一个放在硬盘上的页的起始扇区号（见swap.h中对swap\_entry\_t的描述）：

```
swap_entry_t
-------------------------
| offset | reserved | 0 |
-------------------------
24 bits 7 bits 1 bit
```

考虑到硬盘的最小访问单位是一个扇区，而一个扇区的大小为512（2\^8）字节，所以需要8个连续扇区才能放置一个4KB的页。在ucore中，用了第二个IDE硬盘来保存被换出的扇区，根据实验三的输出信息

```
“ide 1: 262144(sectors), 'QEMU HARDDISK'.”
```

我们可以知道实验三可以保存262144/8=32768个页，即128MB的内存空间。swap
分区的大小是 swapfs\_init 里面根据磁盘驱动的接口计算出来的，目前 ucore
里面要求 swap 磁盘至少包含 1000 个 page，并且至多能使用 1<<24 个page。

#### 3. 执行换入换出的时机

在实验三中， check\_mm\_struct变量这个数据结构表示了目前
ucore认为合法的所有虚拟内存空间集合，而mm中的每个vma表示了一段地址连续的合法虚拟空间。当ucore或应用程序访问地址所在的页不在内存时，就会产生page fault异常，引起调用do\_pgfault函数，此函数会判断产生访问异常的地址属于check\_mm\_struct某个vma表示的合法虚拟地址空间，且保存在硬盘swap文件中（即对应的PTE的高24位不为0，而最低位为0），则是执行页换入的时机，将调用swap\_in函数完成页面换入。

换出页面的时机相对复杂一些，针对不同的策略有不同的时机。ucore目前大致有两种策略，即积极换出策略和消极换出策略。积极换出策略是指操作系统周期性地（或在系统不忙的时候）主动把某些认为“不常用”的页换出到硬盘上，从而确保系统中总有一定数量的空闲页存在，这样当需要空闲页时，基本上能够及时满足需求；消极换出策略是指，只是当试图得到空闲页时，发现当前没有空闲的物理页可供分配，这时才开始查找“不常用”页面，并把一个或多个这样的页换出到硬盘上。

在实验三中的基本练习中，支持上述的第二种情况。对于第一种积极换出策略，即每隔1秒执行一次的实现积极的换出策略，可考虑在扩展练习中实现。对于第二种消极的换出策略，则是在ucore调用alloc\_pages函数获取空闲页时，此函数如果发现无法从物理内存页分配器获得空闲页，就会进一步调用swap\_out函数换出某页，实现一种消极的换出策略。

#### 4. 页替换算法的数据结构设计

到实验二为止，我们知道目前表示内存中物理页使用情况的变量是基于数据结构Page的全局变量pages数组，pages的每一项表示了计算机系统中一个物理页的使用情况。为了表示物理页可被换出或已被换出的情况，可对Page数据结构进行扩展：

```
struct Page {  
……   
list\_entry\_t pra\_page\_link;   
uintptr\_t pra\_vaddr;   
};  
```

pra\_page\_link可用来构造按页的第一次访问时间进行排序的一个链表，这个链表的开始表示第一次访问时间最近的页，链表结尾表示第一次访问时间最远的页。当然链表头可以就可设置为pra\_list\_head（定义在swap\_fifo.c中），构造的时机是在page fault发生后，进行do\_pgfault函数时。pra\_vaddr可以用来记录此物理页对应的虚拟页起始地址。

当一个物理页 （struct Page） 需要被 swap 出去的时候，首先需要确保它已经分配了一个位于磁盘上的swap page（由连续的8个扇区组成）。这里为了简化设计，在swap\_check函数中建立了每个虚拟页唯一对应的swap page，其对应关系设定为：虚拟页对应的PTE的索引值 = swap page的扇区起始位置\*8。

为了实现各种页替换算法，我们设计了一个页替换算法的类框架swap\_manager:

```
struct swap_manager  
{  
    const char *name;  
    /* Global initialization for the swap manager */  
    int (*init) (void);  
    /* Initialize the priv data inside mm_struct */  
    int (*init_mm) (struct mm_struct *mm);  
    /* Called when tick interrupt occured */  
    int (*tick_event) (struct mm_struct *mm);  
    /* Called when map a swappable page into the mm_struct */  
    int (*map_swappable) (struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in);   
    /* When a page is marked as shared, this routine is called to delete the addr entry from the swap manager */
    int (*set_unswappable) (struct mm_struct *mm, uintptr_t addr);  
    /* Try to swap out a page, return then victim */  
    int (*swap_out_victim) (struct mm_struct *mm, struct Page *ptr_page, int in_tick);  
    /* check the page relpacement algorithm */  
    int (*check\_swap)(void);   
};  
``` 

这里关键的两个函数指针是map\_swappable和swap\_out\_vistim，前一个函数用于记录页访问情况相关属性，后一个函数用于挑选需要换出的页。显然第二个函数依赖于第一个函数记录的页访问情况。tick\_event函数指针也很重要，结合定时产生的中断，可以实现一种积极的换页策略。

#### 5. swap_check的检查实现

下面具体讲述一下实验三中实现置换算法的页面置换的检查执行逻辑，便于大家实现练习2。实验三的检查过程在函数swap\_check（kern/mm/swap.c中）中，其大致流程如下。

1. 调用mm\_create建立mm变量，并调用vma\_create创建vma变量，设置合法的访问范围为4KB\~24KB；
2. 调用free\_page等操作，模拟形成一个只有4个空闲 physical page；并设置了从4KB\~24KB的连续5个虚拟页的访问操作；
3. 设置记录缺页次数的变量pgfault\_num=0，执行check\_content\_set函数，使得起始地址分别对起始地址为0x1000, 0x2000, 0x3000, 0x4000的虚拟页按时间顺序先后写操作访问，由于之前没有建立页表，所以会产生page fault异常，如果完成练习1，则这些从4KB\~20KB的4虚拟页会与ucore保存的4个物理页帧建立映射关系；
4. 然后对虚页对应的新产生的页表项进行合法性检查；
5. 然后进入测试页替换算法的主体，执行函数check\_content\_access，并进一步调用到\_fifo\_check\_swap函数，如果通过了所有的assert。这进一步表示FIFO页替换算法基本正确实现；
6. 最后恢复ucore环境。
