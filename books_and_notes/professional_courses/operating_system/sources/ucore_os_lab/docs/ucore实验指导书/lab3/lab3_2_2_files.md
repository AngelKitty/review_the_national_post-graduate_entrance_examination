
### 项目组成

表1：实验三文件列表

```
|-- boot
|-- kern
| |-- driver
| | |-- …
| | |-- ide.c
| | \`-- ide.h
| |-- fs
| | |-- fs.h
| | |-- swapfs.c
| | \`-- swapfs.h
| |-- init
| | |-- …
| | \`-- init.c
| |-- mm
| | |-- default\_pmm.c
| | |-- default\_pmm.h
| | |-- memlayout.h
| | |-- mmu.h
| | |-- pmm.c
| | |-- pmm.h
| | |-- swap.c
| | |-- swap.h
| | |-- swap\_fifo.c
| | |-- swap\_fifo.h
| | |-- vmm.c
| | \`-- vmm.h
| |-- sync
| \`-- trap
| |-- trap.c
| \`-- …
|-- libs
| |-- list.h
| \`-- …
\`-- tools
```

相对与实验二，实验三主要改动如下：

* kern/mm/default\_pmm.[ch]：实现基于struct pmm\_manager类框架的Fist-Fit物理内存分配参考实现（分配最小单位为页，即4096字节），相关分配页和释放页等实现会间接被kmalloc/kfree等函数使用。
* kern/mm/pmm.[ch]：pmm.h定义物理内存分配类框架struct pmm\_manager。pmm.c包含了对此物理内存分配类框架的访问，以及与建立、修改、访问页表相关的各种函数实现。在本实验中会用到kmalloc/kfree等函数。
* libs/list.h：定义了通用双向链表结构以及相关的查找、插入等基本操作，这是建立基于链表方法的物理内存管理（以及其他内核功能）的基础。在lab0文档中有相关描述。其他有类似双向链表需求的内核功能模块可直接使用list.h中定义的函数。在本实验中会多次用到插入，删除等操作函数。
* kern/driver/ide.[ch]：定义和实现了内存页swap机制所需的磁盘扇区的读写操作支持；在本实验中会涉及通过swapfs\_\*函数间接使用文件中的函数。故了解即可。
* kern/fs/\*：定义和实现了内存页swap机制所需从磁盘读数据到内存页和写内存数据到磁盘上去的函数 swapfs\_read/swapfs\_write。在本实验中会涉及使用这两个函数。
* kern/mm/memlayout.h：修改了struct Page，增加了两项pra\_\*成员结构，其中pra\_page\_link可以用来建立描述各个页访问情况（比如根据访问先后）的链表。在本实验中会涉及使用这两个成员结构，以及le2page等宏。
* kern/mm/vmm.[ch]：vmm.h描述了mm\_struct，vma\_struct等表述可访问的虚存地址访问的一些信息，下面会进一步详细讲解。vmm.c涉及mm,vma结构数据的创建/销毁/查找/插入等函数，这些函数在check\_vma、check\_vmm等中被使用，理解即可。而page
fault处理相关的do\_pgfault函数是本次实验需要涉及完成的。
* kern/mm/swap.[ch]：定义了实现页替换算法类框架struct swap\_manager。swap.c包含了对此页替换算法类框架的初始化、页换入/换出等各种函数实现。重点是要理解何时调用swap\_out和swap\_in函数。和如何在此框架下连接具体的页替换算法实现。check\_swap函数以及被此函数调用的\_fifo\_check\_swap函数完成了对本次实验中的练习2：FIFO页替换算法基本正确性的检查，可了解，便于知道为何产生错误。
* kern/mm/swap\_fifo.[ch]：FIFO页替换算法的基于页替换算法类框架struct swap\_manager的简化实现，主要被swap.c的相关函数调用。重点是\_fifo\_map\_swappable函数（可用于建立页访问属性和关系，比如访问时间的先后顺序）和\_fifo\_swap\_out\_victim函数（可用于实现挑选出要换出的页），当然换出哪个页需要借助于fifo\_map\_swappable函数建立的某种属性关系，已选出合适的页。
* kern/mm/mmu.h：其中定义额也页表项的各种属性位，比如PTE\_P\\PET\_D\\PET\_A等，对于实现扩展实验的clock算法会有帮助。

本次实验的主要练习集中在vmm.c中的do\_pgfault函数和swap\_fifo.c中的\_fifo\_map\_swappable函数、\_fifo\_swap\_out\_victim函数。

#### 编译执行

编译并运行代码的命令如下：

```
make
make qemu
```

则可以得到如附录所示的显示内容（仅供参考，不是标准答案输出）

