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
; 编译方法：ML /c /coff TrSMC.ASM
; 链接方法：LINK /SECTION:.text,ERW /SUBSYSTEM:WINDOWS TrSMC.OBJ
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
; 函数声明
CreateImage			proto pszFileName:LPVOID,pImageInfo:LPVOID
SearchBlockDataAddr	proto pImageInfo:LPVOID
SaveBlockData		proto lpAddress:LPVOID,pImageInfo:LPVOID
CreateKeyStream		proto
ReleaseMemory		proto lpMemory:LPVOID
StreamEncrypt		proto pKeyStream:LPVOID
EncryptData			proto lpBuffer:LPVOID,nBuffSize:DWORD

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 结构定义
IMAGE_INFO			STRUCT
	ImageBase		DWORD	0
	HeaderSize		DWORD	0
	ImagePtr		DWORD	0
	ImageSize		DWORD	0
	ScnHeaderPtr	DWORD	0
	ScnNum			DWORD	0
IMAGE_INFO			ENDS

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 常量定义
.CONST
INIT_KEY			EQU	74746A6Ch
HASH_NUM1			EQU	12345678h
HASH_NUM2			EQU	87654321h

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 数据定义
.DATA
szOrgFileName		BYTE	'OSMC.exe',0
szNewFileName		BYTE	'SMC.exe',0

sDosHeader			IMAGE_DOS_HEADER	<0>
sNtHeaders			IMAGE_NT_HEADERS	<0>

sImageInfo			IMAGE_INFO			<0>

KeyStreamPtr		DWORD	0
SizeOfAllBlocks		DWORD	0
SizeOfSmcBlocks		DWORD	0
AddrOfBlock1		DWORD	0
SizeOfBlock1		DWORD	0
AddrOfBlock2		DWORD	0
SizeOfBlock2		DWORD	0
AddrOfBlock3		DWORD	0
SizeOfBlock3		DWORD	0
AddrOfBlock4		DWORD	0

szErrTitle			BYTE	'错误',0
szErrMsg1			BYTE	'打开文件操作失败！',0
szErrMsg2			BYTE	'读数据失败！',0
szErrMsg3			BYTE	'无效执行文件格式!',0
szErrMsg4			BYTE	'申请内存操作失败！',0
szErrMsg5			BYTE	'没有发现特征块位置！',0
szErrMsg6			BYTE	'写数据失败！',0

.CODE
;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 创建进程运行时内存映象
CreateImage			proc uses ebx ecx edx esi edi pszFileName:LPVOID,pImageInfo:LPVOID
	LOCAL	hFile		:	DWORD
	LOCAL	dwReadBytes	:	DWORD
	LOCAL	dwPeOffset	:	DWORD
	LOCAL	dwHeaderSize:	DWORD
	LOCAL	dwImageSize	:	DWORD
	LOCAL	dwScnNum	:	DWORD
	LOCAL	dwImageBase	:	DWORD
	LOCAL	dwScnAlign	:	DWORD
	LOCAL	dwFileAlign	:	DWORD
	LOCAL	dwImagePtr	:	DWORD
	LOCAL	dwResult	:	DWORD
	LOCAL	dwNtHeadersPtr	:	DWORD
	LOCAL	dwScnHeaderPtr	:	DWORD
			mov		[hFile],0
			mov		[dwReadBytes],0
			mov		[dwPeOffset],0
			mov		[dwHeaderSize],0
			mov		[dwImageSize],0
			mov		[dwScnNum],0
			mov		[dwImageBase],0
			mov		[dwScnAlign],0
			mov		[dwFileAlign],0
			mov		[dwImagePtr],0
			mov		[dwResult],0
			mov		[dwNtHeadersPtr],0
			mov		[dwScnHeaderPtr],0

			;-=-=打开文件读=-=-
			push	NULL
			push	FILE_ATTRIBUTE_NORMAL
			push	OPEN_ALWAYS
			push	NULL
			push	FILE_SHARE_READ
			push	GENERIC_READ
			push	pszFileName
			call	CreateFileA
			cmp		eax,INVALID_HANDLE_VALUE
			jnz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg1,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			mov		[hFile],eax

			;-=-=读DOS格式头=-=-
			push	NULL
			lea		eax,[dwReadBytes]
			push	eax
			push	sizeof IMAGE_DOS_HEADER
			lea		eax,[sDosHeader]
			push	eax
			push	[hFile]
			call	ReadFile
			cmp		[dwReadBytes],sizeof IMAGE_DOS_HEADER
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg2,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
			;-=-=判断是否有效的'MZ'标志
