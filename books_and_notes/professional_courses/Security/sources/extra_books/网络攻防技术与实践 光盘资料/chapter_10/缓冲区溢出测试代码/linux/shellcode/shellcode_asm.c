
int main ()
{
    __asm__
    ("
        mov    $0x0,%edx
        push   %edx
        push   $0x68732f6e
        push   $0x69622f2f
        mov    %esp,%ebx
        push   %edx
        push   %ebx
        mov    %esp,%ecx
        mov    $0xb,%eax
        int    $0x80
    ");
}
