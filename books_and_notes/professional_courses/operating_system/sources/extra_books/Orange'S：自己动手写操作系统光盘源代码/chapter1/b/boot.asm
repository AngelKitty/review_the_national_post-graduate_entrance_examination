;%define _BOOT_DEBUG_		; 制作 Boot Sector 时一定将此行注释掉!
				; 去掉此行注释后可做成.COM文件易于调试:
				;   nasm Boot.asm -o Boot.com 

%ifdef  _BOOT_DEBUG_
	org  0100h		; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h		; BIOS 将把 Boot Sector 加载到 0:7C00 处
%endif

	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	call	DispStr		; 调用显示字符串例程
	jmp	$		; 无限循环
DispStr:
	mov	ax, BootMessage
	mov	bp, ax		; ES:BP = 串地址
	mov	cx, 16		; CX = 串长度
	mov	ax, 01301h	; AH = 13,  AL = 01h
	mov	bx, 000ch	; 页号为0(BH = 0) 黑底红字(BL = 0Ch,高亮)
	mov	dl, 0
	int	10h		; int 10h
	ret
BootMessage:		db	"Hello, OS world!"
times 	510-($-$$)	db	0 ; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55			  ; 结束标志
