;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
; 说明：	随书配套代码---SMC技术演示
;
; 程序功能：此程序用来形成smcexm1.asm中的CodeBegin开始处的数据
;
; 使用说明：编译该源程序，运行程序将产生smcexm1.bin文件，此文件中的数据即为smcexm1.asm所需要的数据
;			使用HexWorkShop把smcexm1.bin文件中的数据转换成十六进制文字形式，复制到smcexm1.asm即可。
;
; 编程语言：MASM 6.0
;
; 编译方法：ML /c /coff trexm1.asm
; 链接方法：LINK /SECTION:.text,ERW /SUBSYSTEM:WINDOWS trexm1.obj
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

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 函数声明
EncryptFunc			proto :LPVOID,:DWORD
WriteToFile			proto :LPVOID,:LPVOID,:DWORD

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 数据定义
.DATA
szErrTitle		BYTE	"Error Information",0
szErrMsg1		BYTE	"Error: Create File for Write Data Failed!",0
szErrMsg2		BYTE	"Error: Write Data to File Failed!",0
szErrMsg3		BYTE	"Can't Write Data to File!",0
szGoodTitle		BYTE	"Success",0
szGoodMsg		BYTE	"Ok! Write Success!",0
lpszFileName	BYTE	"smcexm1.bin",0

.CODE
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 将被加密的代码
.RADIX 16
ALIGN	1
CodeBegin:	jmp		loc_begin

szTitle		BYTE	'Success',0
szMsg		BYTE	'I love this game!',0

loc_begin:
			;-=-=-=-从堆栈中得到MessageBoxA函数的地址-=-=-=-=-
			pop		edi
			;-=-=-=-计算运行时的地址与编译地址之差-=-=-=-=-
			call	loc_next
loc_next:	pop		ebp
			sub		ebp,offset loc_next
			;-=-=-=-调用MessageBoxA函数显示信息-=-=-=-=-
			push	MB_OK
			lea		eax,[ebp + szTitle]
			push	eax
			lea		eax,[ebp + szMsg]
			push	eax
			push	NULL
			call	edi
			;-=-=-=-把自身的代码清零-=-=-=-=-
			lea		edi,[ebp + CodeBegin]
			mov		ecx,CodeLen
			xor		eax,eax
			cld
			rep		stosb
CodeEnd:
CodeLen		=		(offset CodeEnd - offset CodeBegin)

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 加密函数
EncryptFunc	proc uses ebx ecx edx esi edi lpBuffer:LPVOID,nBuffSize:DWORD
			mov		esi,lpBuffer
			mov		edi,esi
			mov		ecx,nBuffSize
loc_loop:	lodsb
			inc		al
			stosb
			loop	loc_loop
			ret
EncryptFunc	endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 把数据写入文件
WriteToFile	proc uses ebx ecx edx esi edi pszFileName:LPVOID,lpBuffer:LPVOID,dwBuffSize:DWORD
	LOCAL	hFile			:	DWORD
	LOCAL	dwWriteBytes	:	DWORD
	LOCAL	dwResult		:	DWORD
			mov		[hFile],INVALID_HANDLE_VALUE
			mov		[dwWriteBytes],0
			mov		[dwResult],FALSE

			;-=-=-=-=-=-打开文件进行写-=-=-=-=-=-
			invoke	CreateFileA,pszFileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,
								CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
			cmp		eax,INVALID_HANDLE_VALUE
			jnz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg1,addr szErrTitle,MB_OK
			mov		[dwResult],FALSE
			jmp		loc_safe_ret
@@:			mov		[hFile],eax

			;-=-=-=-=-=-写入所有数据-=-=-=-=-=-
			invoke	WriteFile,[hFile],lpBuffer,dwBuffSize,addr dwWriteBytes,NULL
			mov		eax,[dwWriteBytes]
			cmp		eax,dwBuffSize
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg2,addr szErrTitle,MB_OK
			mov		[dwResult],FALSE
			jmp		loc_safe_ret
@@:			mov		[dwResult],TRUE

loc_safe_ret:
			;-=-=-=-=-=-关闭文件-=-=-=-=-=-
			cmp		[hFile],INVALID_HANDLE_VALUE
			jz		@F
			invoke	CloseHandle,[hFile]
@@:			mov		eax,[dwResult]
			ret
WriteToFile			endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 程序入口
Main:
			push	CodeLen
			push	offset CodeBegin
			call	EncryptFunc
			push	CodeLen
			push	offset CodeBegin
			push	offset lpszFileName
			call	WriteToFile
			cmp		eax,TRUE
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg3,addr szErrTitle,MB_OK
			jmp		lm_exit
@@:			invoke	MessageBoxA,NULL,addr szGoodMsg,addr szGoodTitle,MB_OK
lm_exit:	invoke	ExitProcess,0
end Main
