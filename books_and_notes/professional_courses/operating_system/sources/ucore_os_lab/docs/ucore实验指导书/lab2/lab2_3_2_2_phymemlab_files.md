### 项目组成

表1： 实验二文件列表

```
bash
|-- boot
| |-- asm.h
| |-- bootasm.S
| \`-- bootmain.c
|-- kern
| |-- init
| | |-- entry.S
| | \`-- init.c
| |-- mm
| | |-- default\_pmm.c
| | |-- default\_pmm.h
| | |-- memlayout.h
| | |-- mmu.h
| | |-- pmm.c
| | \`-- pmm.h
| |-- sync
| | \`-- sync.h
| \`-- trap
| |-- trap.c
| |-- trapentry.S
| |-- trap.h
| \`-- vectors.S
|-- libs
| |-- atomic.h
| |-- list.h
\`-- tools
|-- kernel.ld
```

相对与实验一，实验二主要增加和修改的文件如上表所示。主要改动如下：

* boot/bootasm.S：增加了对计算机系统中物理内存布局的探测功能；
* kern/init/entry.S：根据临时段表重新暂时建立好新的段空间，为进行分页做好准备。
* kern/mm/default\_pmm.[ch]：提供基本的基于链表方法的物理内存管理（分配单位为页，即4096字节）；
* kern/mm/pmm.[ch]：pmm.h定义物理内存管理类框架struct
pmm\_manager，基于此通用框架可以实现不同的物理内存管理策略和算法(default\_pmm.[ch]
实现了一个基于此框架的简单物理内存管理策略)；
pmm.c包含了对此物理内存管理类框架的访问，以及与建立、修改、访问页表相关的各种函数实现。
* kern/sync/sync.h：为确保内存管理修改相关数据时不被中断打断，提供两个功能，一个是保存eflag寄存器中的中断屏蔽位信息并屏蔽中断的功能，另一个是根据保存的中断屏蔽位信息来使能中断的功能；（可不用细看）
* libs/list.h：定义了通用双向链表结构以及相关的查找、插入等基本操作，这是建立基于链表方法的物理内存管理（以及其他内核功能）的基础。其他有类似双向链表需求的内核功能模块可直接使用list.h中定义的函数。
* libs/atomic.h：定义了对一个变量进行读写的原子操作，确保相关操作不被中断打断。（可不用细看）
* tools/kernel.ld：ld形成执行文件的地址所用到的链接脚本。修改了ucore的起始入口和代码段的起始地址。相关细节可参看附录C。

**编译方法**

编译并运行代码的命令如下：
```bash
make

make qemu
```
则可以得到如下显示界面（仅供参考）
```bash
chenyu$ make qemu
(THU.CST) os is loading ...

Special kernel symbols:
  entry  0xc010002c (phys)
  etext  0xc010537f (phys)
  edata  0xc01169b8 (phys)
  end    0xc01178dc (phys)
Kernel executable memory footprint: 95KB
memory managment: default_pmm_manager
e820map:
  memory: 0009f400, [00000000, 0009f3ff], type = 1.
  memory: 00000c00, [0009f400, 0009ffff], type = 2.
  memory: 00010000, [000f0000, 000fffff], type = 2.
  memory: 07efd000, [00100000, 07ffcfff], type = 1.
  memory: 00003000, [07ffd000, 07ffffff], type = 2.
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
++ setup timer interrupts
100 ticks
100 ticks
……
```
通过上图，我们可以看到ucore在显示其entry（入口地址）、etext（代码段截止处地址）、edata（数据段截止处地址）、和end（ucore截止处地址）的值后，探测出计算机系统中的物理内存的布局（e820map下的显示内容）。接下来ucore会以页为最小分配单位实现一个简单的内存分配管理，完成二级页表的建立，进入分页模式，执行各种我们设置的检查，最后显示ucore建立好的二级页表内容，并在分页模式下响应时钟中断。
