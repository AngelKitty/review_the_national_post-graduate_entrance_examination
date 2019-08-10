section .data       ; data segment
msg     db      "Hello, world!", 0x0a   ; the string and newline char

section .text       ; text segment
global _start       ; Default entry point for ELF linking

_start:
  ; SYSCALL: write(1, msg, 14) 
  mov eax, 4        ; put 4 into eax, since write is syscall #4
  mov ebx, 1        ; put 1 into ebx, since stdout is 1
  mov ecx, msg      ; put the address of the string into ecx
  mov edx, 14       ; put 14 into edx, since our string is 14 bytes
  int 0x80          ; Call the kernel to make the system call happen

  ; SYSCALL: exit(0)
  mov eax, 1        ; put 1 into eax, since exit is syscall #1
  mov ebx, 0        ; exit with success
  int 0x80          ; do the syscall