@@:			mov		edi,offset sDosHeader
			assume	edi:ptr IMAGE_DOS_HEADER
			cmp		[edi].e_magic,IMAGE_DOS_SIGNATURE
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg3,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			mov		eax,[edi].e_lfanew
			mov		[dwPeOffset],eax
			;-=-=定位到PE文件头=-=-
			push	FILE_BEGIN
			push	0
			push	[dwPeOffset]
			push	[hFile]
			call	SetFilePointer
			;-=-=读出PE文件头=-=-
			push	NULL
			lea		eax,[dwReadBytes]
			push	eax
			push	sizeof IMAGE_NT_HEADERS
			lea		eax,[sNtHeaders]
			push	eax
			push	[hFile]
			call	ReadFile
			cmp		[dwReadBytes],sizeof IMAGE_NT_HEADERS
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg2,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			;-=-=判断是否有效的'PE'标志
			mov		edi,offset sNtHeaders
			assume	edi:ptr IMAGE_NT_HEADERS
			cmp		[edi].Signature,IMAGE_NT_SIGNATURE
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg3,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
			;-=-=获取相关信息保存=-=-
@@:			mov		eax,[edi].OptionalHeader.SizeOfHeaders
			mov		[dwHeaderSize],eax
			mov		eax,[edi].OptionalHeader.SizeOfImage
			mov		[dwImageSize],eax
			movzx	eax,[edi].FileHeader.NumberOfSections
			mov		[dwScnNum],eax
			mov		eax,[edi].OptionalHeader.ImageBase
			mov		[dwImageBase],eax
			;-=-=申请内存用于建立映象=-=-
			invoke	VirtualAlloc,NULL,[dwImageSize],MEM_COMMIT,PAGE_READWRITE
			test	eax,eax
			jnz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg4,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			mov		[dwImagePtr],eax
			;-=-=定位到文件开始处=-=-
			push	FILE_BEGIN
			push	0
			push	0
			push	[hFile]
			call	SetFilePointer
			;-=-=读出文件头数据到内存=-=-
			push	NULL
			lea		eax,[dwReadBytes]
			push	eax
			push	[dwHeaderSize]
			push	[dwImagePtr]
			push	[hFile]
			call	ReadFile
			mov		eax,[dwReadBytes]
			cmp		eax,[dwHeaderSize]
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg2,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			mov		eax,[dwImagePtr]
			add		eax,[dwPeOffset]
			add		eax,sizeof IMAGE_NT_HEADERS
			mov		[dwScnHeaderPtr],eax
			mov		esi,[dwScnHeaderPtr]
			assume	esi:ptr IMAGE_SECTION_HEADER
			mov		ecx,[dwScnNum]
loc_loop:	;-=-=定位到区段数据起始处-=-=
			push	esi
			push	ecx
			push	FILE_BEGIN
			push	0
			push	[esi].PointerToRawData
			push	[hFile]
			call	SetFilePointer
			pop		ecx
			pop		esi
			;-=-=读出区段数据到内存相应位置=-=-
			push	esi
			push	ecx
			push	NULL
			lea		eax,[dwReadBytes]
			push	eax
			push	[esi].SizeOfRawData
			mov		eax,[esi].VirtualAddress
			add		eax,[dwImagePtr]
			push	eax
			push	[hFile]
			call	ReadFile
			pop		ecx
			pop		esi
			mov		eax,[dwReadBytes]
			cmp		eax,[esi].SizeOfRawData
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg2,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			add		esi,sizeof IMAGE_SECTION_HEADER
			loop	loc_loop
			;-=-=返回相应信息-=-=
			mov		esi,[pImageInfo]
			assume	esi:ptr IMAGE_INFO
			test	esi,esi
			jz		loc_fail_ret
			mov		eax,[dwImageBase]
			mov		[esi].ImageBase,eax
			mov		eax,[dwHeaderSize]
			mov		[esi].HeaderSize,eax
			mov		eax,[dwImagePtr]
			mov		[esi].ImagePtr,eax
			mov		eax,[dwImageSize]
			mov		[esi].ImageSize,eax
			mov		eax,[dwScnHeaderPtr]
			mov		[esi].ScnHeaderPtr,eax
			mov		eax,[dwScnNum]
			mov		[esi].ScnNum,eax
			mov		[dwResult],TRUE
			jmp		loc_safe_ret
