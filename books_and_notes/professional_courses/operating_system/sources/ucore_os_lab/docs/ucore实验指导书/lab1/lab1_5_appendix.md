
## 附录“启动后第一条执行的指令”

### intel资料的说明
【参考IA-32 Intel Architecture Software Developer’s Manual Volume 3: System Programming Guide Section 9.1.4】

9.1.4 First Instruction Executed

The first instruction that is fetched and executed following a hardware reset is located at physical address FFFFFFF0H. This address is 16 bytes below the processor’s uppermost physical address. The EPROM containing the softwareinitialization code must be located at this address.

The address FFFFFFF0H is beyond the 1-MByte addressable range of the processor while in real-address mode. The processor is initialized to this starting address as follows. The CS register has two parts: the visible segment selector part and the hidden base address part. In real-address mode, the base address is normally formed by shifting the 16-bit segment selector value 4 bits to the left to produce a 20-bit base address. However, during a hardware reset, the segment selector in the CS register is loaded with F000H and the base address is loaded with FFFF0000H. The starting address is thus formed by adding the base address to the value in the EIP register (that is, FFFF0000 + FFF0H = FFFFFFF0H). 

The first time the CS register is loaded with a new value after a hardware reset, the processor will follow the normal rule for address translation in real-address mode (that is, [CS base address = CS segment selector * 16]). To insure that the base address in the CS register remains unchanged until the EPROM based softwareinitialization code is completed, the code must not contain a far jump or far call or allow an interrupt to occur (which would cause the CS selector value to be changed).


### 单步调试和查看BIOS代码
如果你是想看BIOS的汇编，可试试如下方法：
练习2可以单步跟踪，方法如下：
 
1 修改 lab1/tools/gdbinit,
```
set architecture i8086
target remote :1234
```
2 在 lab1目录下，执行
```
make debug
```
这时gdb停在BIOS的第一条指令处：
```
0xffff0: ljmp $0xf000,$0xe05b
```

3 在看到gdb的调试界面(gdb)后，执行如下命令，就可以看到BIOS在执行了
```
si
si
...
```
4 此时的`CS=0xf, IP=fff0`，如果想看BIOS的代码
```
x /2i 0xffff0
```
应该可以看到
```
0xffff0: ljmp $0xf000,$0xe05b
0xffff5: xor %dh,0x322f
```
进一步可以执行
```
x /10i 0xfe05b
```
可以看到后续的BIOS代码。
