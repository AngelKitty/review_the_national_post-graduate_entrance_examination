/*******************************************************
/*《加密与解密》第三版配套实例
/*  第15章 反跟踪技术
/*15.3.4 防止调试器附加
/* code by forgot 2008.3
/*(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

		@get_api_addr	"NTDLL.DLL","ZwContinue"
		xchg	ebx,eax

;  得到Ntdll.dll的ZwContinue地址

		call	a1
		dd	0
a1:	push	PAGE_READWRITE
		push	5
		push	ebx
		call	VirtualProtect
		@check	0,"Error: cannot deprotect the region!"

;   申请内存读写权限

		lea	edi,_ZwContinue_b
		mov	ecx,0Fh
		mov	esi,ebx
		rep	movsb

;   Edi寄存器指向我们自定义的一块大小为0F的内存区域，
;   这正是Ntdll.ZwContinue函数的大小。rep movsb指令把
;   原始ZwContinue函数复制到我们指定的ZwContinue_b处

		lea	eax,_ZwContinue
		mov	edi,ebx
		call 	make_jump
		
;   _ZwContinue处地址放入eax，原函数地址放入edi,调用
;   make_jump在原函数开头构造一个跳转指令（常用伎俩）。

		@debug 	"attach debugger to me now!",
MB_ICONINFORMATION



exit:	mov	byte ptr [flag],1

;   正常调用，flag为1
		push 	0
		@callx 	ExitProcess

make_jump:
		pushad
		mov	byte ptr [edi],0E9h
		sub	eax,edi
		sub	eax,5
		mov	dword ptr [edi+1],eax
		popad
		ret

;   保留所有寄存器，构造跳转，使ZwContinue原函数跳入我
;   们的_ZwContinue执行

flag	db	0

;   定义flag，用来判断是否被附加调试

_ZwContinue:	pushad
		cmp	byte ptr [flag],0
		jne	we_q
		@debug	"Debugger found!",MB_ICONERROR
we_q:	popad

;   判断flag是否为0，如果为0，检测到调试器，否则继续
;   执行下面的代码，正是我们复制的ZwContinue原始代码

_ZwContinue_b:	db	0Fh dup (0)

comment $		
		77F5B638 > B8 20000000      MOV EAX,20
		77F5B63D   BA 0003FE7F      MOV EDX,7FFE0300
		77F5B642   FFD2             CALL EDX
		77F5B644   C2 0800          RETN 8
$

;   复制完成后，这里看起来应该是上边的样子

end start
