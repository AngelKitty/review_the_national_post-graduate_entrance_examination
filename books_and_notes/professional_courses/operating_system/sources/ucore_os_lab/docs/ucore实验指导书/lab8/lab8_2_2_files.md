
### 2.2 项目组成 

```
.   
├── boot   
├── kern  
│ ├── debug  
│ ├── driver   
│ │ ├── clock.c   
│ │ ├── clock.h   
│ │ └── ……   
│ ├── fs   
│ │ ├── devs   
│ │ │ ├── dev.c   
│ │ │ ├── dev\_disk0.c   
│ │ │ ├── dev.h   
│ │ │ ├── dev\_stdin.c   
│ │ │ └── dev\_stdout.c    
│ │ ├── file.c   
│ │ ├── file.h   
│ │ ├── fs.c   
│ │ ├── fs.h   
│ │ ├── iobuf.c   
│ │ ├── iobuf.h   
│ │ ├── sfs       
│ │ │ ├── bitmap.c   
│ │ │ ├── bitmap.h   
│ │ │ ├── sfs.c  
│ │ │ ├── sfs\_fs.c    
│ │ │ ├── sfs.h  
│ │ │ ├── sfs\_inode.c   
│ │ │ ├── sfs\_io.c   
│ │ │ └── sfs\_lock.c  
│ │ ├── swap   
│ │ │ ├── swapfs.c   
│ │ │ └── swapfs.h   
│ │ ├── sysfile.c   
│ │ ├── sysfile.h   
│ │ └── vfs   
│ │ ├── inode.c  
│ │ ├── inode.h   
│ │ ├── vfs.c  
│ │ ├── vfsdev.c   
│ │ ├── vfsfile.c  
│ │ ├── vfs.h   
│ │ ├── vfslookup.c    
│ │ └── vfspath.c  
│ ├── init  
│ ├── libs   
│ │ ├── stdio.c   
│ │ ├── string.c    
│ │ └── ……   
│ ├── mm   
│ │ ├── vmm.c  
│ │ └── vmm.h   
│ ├── process   
│ │ ├── proc.c   
│ │ ├── proc.h  
│ │ └── ……   
│ ├── schedule  
│ ├── sync   
│ ├── syscall   
│ │ ├── syscall.c  
│ │ └── ……   
│ └── trap   
│ ├── trap.c  
│ └── ……  
├── libs    
├── tools   
│ ├── mksfs.c   
│ └── ……   
└── user   
├── badarg.c  
├── badsegment.c    
├── divzero.c   
├── exit.c   
├── faultread.c    
├── faultreadkernel.c  
├── forktest.c    
├── forktree.c        
├── hello.c    
├── libs   
│ ├── dir.c   
│ ├── dir.h   
│ ├── file.c  
│ ├── file.h    
│ ├── initcode.S   
│ ├── lock.h   
│ ├── stdio.c   
│ ├── syscall.c   
│ ├── syscall.h   
│ ├── ulib.c   
│ ├── ulib.h   
│ └── umain.c   
├── ls.c   
├── sh.c     
└── ……
```

本次实验主要是理解kern/fs目录中的部分文件，并可用user/\*.c测试所实现的Simple
FS文件系统是否能够正常工作。本次实验涉及到的代码包括：

* 文件系统测试用例： user/\*.c：对文件系统的实现进行测试的测试用例；  

* 通用文件系统接口      
n user/libs/file.[ch]|dir.[ch]|syscall.c：与文件系统操作相关的用户库实行；    
n kern/syscall.[ch]：文件中包含文件系统相关的内核态系统调用接口   
n kern/fs/sysfile.[ch]|file.[ch]：通用文件系统接口和实行   

* 文件系统抽象层-VFS   
n kern/fs/vfs/\*.[ch]：虚拟文件系统接口与实现   

* Simple FS文件系统    
n kern/fs/sfs/\*.[ch]：SimpleFS文件系统实现    

* 文件系统的硬盘IO接口   
n kern/fs/devs/dev.[ch]|dev\_disk0.c：disk0硬盘设备提供给文件系统的I/O访问接口和实现   

* 辅助工具    
n tools/mksfs.c：创建一个Simple FS文件系统格式的硬盘镜像。（理解此文件的实现细节对理解SFS文件系统很有帮助）   

* 对内核其它模块的扩充   
n kern/process/proc.[ch]：增加成员变量 struct fs\_struct \*fs\_struct，用于支持进程对文件的访问；重写了do\_execve load\_icode等函数以支持执行文件系统中的文件。     
n kern/init/init.c：增加调用初始化文件系统的函数fs\_init。    
