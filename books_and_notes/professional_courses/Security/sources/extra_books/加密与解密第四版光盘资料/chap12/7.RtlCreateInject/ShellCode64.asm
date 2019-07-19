.CODE

ShellCodeFun64 PROC
	push    rax						;eq to sub rsp,8
	push    rbx
	pushfq
	call  next
next:
	pop     rbx
	and     bx,0					;rbx = Allocated Mem base
	mov     rcx,qword ptr [rbx+38h]	;lpParameter
	call    qword ptr [rbx+30h]		;lpThreadStartRoutine
	xor		rax,rax					;ExitStatus
	lea		rcx,[rax-2]				;-2 = CurrentThread
	mov		rdx,rax
	call    qword ptr [rbx+40h]		;ZwTerminateThread
	popfq							;no return here
	pop     rbx
	ret
	nop
	nop
	nop
	nop
	nop
ShellCodeFun64 ENDP

END