loc_fail_ret:
			cmp		[dwImagePtr],0
			jz		@F
			invoke	VirtualFree,[dwImagePtr],[dwImageSize],MEM_RELEASE
@@:			mov		[dwResult],FALSE
loc_safe_ret:
			cmp		[hFile],0
			jz		@F
			invoke	CloseHandle,[hFile]
@@:			mov		eax,[dwResult]
			ret
CreateImage			endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 搜索特征标志，以定位关键数据地址
SearchBlockDataAddr	proc uses ebx ecx edx esi edi pImageInfo:LPVOID
			mov		esi,pImageInfo
			test	esi,esi
			jz		loc_fail_ret
			assume	esi:ptr IMAGE_INFO
			mov		edi,[esi].ImagePtr
			mov		ecx,[esi].ImageSize
			mov		al,0C3h
loc_loop:	repnz	scasb
			test	ecx,ecx
			jz		loc_fail_ret
			mov		edx,dword ptr [edi-1]
			cmp		edx,0C3C3C3C3h
 			jnz		loc_loop
			lea		eax,[edi+3]
			jmp		loc_ret
loc_fail_ret:
			invoke	MessageBoxA,NULL,addr szErrMsg5,addr szErrTitle,MB_OK
			xor		eax,eax
loc_ret:	ret
SearchBlockDataAddr	endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 保存关键数据
SaveBlockData		proc uses ebx ecx edx esi edi lpAddress:LPVOID,pImageInfo:LPVOID
	LOCAL	dwImageBase	:	DWORD
	LOCAL	dwImagePtr	:	DWORD
			mov		[dwImageBase],0
			mov		[dwImagePtr],0
			mov		edi,pImageInfo
			test	edi,edi
			jz		loc_fail_ret
			assume	edi:ptr IMAGE_INFO
			mov		eax,[edi].ImageBase
			mov		[dwImageBase],eax
			mov		eax,[edi].ImagePtr
			mov		[dwImagePtr],eax
			mov		esi,lpAddress
			mov		edi,offset AddrOfBlock1
			xor		edx,edx
			xor		ebx,ebx
			; 保存Block1的地址
			xor		eax,eax
			xchg	eax,dword ptr [esi]
			sub		eax,[dwImageBase]
			add		eax,[dwImagePtr]
			stosd
			; 保存Block1的大小
			xor		eax,eax
			xchg	eax,dword ptr [esi+4]
			stosd
			add		ebx,eax
			; 保存Block2的地址
			xor		eax,eax
			xchg	eax,dword ptr [esi+8]
			sub		eax,[dwImageBase]
			add		eax,[dwImagePtr]
			stosd
			; 保存Block2的大小
			xor		eax,eax
			xchg	eax,dword ptr [esi+0Ch]
			stosd
			add		edx,eax
			add		ebx,eax
			; 保存Block3的地址
			xor		eax,eax
			xchg	eax,dword ptr [esi+10h]
			sub		eax,[dwImageBase]
			add		eax,[dwImagePtr]
			stosd
			; 保存Block3的大小
			xor		eax,eax
			xchg	eax,dword ptr [esi+14h]
			stosd
			add		edx,eax
			add		ebx,eax
			; 保存需要加密的块的总字节数
			mov		[SizeOfSmcBlocks],edx
			mov		[SizeOfAllBlocks],ebx
			; 保存Block4的地址
			xor		eax,eax
			xchg	eax,dword ptr [esi+18h]
			sub		eax,[dwImageBase]
			add		eax,[dwImagePtr]
			stosd
			mov		eax,TRUE
			jmp		loc_ret
loc_fail_ret:
			IFDEF	DEBUG_BUG
				int 3
			ENDIF
			mov		eax,FALSE
loc_ret:	ret
SaveBlockData		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 建立用于加密的密钥流
CreateKeyStream		proc uses ebx ecx edx esi edi
	LOCAL	StreamPtr	:	DWORD
			mov		[StreamPtr],0
			;-=-=申请内存用于保存密钥流数据=-=-
			mov		eax,[SizeOfSmcBlocks]
			shl		eax,2
			invoke	VirtualAlloc,NULL,eax,MEM_COMMIT,PAGE_READWRITE
			test	eax,eax
			jz		loc_fail_ret
			mov		[StreamPtr],eax
			;-=-=建立用于加密Block2块的密钥流=-=-
			mov		edi,[StreamPtr]			;
			mov		esi,[AddrOfBlock2]		;
			xor		ecx,ecx					;
			mov		edx,INIT_KEY			; EDX 为初始密钥
