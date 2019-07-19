;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
; 说明：	随书配套代码---SMC技术演示
;
; 程序功能：此程序用来演示利用SMC技术进行反跟踪的运用。
;
; 使用说明：编译OSMC.ASM和TrSMC.ASM源程序，得到OSMC.EXE和TrSMC.EXE文件。
;			运行TrSMC.EXE将对OSMC.EXE文件进行加密处理，得到最终的样例程序SMC.EXE。
;			运行SMC.EXE可以看到一信息提示。如果你单步跟踪程序，可以看到程序其中利用SMC技术并结合
;			其他反跟踪技术的效果。
;
; 编程语言：MASM 6.0
;
; 编译方法：ML /c /coff OSMC.ASM
; 链接方法：LINK /SECTION:.text,ERW /SUBSYSTEM:WINDOWS OSMC.OBJ
;
; 注意事项：注意必须使编译后的程序代码段可写，否则程序运行将出现地址不可写错误。
;			即链接方法中必须在/SECTION开关选项中加入ERW设置。
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

.586P
.MODEL FLAT,STDCALL
OPTION CASEMAP:NONE

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 引入头文件
include		windows.inc
include		kernel32.inc
include		user32.inc

includelib	kernel32.lib
includelib	user32.lib

.RADIX 16

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 常量定义
.CONST
SIZE_OF_BLOCK1		EQU	(offset Block2 - offset Block1) SHR 2
SIZE_OF_BLOCK2		EQU (offset Block3 - offset Block2) SHR 2
SIZE_OF_BLOCK3		EQU (offset Block4 - offset Block3) SHR 2
SIZE_OF_ALLBLOCK	EQU (SIZE_OF_BLOCK1 + SIZE_OF_BLOCK2 + SIZE_OF_BLOCK3)
INIT_KEY			EQU 74746A6Ch
HASH_NUM1			EQU 12345678h
HASH_NUM2			EQU 87654321h
CONTEXT_ALL			EQU CONTEXT_FULL OR CONTEXT_DEBUG_REGISTERS
DR0_ENABLED              EQU 000000001b
LOCAL_EXACT_BPM_ENABLED  EQU 100000000b

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 数据定义
.DATA
OldFinalHandler		DWORD	0
OldEsp				DWORD	0
sContext			CONTEXT	<0>
szTitle				BYTE	'样例：利用SMC技术进行反跟踪',0
szErrMsg			BYTE	'我发现你了！...你在跟踪我...哈哈...',0

.CODE
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 解密函数：没有做更多的改动，依然是那个简单的方法
DecryptFunc	proc uses ebx ecx edx esi edi lpBuffer:LPVOID,nBuffSize:DWORD
			mov		esi,lpBuffer
			mov		edi,esi
			mov		ecx,nBuffSize
			shl		ecx,2
loc_loop:	lodsb
			dec		al
			stosb
			loop	loc_loop
			ret
DecryptFunc	endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 结构化异常处理
Final_Handler		proc C pExceptPoint:DWORD
			mov		edi,pExceptPoint
			assume	edi:ptr EXCEPTION_POINTERS
			mov		esi,[edi].pExceptionRecord
			assume	esi:ptr EXCEPTION_RECORD
			mov		edi,[edi].ContextRecord
			assume	edi:ptr CONTEXT

			; 如果发生严重错误，则不进行处理直接返回，从而转向下一级异常处理程序
			test	[esi].ExceptionFlags,1
			jnz		loc_search

			; 如果异常进行展开，则不进行处理直接返回。
			test	[esi].ExceptionFlags,6
			jnz		loc_unwind

			; 其他情况，则修改线程的上下文中的EIP
			mov		[edi].regEip,err_go_here
			mov		eax,[OldEsp]
			mov		[edi].regEsp,eax
			mov		[edi].ContextFlags,CONTEXT_ALL
			mov		eax,ExceptionContinueExecution
			jmp		loc_ret

			;转向下一级异常处理程序
loc_search:
loc_unwind:	mov		eax,ExceptionContinueSearch
loc_ret:	ret
Final_Handler		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 程序入口
Main:
			assume	fs:nothing
			;-=-=建立结构化异常处理=-=-
			invoke	SetUnhandledExceptionFilter,addr Final_Handler
			mov		[OldFinalHandler],eax
			mov		[OldEsp],esp
			;-=-=解密以后代码块=-=-
			push	SIZE_OF_ALLBLOCK
			push	offset Block1
			call	DecryptFunc
			jmp		Block1

; 设置这些数据的目的是为了方便程序加密
FlagData	DWORD	0C3C3C3C3h
			DWORD	offset Block1
			DWORD	SIZE_OF_BLOCK1
			DWORD	offset Block2
			DWORD	SIZE_OF_BLOCK2
			DWORD	offset Block3
			DWORD	SIZE_OF_BLOCK3
			DWORD	offset Block4

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 第一层代码，也是最外层代码
Block1:
			call	loc_next
			;-=-=这里是异常处理函数的起始地址=-=-
			mov		esi,[esp+4]
			assume	esi:ptr EXCEPTION_RECORD
			mov		edi,[esp+0Ch]
			assume	edi:ptr CONTEXT
			cmp		[esi].ExceptionCode,EXCEPTION_SINGLE_STEP
			jz		@F
			mov		eax,ExceptionContinueSearch
			ret
