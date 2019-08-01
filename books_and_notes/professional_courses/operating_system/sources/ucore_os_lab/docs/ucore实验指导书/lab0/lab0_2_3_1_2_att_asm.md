
##### 2.3.1.2 AT&T汇编基本语法

Ucore中用到的是AT&T格式的汇编，与Intel格式的汇编有一些不同。二者语法上主要有以下几个不同：
```
	* 寄存器命名原则
		AT&T: %eax                      Intel: eax
	* 源/目的操作数顺序 
		AT&T: movl %eax, %ebx           Intel: mov ebx, eax
	* 常数/立即数的格式　
		AT&T: movl $_value, %ebx        Intel: mov eax, _value
	  把value的地址放入eax寄存器
		AT&T: movl $0xd00d, %ebx        Intel: mov ebx, 0xd00d
	* 操作数长度标识 
		AT&T: movw %ax, %bx             Intel: mov bx, ax
	* 寻址方式 
		AT&T:   immed32(basepointer, indexpointer, indexscale)
		Intel:  [basepointer + indexpointer × indexscale + imm32)
```
如果操作系统工作于保护模式下，用的是32位线性地址，所以在计算地址时不用考虑segment:offset的问题。上式中的地址应为：
```
	imm32 + basepointer + indexpointer × indexscale
```
下面是一些例子：
```
	* 直接寻址 
            AT&T:  foo                         Intel: [foo]
            boo是一个全局变量。注意加上$是表示地址引用，不加是表示值引用。对于局部变量，可以通过堆栈指针引用。
            
	* 寄存器间接寻址 
            AT&T: (%eax)                        Intel: [eax]
            
	* 变址寻址 
            AT&T: _variable(%eax)               Intel: [eax + _variable]
            AT&T: _array( ,%eax, 4)             Intel: [eax × 4 + _array]
            AT&T: _array(%ebx, %eax,8)          Intel: [ebx + eax × 8 + _array]
```