loc_loop1:	push	esi						;
			push	ecx						; 入栈保存 ESI 和 ECX
			mov		esi,[AddrOfBlock1]		;
			add		ecx,[SizeOfBlock1]		;
@@:			lodsd							; 
			xor		edx,eax					; 
			xor		edx,ecx					; Key = Key ^ Block1[i] ^ SizeOfBlock1
			loop	@B						;
			pop		ecx						;
			pop		esi						; 从堆栈中恢复 ESI 和 ECX 原来的值
			mov		eax,edx					;
			stosd							; 保存密钥
			xor		edx,HASH_NUM1			;
			xor		edx,ecx					; Key = Key ^ HASH_NUM1 ^ SizeOfBlock2
			inc		ecx						;
			cmp		ecx,[SizeOfBlock2]		;
			jnz		loc_loop1				;
			;-=-=建立用于加密Block3块的密钥流=-=-
			mov		esi,[AddrOfBlock3]		;
			xor		ecx,ecx					;
loc_loop2:	push	esi						;
			push	ecx						; 入栈保存 ESI 和 ECX
			mov		esi,[AddrOfBlock2]		;
			add		ecx,[SizeOfBlock2]		;
@@:			lodsd							; 
			xor		edx,eax					; 
			xor		edx,ecx					; Key = Key ^ Block2[i] ^ SizeOfBlock2
			loop	@B						;
			pop		ecx						;
			pop		esi						; 从堆栈中恢复 ESI 和 ECX 原来的值
			mov		eax,edx					;
			stosd							;
			xor		edx,HASH_NUM2			;
			xor		edx,ecx					; Key = Key ^ HASH_NUM2 ^ SizeOfBlock2
			inc		ecx						;
			cmp		ecx,[SizeOfBlock3]		;
			jnz		loc_loop2				;

			mov		eax,[StreamPtr]
			jmp		loc_ret
loc_fail_ret:
			xor		eax,eax
loc_ret:	ret
CreateKeyStream		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 释放内存
ReleaseMemory		proc uses ebx ecx edx esi edi	lpMemory:LPVOID
			cmp		lpMemory,0
			jz		@F
			invoke	VirtualFree,lpMemory,0,MEM_RELEASE
@@:			ret
ReleaseMemory		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 加密函数：没有做更多的改动，依然是个简单的加密方法
EncryptFunc	proc uses ebx ecx edx esi edi lpBuffer:LPVOID,nBuffSize:DWORD
			mov		esi,lpBuffer
			mov		edi,esi
			mov		ecx,nBuffSize
			shl		ecx,2
loc_loop:	lodsb
			inc		al
			stosb
			loop	loc_loop
			ret
EncryptFunc	endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 使用密钥流加密代码块
StreamEncrypt		proc uses ebx ecx edx esi edi pKeyStream:LPVOID
			;-=-=加密Block2块代码=-=-
			mov		edi,[AddrOfBlock2]
			mov		esi,pKeyStream
			mov		ecx,[SizeOfBlock2]
loc_loop1:	lodsd
			xor		dword ptr [edi],eax
			add		edi,4
			loop	loc_loop1
			;-=-=加密Block3块代码=-=-
			mov		edi,[AddrOfBlock3]
			mov		ecx,[SizeOfBlock3]
loc_loop2:	lodsd
			xor		dword ptr [edi],eax
			add		edi,4
			loop	loc_loop2
			push	[SizeOfAllBlocks]
			push	[AddrOfBlock1]
			call	EncryptFunc
			ret
