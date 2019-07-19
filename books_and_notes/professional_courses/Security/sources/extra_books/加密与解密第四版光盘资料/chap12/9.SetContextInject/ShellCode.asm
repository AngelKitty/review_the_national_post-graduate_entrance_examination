.CODE

ShellCodeFun64 PROC

	push    rax
	push    rbx
	pushfq
	call  next
next:
	pop     rbx
	and     bx,0
	mov     rcx,qword ptr [rbx+38h] ; lpDllPath
	call    qword ptr [rbx+30h]		;AddrofLoadLibraryA
	mov     rax,qword ptr [rbx+40h]	;OriginalEIP
	xchg    rax,qword ptr [rsp+10h]	;saved retnaddr
	popfq
	pop     rbx
	ret
	nop
	nop
	nop
	nop
	nop
ShellCodeFun64 ENDP

END