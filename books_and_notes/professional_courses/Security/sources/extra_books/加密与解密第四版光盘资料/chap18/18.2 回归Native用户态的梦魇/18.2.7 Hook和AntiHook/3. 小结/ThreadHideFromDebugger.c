//code by shoooo
push    0
push    0
push    11
push    -2
mov     eax, 0C7
mov     edx, esp
int     2E
mov     eax, 0E5
mov     edx, esp
int     2E
mov     eax, 0EE
mov     edx, esp
int     2E
mov     eax, 136
mov     edx, esp
int     2E
add     esp, 10
