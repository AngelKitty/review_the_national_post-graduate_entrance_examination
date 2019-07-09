
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               hdboot.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2008
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

org  0x7c00			; bios always loads boot sector to 0000:7C00

	jmp	boot_start

%include		"load.inc"

STACK_BASE		equ	0x7C00	; base address of stack when booting
TRANS_SECT_NR		equ	2
SECT_BUF_SIZE		equ	TRANS_SECT_NR * 512

disk_address_packet:	db	0x10		; [ 0] Packet size in bytes. Must be 0x10 or greater.
			db	0		; [ 1] Reserved, must be 0.
sect_cnt:		db	TRANS_SECT_NR	; [ 2] Number of blocks to transfer.
			db	0		; [ 3] Reserved, must be 0.
			dw	0		; [ 4] Address of transfer buffer. Offset
			dw	SUPER_BLK_SEG	; [ 6]                             Seg
lba_addr:		dd	0		; [ 8] Starting LBA address. Low  32-bits.
			dd	0		; [12] Starting LBA address. High 32-bits.

	
err:
	mov	dh, 3			; "Error 0  "
	call	disp_str		; display the string
	jmp	$

boot_start:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, STACK_BASE

	call	clear_screen

	mov	dh, 0			; "Booting  "
	call	disp_str		; display the string

	;; read the super block to SUPER_BLK_SEG::0
	mov	dword [disk_address_packet +  8], ROOT_BASE + 1
	call	read_sector
	mov	ax, SUPER_BLK_SEG
	mov	fs, ax

	mov	dword [disk_address_packet +  4], LOADER_OFF
	mov	dword [disk_address_packet +  6], LOADER_SEG

	;; get the sector nr of `/' (ROOT_INODE), it'll be stored in eax
	mov	eax, [fs:SB_ROOT_INODE]
	call	get_inode

	;; read `/' into ex:bx
	mov	dword [disk_address_packet +  8], eax
	call	read_sector

	;; let's search `/' for the loader
	mov	si, LoaderFileName
	push	bx		; <- save
.str_cmp:
	;; before comparation:
	;;     es:bx -> dir_entry @ disk
	;;     ds:si -> filename we want
	add	bx, [fs:SB_DIR_ENT_FNAME_OFF]
.1:
	lodsb				; ds:si -> al
	cmp	al, byte [es:bx]
	jz	.2
	jmp	.different		; oops
.2:					; so far so good
	cmp	al, 0			; both arrive at a '\0', match
	jz	.found
	inc	bx			; next char @ disk
	jmp	.1			; on and on
.different:
	pop	bx		; -> restore
	add	bx, [fs:SB_DIR_ENT_SIZE]
	sub	ecx, [fs:SB_DIR_ENT_SIZE]
	jz	.not_found
	push	bx
	mov	si, LoaderFileName
	jmp	.str_cmp
.not_found:
	mov	dh, 2
	call	disp_str
	jmp	$
.found:
	nop
	nop
	nop
	pop	bx
	add	bx, [fs:SB_DIR_ENT_INODE_OFF]
	mov	eax, [es:bx]		; eax <- inode nr of loader
	call	get_inode		; eax <- start sector nr of loader
	mov	dword [disk_address_packet +  8], eax
load_loader:
	call	read_sector
	cmp	ecx, SECT_BUF_SIZE
	jl	.done
	sub	ecx, SECT_BUF_SIZE	; bytes_left -= SECT_BUF_SIZE
	add	word  [disk_address_packet + 4], SECT_BUF_SIZE ; transfer buffer
	jc	err
	add	dword [disk_address_packet + 8], TRANS_SECT_NR ; LBA
	jmp	load_loader
.done:
	mov	dh, 1
	call	disp_str
	jmp	LOADER_SEG:LOADER_OFF
	jmp	$


;============================================================================
;字符串
;----------------------------------------------------------------------------
LoaderFileName		db	"hdloader.bin", 0	; LOADER.BIN 之文件名
; 为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	9
BootMessage:		db	"Booting  "; 9字节, 不够则用空格补齐. 序号 0
Message1		db	"BootReady"; 9字节, 不够则用空格补齐. 序号 1
Message2		db	"No LOADER"; 9字节, 不够则用空格补齐. 序号 2
Message3		db	"Error 0  "; 9字节, 不够则用空格补齐. 序号 3
;============================================================================

clear_screen:
	mov	ax, 0x600		; AH = 6,  AL = 0
	mov	bx, 0x700		; 黑底白字(BL = 0x7)
	mov	cx, 0			; 左上角: (0, 0)
	mov	dx, 0x184f		; 右下角: (80, 50)
	int	0x10			; int 0x10
	ret

;----------------------------------------------------------------------------
; 函数名: disp_str
;----------------------------------------------------------------------------
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
disp_str:
	mov	ax, MessageLength
	mul	dh
	add	ax, BootMessage
	mov	bp, ax			; ┓
	mov	ax, ds			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	mov	ax, 0x1301		; AH = 0x13,  AL = 0x1
	mov	bx, 0x7			; 页号为0(BH = 0) 黑底白字(BL = 0x7)
	mov	dl, 0
	int	0x10			; int 0x10
	ret

;----------------------------------------------------------------------------
; read_sector
;----------------------------------------------------------------------------
; before:
;     - fields disk_address_packet should have been filled
;       before invoking the routine
; after:
;     - es:bx -> data read
; registers changed:
;     - eax, ebx, dl, si, es
read_sector:
	xor	ebx, ebx

	;mov	dword [disk_address_packet +  8], eax
	mov	dword [disk_address_packet + 12], 0

	mov	ah, 0x42
	mov	dl, 0x80
	mov	si, disk_address_packet
	int	0x13

	mov	ax, [disk_address_packet + 6]
	mov	es, ax
	mov	bx, [disk_address_packet + 4]

	ret

;----------------------------------------------------------------------------
; get_inode
;----------------------------------------------------------------------------
; before:
;     - eax    : inode nr.
; after:
;     - eax    : sector nr.
;     - ecx    : the_inode.i_size
;     - es:ebx : inodes sector buffer
; registers changed:
;     - eax, ebx, ecx, edx
get_inode:
	dec	eax				; eax <-  inode_nr -1
	mov	bl, [fs:SB_INODE_SIZE]
	mul	bl				; eax <- (inode_nr - 1) * INODE_SIZE
	mov	edx, SECT_BUF_SIZE
	sub	edx, dword [fs:SB_INODE_SIZE]
	cmp	eax, edx
	jg	err
	push	eax

	mov	ebx, [fs:SB_NR_IMAP_SECTS]
	mov	edx, [fs:SB_NR_SMAP_SECTS]
	lea	eax, [ebx+edx+ROOT_BASE+2]
	mov	dword [disk_address_packet +  8], eax
	call	read_sector

	pop	eax				; [es:ebx+eax] -> the inode

	mov	edx, dword [fs:SB_INODE_ISIZE_OFF]
	add	edx, ebx
	add	edx, eax			; [es:edx] -> the_inode.i_size
	mov	ecx, [es:edx]			; ecx <- the_inode.i_size

	add	ax, word [fs:SB_INODE_START_OFF]; es:[ebx+eax] -> the_inode.i_start_sect

	add	bx, ax
	mov	eax, [es:bx]
	add	eax, ROOT_BASE			; eax <- the_inode.i_start_sect
	ret


times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志
