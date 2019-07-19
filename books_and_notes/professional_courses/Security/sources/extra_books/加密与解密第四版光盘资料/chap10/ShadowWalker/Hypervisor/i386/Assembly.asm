comment $
===========================================================================
 * Assembly	:操作Intel VT的主要汇编函数
 * Author	:Joen
 * QQ		:51753931
 * E-mail	:Joen@JoenChen.com
 * Website	:http://www.joenchen.com
 * 这个文件是开启VT时, 依赖的一些汇编指令函数
 */
;===========================================================================$
	.686p
	.model flat, stdcall
	option casemap:none
	
STATUS_UNSUCCESSFUL	equ	0C0000001h
STATUS_SUCCESS		equ	00000000h

HandleVmExit		Proto :dword, :dword
;===========================================================================	
	.Code

EXTERN	 LoadHypervisor@4:Proc

_GetEflags	Proc
	pushfd
	pop	eax
	ret
_GetEflags	Endp
;===========================================================================
_GetCsSelector	Proc
	xor	eax, eax
	mov	ax,cs
	ret
_GetCsSelector	Endp

_GetDsSelector	Proc
	xor	eax, eax
	mov	ax,ds
	ret
_GetDsSelector	Endp

_GetEsSelector	Proc
	xor	eax, eax
	mov	ax,es
	ret
_GetEsSelector	Endp

_GetFsSelector	Proc
	xor	eax, eax
	mov	ax,fs
	ret
_GetFsSelector	Endp

_GetGsSelector	Proc
	xor	eax, eax
	mov	ax,gs
	ret
_GetGsSelector	Endp

_GetSsSelector	Proc
	xor	eax, eax
	mov	ax,ss
	ret
_GetSsSelector	Endp
	
_GetLdtrSelector Proc
	xor	eax, eax
	sldt	ax
	ret
_GetLdtrSelector Endp

_GetTrSelector	Proc
	xor	eax, eax
	str	ax
	ret
_GetTrSelector	Endp
;===========================================================================
_GetCr0		Proc
	
	mov 	eax, cr0
	ret
	
_GetCr0		Endp

_GetCr3		Proc
	
	mov 	eax, cr3
	ret
_GetCr3		Endp

_GetCr4		Proc
	mov 	eax, cr4
	ret
_GetCr4 	Endp
;===========================================================================
_SetCr2		Proc 	NewCr2:dword
	
	mov 	eax, NewCr2
	mov	cr2, eax
	ret
_SetCr2 	Endp

_SetCr3		Proc 	NewCr3:dword
	
	mov 	eax, NewCr3
	mov	cr3, eax
	ret
_SetCr3		Endp

_SetCr4		Proc	NewCr4:dword
	
	mov 	eax,NewCr4
	mov 	cr4, eax
	ret
	
_SetCr4		Endp
;===========================================================================
_ReadMsr	Proc	Index:dword
	
	mov	ecx,Index
	rdmsr
	ret
_ReadMsr	Endp

_WriteMsr	Proc	Index:dword,LowPart,HighPart
		
	mov	ecx, Index
	mov	eax, LowPart
	mov	edx, HighPart
	wrmsr
	ret
	
_WriteMsr 	Endp
;===========================================================================
_ExecuteVmxOn	Proc	LowPtr:dword,HighPtr:dword
	
	push	HighPtr
	push	LowPtr

	vmxon	qword ptr [esp]
	add	esp,8
	ret
	
_ExecuteVmxOn 	Endp
;===========================================================================
;	这个函数不会返回.会进入到VMCB中设置的Guest Eip中.  
;===========================================================================
_ExecuteVmLaunch Proc
	
	vmlaunch
	ret
_ExecuteVmLaunch Endp

;===========================================================================
_ExecuteVmClear	Proc	LowPtr:dword, HighPtr:dword

	push	HighPtr
	push	LowPtr

	vmclear	qword ptr [esp]

	add	esp,8
	ret
_ExecuteVmClear Endp