```
$ make qemu
(THU.CST) os is loading ...

Special kernel symbols:
  entry  0xc010002a (phys)
  etext  0xc01081c3 (phys)
  edata  0xc011fac8 (phys)
  end    0xc0120cf0 (phys)
Kernel executable memory footprint: 132KB
ebp:0xc011ef48 eip:0xc0100a51 args:0x00010094 0x00000000 0xc011ef78 0xc01000b8 
    kern/debug/kdebug.c:308: print_stackframe+21
ebp:0xc011ef58 eip:0xc0100d4f args:0x00000000 0x00000000 0x00000000 0xc011efc8 
    kern/debug/kmonitor.c:129: mon_backtrace+10
ebp:0xc011ef78 eip:0xc01000b8 args:0x00000000 0xc011efa0 0xffff0000 0xc011efa4 
    kern/init/init.c:56: grade_backtrace2+19
ebp:0xc011ef98 eip:0xc01000d9 args:0x00000000 0xffff0000 0xc011efc4 0x0000002a 
    kern/init/init.c:61: grade_backtrace1+27
ebp:0xc011efb8 eip:0xc01000f5 args:0x00000000 0xc010002a 0xffff0000 0xc010006d 
    kern/init/init.c:66: grade_backtrace0+19
ebp:0xc011efd8 eip:0xc0100115 args:0x00000000 0x00000000 0x00000000 0xc0108200 
    kern/init/init.c:71: grade_backtrace+26
ebp:0xc011eff8 eip:0xc010007a args:0x00000000 0x00000000 0x0000ffff 0x40cf9a00 
    kern/init/init.c:31: kern_init+79
memory management: default_pmm_manager
e820map:
  memory: 0009fc00, [00000000, 0009fbff], type = 1.
  memory: 00000400, [0009fc00, 0009ffff], type = 2.
  memory: 00010000, [000f0000, 000fffff], type = 2.
  memory: 07ee0000, [00100000, 07fdffff], type = 1.
  memory: 00020000, [07fe0000, 07ffffff], type = 2.
  memory: 00040000, [fffc0000, ffffffff], type = 2.
check_alloc_page() succeeded!
check_pgdir() succeeded!
check_boot_pgdir() succeeded!
-------------------- BEGIN --------------------
PDE(0e0) c0000000-f8000000 38000000 urw
  |-- PTE(38000) c0000000-f8000000 38000000 -rw
PDE(001) fac00000-fb000000 00400000 -rw
  |-- PTE(000e0) faf00000-fafe0000 000e0000 urw
  |-- PTE(00001) fafeb000-fafec000 00001000 -rw
--------------------- END ---------------------
check_vma_struct() succeeded!
page fault at 0x00000100: K/W [no page found].
check_pgfault() succeeded!
check_vmm() succeeded.
ide 0:      10000(sectors), 'QEMU HARDDISK'.
ide 1:     262144(sectors), 'QEMU HARDDISK'.
SWAP: manager = fifo swap manager
BEGIN check_swap: count 31966, total 31966
setup Page Table for vaddr 0X1000, so alloc a page
setup Page Table vaddr 0~4MB OVER!
set up init env for check_swap begin!
page fault at 0x00001000: K/W [no page found].
page fault at 0x00002000: K/W [no page found].
page fault at 0x00003000: K/W [no page found].
page fault at 0x00004000: K/W [no page found].
set up init env for check_swap over!
write Virt Page c in fifo_check_swap
write Virt Page a in fifo_check_swap
write Virt Page d in fifo_check_swap
write Virt Page b in fifo_check_swap
write Virt Page e in fifo_check_swap
page fault at 0x00005000: K/W [no page found].
swap_out: i 0, store page in vaddr 0x1000 to disk swap entry 2
write Virt Page b in fifo_check_swap
write Virt Page a in fifo_check_swap
page fault at 0x00001000: K/W [no page found].
swap_out: i 0, store page in vaddr 0x2000 to disk swap entry 3
swap_in: load disk swap entry 2 with swap_page in vadr 0x1000
write Virt Page b in fifo_check_swap
page fault at 0x00002000: K/W [no page found].
swap_out: i 0, store page in vaddr 0x3000 to disk swap entry 4
swap_in: load disk swap entry 3 with swap_page in vadr 0x2000
write Virt Page c in fifo_check_swap
page fault at 0x00003000: K/W [no page found].
swap_out: i 0, store page in vaddr 0x4000 to disk swap entry 5
swap_in: load disk swap entry 4 with swap_page in vadr 0x3000
write Virt Page d in fifo_check_swap
page fault at 0x00004000: K/W [no page found].
swap_out: i 0, store page in vaddr 0x5000 to disk swap entry 6
swap_in: load disk swap entry 5 with swap_page in vadr 0x4000
count is 7, total is 7
check_swap() succeeded!
++ setup timer interrupts
100 ticks
100 ticks
100 ticks
100 ticks
```