@@:			mov		eax,[edi].regEax
			xchg	eax,[edi].regEdx
			mov		[edi].regEax,eax
			xor		eax,eax
			mov		[edi].iDr0,eax
			and		[edi].iDr1,eax
			and		[edi].iDr2,eax
			and		[edi].iDr3,eax
			and		[edi].iDr6,0FFFF0FF0h
			and		[edi].iDr7,eax
			mov		[edi].ContextFlags,CONTEXT_ALL
			mov		eax,ExceptionContinueExecution
			ret

			;-=-=建立SEH机制=-=-
loc_next:	push	fs:[0]
			mov		fs:[0],esp
			;-=-=以下的大循环用于解密下一个代码块的数据=-=-
			mov		edi,offset Block2
			mov		esi,edi
			xor		ecx,ecx
			mov		edx,INIT_KEY
loc_loop1:	push	esi
			push	ecx			; 入栈保存寄存器值
			;-=-=以下小循环用于计算解密用的密钥=-=-
			mov		esi,offset Block1
			add		ecx,SIZE_OF_BLOCK1
			;-=-=让当前代码块的字节参与密钥计算，实现自身保护=-=-
@@:			lodsd
			xor		eax,edx
			xor		eax,ecx
			;-=-=把反跟踪与计算相结合=-=-
			pushf
			or		byte ptr [esp+1],01h
			popf
			nop
			loop	@B
			;-=-=-=-=-小循环结束-=-=-=-=-
			pop		ecx
			pop		esi
			;-=-=反跟踪代码=-=-
			pushad
			mov		edi,offset sContext
			mov		ecx,sizeof CONTEXT
			xor		eax,eax
			cld
			rep		stosb
			invoke	GetCurrentThread
			mov		edi,eax
			mov		[sContext].ContextFlags, CONTEXT_ALL
			invoke	GetThreadContext,edi,addr sContext
			mov		[sContext].ContextFlags, CONTEXT_DEBUG_REGISTERS
			mov		[sContext].iDr7,101h ;LOCAL_EXACT_BPM_ENABLED + DR0_ENABLED
			invoke	SetThreadContext,edi,addr sContext
			popad
			;-=-=解密下一代码块的一个字节=-=-
			lodsd
			xor		eax,edx
			stosd
			;-=-=变换密钥=-=-
			xor		edx,HASH_NUM1
			xor		edx,ecx
			inc		ecx
			cmp		ecx,SIZE_OF_BLOCK2
			jnz		loc_loop1
			;-=-=-=-=-大循环结束-=-=-=-=-
			pop		fs:[0]
			add		esp,4

; 第一层代码结束
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 第二层代码开始

Block2:
			;-=-=以下的大循环用于解密下一个代码块的数据=-=-
			mov		edi,offset Block3
			mov		esi,edi
			xor		ecx,ecx
			jmp		loc_loop2

			; 把数据保存在这里的原因是防止跟踪者篡改数据，可以起到保护数据的目的
szHello		db		49,66,6D,6D,70,2D,4A,21,6D,70,77,66,21,54,4E,44,22
SizeOfMsg	EQU		$ - offset szHello		; $
			db		00

loc_loop2:	push	esi
			push	ecx			; 入栈保存寄存器值
			;-=-=以下小循环用于计算解密用的密钥=-=-
			mov		esi,offset Block2
			add		ecx,SIZE_OF_BLOCK2
			;-=-=让当前代码块的字节参与密钥计算，实现自身保护=-=-
@@:			lodsd
			xor		edx,eax
			xor		edx,ecx
			loop	@B
			;-=-=-=-=-小循环结束-=-=-=-=-
			pop		ecx
			pop		esi
			;-=-=解密下一代码块的一个字节=-=-
			lodsd
			xor		eax,edx
			stosd
			;-=-=变换密钥=-=-
			xor		edx,HASH_NUM2
			xor		edx,ecx
			inc		ecx
			cmp		ecx,SIZE_OF_BLOCK3
			jnz		loc_loop2

; 第二层代码结束
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 第三层代码开始

Block3:
			;-=-=解密出要显示的文字=-=-
			mov		esi,offset szHello
			mov		edi,esi
			mov		ecx,SizeOfMsg
@@:			lodsb
			dec		al
			stosb
			loop	@B

; 第三层代码结束，这里是所有加密代码块的结束位置
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 正常代码，显示前面解密出的文字

Block4:		;-=-=显示前面解密出的文字=-=-
			invoke	MessageBoxA,NULL,addr szHello,addr szTitle,MB_OK
gm0_exit:	;-=-=恢复原始的结构化异常处理=-=-
			invoke	SetUnhandledExceptionFilter,[OldFinalHandler]
			invoke	ExitProcess,0

err_go_here:
			invoke	MessageBoxA,NULL,addr szErrMsg,addr szTitle,MB_OK
			jmp		gm0_exit
end Main