_ExecuteInvd	Proc
	invd
	ret
_ExecuteInvd	Endp

;===========================================================================
_ExecuteVmPtrLd	Proc	LowPtr:dword, HighPtr:dword
	
	push	HighPtr
	push	LowPtr
	vmptrld	qword ptr [esp]
	add	esp,8
	ret
_ExecuteVmPtrLd Endp

_ExecuteCpuId	Proc	uses ebx esi edi fn:dword, ret_eax:dword,ret_ebx:dword,ret_ecx:dword, ret_edx:dword
	
	
        mov	eax, fn
        cpuid
        mov	esi, ret_eax
        mov	dword ptr [esi], eax
        mov	esi, ret_ebx
        mov	dword ptr [esi], ebx
        mov	esi, ret_ecx
        mov	dword ptr [esi], ecx
        mov	esi, ret_edx
        mov	dword ptr [esi], edx
       
        ret
_ExecuteCpuId 	Endp

_ExecuteVmcall	Proc	uses ebx esi edi _eax:dword, _ebx:dword, _ecx:dword, _edx:dword
	
	mov 	eax, _eax
	mov 	ebx, _ebx
	mov 	ecx, _ecx
	mov 	edx, _edx
	vmcall

	ret
	
_ExecuteVmcall 	Endp

_InvVpidAllContext	Proc
	
	;invtype = 2 刷新除了VPID=0以外的所有VPID
	push	eax
        mov	eax, 2
        push	0
        push	0
        push	0
        push	0

	db	66h, 0fh, 38h, 81h, 04h, 24h	;Invvpid eax, [esp]
        add	esp, 16
        pop	eax
	ret
	
_InvVpidAllContext 	Endp

_InvEptAllContext	Proc
	
	;eax = 2 all-context刷新模式,所有ep4ta刷新,忽略第二个参数
	push	eax
        mov	eax, 2

        push	0
        push	0
        push	0
        push	0

	db 	66h, 0fh, 38h, 80h, 04h, 24h	;Invept eax, [esp]

        add	esp, 16
        pop	eax
	ret
	
_InvEptAllContext 	Endp
;===========================================================================
_ExecuteVmxOff	Proc	RegEsp:dword, RegEip:dword
	
	push	00006818h	;GUEST_IDTR_BASE
	call	_ReadVMCS
	push	eax
	push	00004812h	;GUEST_IDTR_LIMIT
	call	_ReadVMCS
	shl	eax,16
	push	eax
	lidt	fword ptr [esp+2]
	pop	eax
	pop	eax

	push	00006816h	;GUEST_GDTR_BASE
	call	_ReadVMCS
	push	eax
	push	00004810h	;GUEST_GDTR_LIMIT
	call	_ReadVMCS
	shl	eax,16
	push	eax
	lgdt	fword ptr [esp+2]
	pop	eax
	pop	eax

	push	00006800h	;GUEST_CR0
	call	_ReadVMCS
	mov	cr0,eax

	push	00006802h	;GUEST_CR3
	call	_ReadVMCS
	mov	cr3,eax

	push	00006804h	;GUEST_CR4
	call	_ReadVMCS
	mov	cr4,eax
	
	mov	eax,RegEip
	mov	ecx,RegEsp
	
	vmxoff
	leave
	
	mov	esp,ecx
	jmp	eax
	
_ExecuteVmxOff 	Endp
;===========================================================================
_VmFailInvalid	Proc	uses ecx

	pushfd
	pop	eax

	xor	ecx, ecx
	bt	eax, 0	;RFLAGS.CF
	adc	cl, cl
	mov	eax, ecx
	ret

_VmFailInvalid	Endp

_VmLaunchFailValid Proc	uses ecx
	
	pushfd
	pop	eax
	xor	ecx, ecx
	bt	eax, 6	;RFLAGS.ZF
	adc	cl, cl
	mov	eax, ecx
	ret
	
_VmLaunchFailValid Endp

