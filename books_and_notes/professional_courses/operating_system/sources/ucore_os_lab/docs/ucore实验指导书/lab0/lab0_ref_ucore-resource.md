#参考资料

> 一些资料信息来源于 http://pdos.csail.mit.edu/6.828/2014/reference.html

### UNIX general info
- [Youtube Unix intro](https://www.youtube.com/watch?v=tc4ROCJYbm0)
- [The UNIX Time-Sharing System](http://citeseer.ist.psu.edu/10962.html), Dennis M. Ritchie and Ken L.Thompson,. Bell System Technical Journal 57, number 6, part 2 (July-August 1978) pages 1905-1930. 
- [The Evolution of the Unix Time-sharing System](http://www.read.seas.harvard.edu/~kohler/class/aosref/ritchie84evolution.pdf), Dennis M. Ritchie, 1979.
- The C programming language (second edition) by Kernighan and Ritchie. Prentice Hall, Inc., 1988. ISBN 0-13-110362-8, 1998.

### building or reading a small OS
 - [How to make an Operating System](https://www.gitbook.com/book/samypesse/how-to-create-an-operating-system/details)
 - [xv6 book 中文](https://www.gitbook.com/book/th0ar/xv6-chinese/details)
 - [自己动手写操作系统](http://item.jd.com/10142246.html)于渊 著,电子工业出版社,2005
 - [Linux-0.11内核完全注释](http://oldlinux.org/download/clk011c-3.0-toc.pdf) 赵炯，2009
 - [oldlinux论坛](http://www.oldlinux.org/oldlinux/)
 - [osdev.org](http://wiki.osdev.org/Expanded_Main_Page)

### some OS course
 - [6.828: Operating Systems Engineering - in MIT](http://pdos.csail.mit.edu/6.828/2014/index.html)
 - [CS-537: Introduction to Operating Systems - in WISC](http://pages.cs.wisc.edu/~remzi/Classes/537/Fall2013/)
 
### x86 Emulation
- [QEMU user manual](http://wiki.qemu.org/Qemu-doc.html)

### x86 Assembly Language
- [Intel 80386 Programmer's Reference Manual, 1987 (HTML).](http://www.logix.cz/michal/doc/i386/)
- [Linux 汇编语言开发指南](http://www.ibm.com/developerworks/cn/linux/l-assembly/index.html)
- [汇编语言程序设计](http://item.jd.com/10057711.html) 布鲁姆 著, 马朝晖 等译,机械工业出版社,2005

### Multiprocessor references:
 - [MP specification](http://pdos.csail.mit.edu/6.828/2014/readings/ia32/MPspec.pdf)
 - [IO APIC](http://pdos.csail.mit.edu/6.828/2014/readings/ia32/ioapic.pdf)
 
### x86系统结构与编程
 - [微型计算机技术及应用（第4版）](http://product.dangdang.com/22915697.html#catalog) 戴梅萼，史嘉权　编著, 清华大学出版社, 2008
 - [x86/x64体系探索及编程](http://item.jd.com/11100249.html) 邓志 著，电子工业出版社，2012
 - [x86汇编语言：从实模式到保护模式](http://item.jd.com/11179005.html) 李忠，王晓波，余洁 著，电子工业出版社，2013

### General BIOS and PC bootstrap
 - [BIOS Services and Software Interrupts](http://www.htl-steyr.ac.at/~morg/pcinfo/hardware/interrupts/inte1at0.htm), Roger Morgan, 1997.
 - ["El Torito" Bootable CD-ROM Format Specification](http://pdos.csail.mit.edu/6.828/2014/readings/boot-cdrom.pdf),Phoenix/IBM, January 1995.

### VGA display - [console.c](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab1/kern/driver/console.c)
 - [VESA BIOS Extension (VBE) 3.0](http://web.archive.org/web/20080302090304/http://www.vesa.org/public/VBE/vbe3.pdf) September 1998.
 - [Free VGA Project](http://www.osdever.net/FreeVGA/home.htm) J.D. Neal, 1998.

### 8253/8254 Programmable Interval Timer (PIT)
 - [82C54 CHMOS Programmable Interval Timer,](http://www.intel.com/design/archives/periphrl/docs/23124406.htm) , Intel, October 1994.
 - [Data Solutions 8253/8254 Tutorial](http://www.decisioncards.com/io/tutorials/8254_tut.html), Data Solutions.

### 8259/8259A Programmable Interrupt Controller (PIC)
 - [8259A Programmable Interrupt Controller](http://pdos.csail.mit.edu/6.828/2014/readings/hardware/8259A.pdf),Intel, December 1988. 

### 16550 UART Serial Port 
 - [PC16550D Universal Asynchronous Receiver/Transmitter with FIFOs](http://www.national.com/pf/PC/PC16550D.html), National Semiconductor, 1995.
 - [http://byterunner.com/16550.html](http://byterunner.com/16550.html), Byterunner Technologies.
 - [Interfacing the Serial / RS232 Port,](http://www.beyondlogic.org/serial/serial.htm), Craig Peacock, August 2001.

### IEEE 1284 Parallel Port 
 - [Parallel Port Central](http://www.lvr.com/parport.htm), Jan Axelson.
 - [Parallel Port Background](http://www.fapo.com/porthist.htm), Warp Nine Engineering.
 - [IEEE 1284 - Updating the PC Parallel Port](http://zone.ni.com/devzone/cda/tut/p/id/3466), National Instruments.
 - [Interfacing the Standard Parallel Port](http://www.beyondlogic.org/spp/parallel.htm), Craig Peacock, August 2001.

### IDE hard drive controller 
 - [AT Attachment with Packet Interface - 6 (working draft)](http://pdos.csail.mit.edu/6.828/2014/readings/hardware/ATA-d1410r3a.pdf), ANSI, December 2001.
 - [Programming Interface for Bus Master IDE Controller](http://pdos.csail.mit.edu/6.828/2014/readings/hardware/IDE-BusMaster.pdf), Brad Hosler, Intel, May 1994.
 - [The Guide to ATA/ATAPI documentation](http://suif.stanford.edu/~csapuntz/ide.html), Constantine Sapuntzakis, January 2002.

