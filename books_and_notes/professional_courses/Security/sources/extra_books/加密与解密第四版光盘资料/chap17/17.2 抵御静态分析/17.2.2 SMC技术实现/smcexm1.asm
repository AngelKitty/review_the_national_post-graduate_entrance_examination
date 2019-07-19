;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
; 说明：	随书配套代码---SMC技术演示
;
; 程序功能：此程序用来演示SMC技术的基础运用。
;
; 使用说明：编译该源程序，运行程序可以看到一信息提示。然后单步跟踪程序，可以看到程序对自身加密的部分
;			代码进行解密。
;
; 编程语言：MASM 6.0
;
; 编译方法：ML /c /coff smcexm1.asm
; 链接方法：LINK /SECTION:.text,ERW /SUBSYSTEM:WINDOWS smcexm1.obj
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

.DATA
szUser32Dll			BYTE	'USER32.DLL',0
szMessageBoxA		BYTE	'MessageBoxA',0
.CODE
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 解密函数
DecryptFunc	proc uses ebx ecx edx esi edi lpBuffer:LPVOID,nBuffSize:DWORD
			mov		esi,lpBuffer
			mov		edi,esi
			mov		ecx,nBuffSize
loc_loop:	lodsb
			dec		al
			stosb
			loop	loc_loop
			ret
DecryptFunc	endp


;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 程序入口
Main:
			;-=-=-=-得到API函数地址入栈保存-=-=-=-=-
			invoke	LoadLibraryA,addr szUser32Dll
			test	eax,eax
			jz		lm_exit
			invoke	GetProcAddress,eax,addr szMessageBoxA
			test	eax,eax
			jz		lm_exit
			push	eax
			;-=-=-=-解密数据-=-=-=-=-
			push	DataLen
			push	offset EnData
			call	DecryptFunc

EnData		BYTE	0ECh,01Bh,054h,076h,064h,064h,066h,074h,074h,001h
			BYTE	04Ah,021h,06Dh,070h,077h,066h,021h,075h,069h,06Ah
			BYTE	074h,021h,068h,062h,06Eh,066h,022h,001h,060h,0E9h
			BYTE	001h,001h,001h,001h,05Eh,082h,0EEh,023h,011h,041h
			BYTE	001h,06Bh,001h,08Eh,086h,003h,011h,041h,001h,051h
			BYTE	08Eh,086h,00Bh,011h,041h,001h,051h,06Bh,001h,000h
			BYTE	0D8h,08Eh,0BEh,001h,011h,041h,001h,0BAh,04Eh,001h
			BYTE	001h,001h,034h,0C1h,0FDh,0F4h,0ABh

DataLen		EQU		$ - offset EnData		;$

lm_exit:	invoke	ExitProcess,0

end Main