;===========================================================================
_WriteVMCS	Proc	uses ecx Field:dword,Value:dword
	
	mov	eax,Field
	mov	ecx,Value
	vmwrite	eax,ecx
	ret
	
_WriteVMCS	Endp

_ReadVMCS	Proc	uses ecx Field:dword
	
	mov	eax,Field
	vmread	ecx,eax
	mov	eax,ecx
	ret
	
_ReadVMCS	Endp

_GetGdtBase	Proc
	Local	gdtr[6]:byte
	
	sgdt	gdtr
	mov	eax,dword ptr gdtr[2]
	ret
_GetGdtBase	Endp

;===========================================================================
;	获取idt表基址
;===========================================================================
_GetIdtBase	Proc
	Local	idtr[6]:byte
	
	sidt	idtr
	mov	eax,dword ptr idtr[2]
	ret
  
_GetIdtBase 	Endp

_SetIdtr	Proc	Base:dword, Limit:dword
	
	push	Base
	shl	Limit, 16
	push	Limit
	
	lidt	fword ptr [esp+2]
	pop	eax
	pop	eax
	ret
_SetIdtr	Endp

_GetGdtLimit	Proc
	Local	gdtr[6]:byte
	
	sgdt	gdtr
	xor	eax, eax
	mov	ax,word ptr gdtr[0]
	ret
_GetGdtLimit	Endp

_GetIdtLimit	Proc
	Local	idtr[6]:byte
	
	sidt	idtr
	xor	eax, eax
	mov	ax,word ptr idtr[0]
	ret
_GetIdtLimit	Endp
;===========================================================================
;这个函数看上去有点奇怪。如果开启VT成功，这个函数不会再返回
;这个函数和_GuestEntryPoint配合实现栈平衡
;===========================================================================
_LoadHypervisor Proc StdCall _GuestEsp

	pushad
	mov	eax, esp
	push	eax        ;setup esp to argv[0]
	call	LoadHypervisor@4
	popad
	ret

_LoadHypervisor Endp
;===========================================================================
;开启VT成功以后，这里就是Guest入口 
;===========================================================================
_GuestEntryPoint Proc	StdCall _GuestEsp
	
	pop	ebp
	popad
	ret
_GuestEntryPoint Endp

;===========================================================================
;	关闭VT技术, 这个函数也是不会返回的
;	这个函数配合_GuestExitPoint实现堆栈平衡
;===========================================================================
_MakeHyperExitCall	Proc 
	
	pushad
	pushfd
	push	ebp
	
	mov 	eax, 'Joen'
	mov 	ecx, 'Chen'
	mov 	edx, 'SW'
	mov 	ebx, 'Quit'
	vmcall
	nop
	
	popfd
	popad
	mov 	eax, STATUS_UNSUCCESSFUL
	ret
	
_MakeHyperExitCall Endp
;===========================================================================
;	这是关闭VT以后的Guest退出函数的入口.
;===========================================================================
_GuestExitPoint	Proc
	
	pop	ebp
	popfd
	popad
	mov	eax, STATUS_SUCCESS
	ret
	
_GuestExitPoint Endp
;===========================================================================
; 开启VT成功以后, 这就是Guest 进入#root模式的入口, 注意这里保存和恢复寄存器
; 不一样, 不要用pushad popad
;===========================================================================
_ExitEventHandler	Proc
	
	cli
	
	push 	edi
	push 	esi
	push 	ebp
	push 	esp
	push 	ebx
	push 	edx
	push 	ecx
	push 	eax
	
	mov	ebx, esp	; Guest Reg
	lea 	eax, [esp+20h]	; Cpu
	
	push	ebx		; Guest Reg
	push 	eax		; Cpu
	call	HandleVmExit

	pop 	eax
	pop 	ecx
	pop 	edx
	pop 	ebx
	pop 	ebp 		; pop esp
	pop 	ebp
	pop 	esi
	pop 	edi
	
	;popfd
	vmresume
	ret
_ExitEventHandler	Endp

End	