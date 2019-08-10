BITS 32             ; tell nasm this is 32-bit code

  call mark_below   ; call below the string to instructions
  db "Hello, world!", 0x0a, 0x0d  ; with newline and carriage return bytes

mark_below:
; ssize_t write(int fd, const void *buf, size_t count);
  pop ecx           ; pop the return address (string ptr) into ecx
  mov eax, 4        ; write syscall #
  mov ebx, 1        ; STDOUT file descriptor
  mov edx, 15       ; length of the string
  int 0x80          ; do syscall: write(1, string, 14)

; void _exit(int status);
  mov eax, 1        ; exit syscall #
  mov ebx, 0        ; status = 0   
  int 0x80          ; do syscall:  exit(0)
