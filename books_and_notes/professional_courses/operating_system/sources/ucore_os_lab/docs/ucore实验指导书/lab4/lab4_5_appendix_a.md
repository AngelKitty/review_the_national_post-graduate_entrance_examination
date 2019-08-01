
## 附录A：实验四的参考输出如下：

```
make qemu
 (THU.CST) os is loading ...
 
 Special kernel symbols:
 entry 0xc010002c (phys)
 etext 0xc010d0f7 (phys)
 edata 0xc012dad0 (phys)
 end 0xc0130e78 (phys)
 Kernel executable memory footprint: 196KB
 memory management: default_pmm_manager
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
 check_slab() succeeded!
 kmalloc_init() succeeded!
 check_vma_struct() succeeded!
 page fault at 0x00000100: K/W [no page found].
 check_pgfault() succeeded!
 check_vmm() succeeded.
 ide 0: 10000(sectors), 'QEMU HARDDISK'.
 ide 1: 262144(sectors), 'QEMU HARDDISK'.
 SWAP: manager = fifo swap manager
 BEGIN check_swap: count 1, total 31944
 mm->sm_priv c0130e64 in fifo_init_mm
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
 check_swap() succeeded!
 ++ setup timer interrupts
 this initproc, pid = 1, name = "init"
 To U: "Hello world!!".
 To U: "en.., Bye, Bye. :)"
 kernel panic at kern/process/proc.c:316:
 process exit!!.
 
 Welcome to the kernel debug monitor!!
 Type 'help' for a list of commands.
 K>
```