StreamEncrypt		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 保存映象到文件
SaveImageToFile		proc uses ebx ecx edx esi edi pszFileName:LPVOID,pImageInfo:LPVOID
	LOCAL	hFile		:	DWORD
	LOCAL	dwImagePtr	:	DWORD
	LOCAL	dwHeaderSize:	DWORD
	LOCAL	dwWriteBytes:	DWORD
	LOCAL	dwResult	:	DWORD
	LOCAL	dwScnNum	:	DWORD
	LOCAL	dwScnHeaderPtr	:	DWORD
			mov		[hFile],0
			mov		[dwImagePtr],0
			mov		[dwHeaderSize],0
			mov		[dwWriteBytes],0
			mov		[dwResult],0
			mov		[dwScnNum],0
			mov		[dwScnHeaderPtr],0
			mov		edi,pImageInfo
			assume	edi:ptr IMAGE_INFO
			test	edi,edi
			jz		loc_fail_ret
			cmp		[edi].ImagePtr,0
			jz		loc_fail_ret
			mov		eax,[edi].ImagePtr
			mov		[dwImagePtr],eax
			mov		eax,[edi].HeaderSize
			mov		[dwHeaderSize],eax
			mov		eax,[edi].ScnNum
			mov		[dwScnNum],eax
			mov		eax,[edi].ScnHeaderPtr
			mov		[dwScnHeaderPtr],eax
			;-=-=打开文件写-=-=
			push	NULL
			push	FILE_ATTRIBUTE_NORMAL
			push	CREATE_ALWAYS
			push	NULL
			push	FILE_SHARE_READ
			push	GENERIC_WRITE
			push	pszFileName
			call	CreateFileA
			cmp		eax,INVALID_HANDLE_VALUE
			jnz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg1,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			mov		[hFile],eax
			;-=-=写入文件头=-=-
			push	NULL
			lea		eax,[dwWriteBytes]
			push	eax
			push	[dwHeaderSize]
			push	[dwImagePtr]
			push	[hFile]
			call	WriteFile
			mov		eax,[dwWriteBytes]
			cmp		eax,[dwHeaderSize]
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg6,addr szErrTitle,MB_OK
			jmp		loc_fail_ret

@@:			mov		esi,[dwScnHeaderPtr]
			assume	esi:ptr IMAGE_SECTION_HEADER
			mov		ecx,[dwScnNum]
loc_loop:	push	ecx
			push	esi
			;-=-=写入区段数据=-=-
			push	NULL
			lea		eax,[dwWriteBytes]
			push	eax
			push	[esi].SizeOfRawData
			mov		eax,[esi].VirtualAddress
			add		eax,[dwImagePtr]
			push	eax
			push	[hFile]
			call	WriteFile
			pop		esi
			pop		ecx
			mov		eax,[dwWriteBytes]
			cmp		eax,[esi].SizeOfRawData
			jz		@F
			invoke	MessageBoxA,NULL,addr szErrMsg6,addr szErrTitle,MB_OK
			jmp		loc_fail_ret
@@:			add		esi,sizeof IMAGE_SECTION_HEADER
			loop	loc_loop
			mov		[dwResult],TRUE
			jmp		loc_safe_ret
loc_fail_ret:
			mov		[dwResult],FALSE
loc_safe_ret:
			cmp		[hFile],0
			jz		@F
			invoke	CloseHandle,[hFile]
@@:			mov		eax,[dwResult]
			ret
SaveImageToFile		endp

;-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; 程序入口
Main:
			;-=-=创建和程序执行时相同的内存映象=-=-
			invoke	CreateImage,addr szOrgFileName,addr sImageInfo
			cmp		eax,FALSE
			jz		gm0_safe_exit
			;-=-=查找特征字符串定位到数据信息处=-=-
			invoke	SearchBlockDataAddr,addr sImageInfo
			test	eax,eax
			jz		gm0_safe_exit
			;-=-=保存每个代码块的偏移地址和大小=-=-
			invoke	SaveBlockData,eax,addr sImageInfo
			;-=-=计算得到密钥流数据=-=-
			invoke	CreateKeyStream
			cmp		eax,FALSE
			jz		gm0_safe_exit
			mov		[KeyStreamPtr],eax
			;-=-=对代码使用密钥流进行加密=-=-
			invoke	StreamEncrypt,[KeyStreamPtr]
			;-=-=保存映象到文件中=-=-
			invoke	SaveImageToFile,addr szNewFileName,addr sImageInfo
gm0_safe_exit:
			mov		edi,[sImageInfo].ImagePtr
			test	edi,edi
			jz		@F
			invoke	ReleaseMemory,edi
@@:			mov		edi,[KeyStreamPtr]
			test	edi,edi
			jz		@F
			invoke	ReleaseMemory,edi
@@:			invoke	ExitProcess,0

end Main
