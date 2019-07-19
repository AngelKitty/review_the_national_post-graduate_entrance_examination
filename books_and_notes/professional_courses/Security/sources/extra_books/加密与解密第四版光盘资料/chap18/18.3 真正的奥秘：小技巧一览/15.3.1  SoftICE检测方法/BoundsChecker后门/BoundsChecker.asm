; 
;《加密与解密》第三版配套实例
; (c)  看雪软件安全网站 www.pediy.com 2000-2008

start:                  assume  fs: flat

                        call    __install_seh                   ; SoftICE 不存在时 int 3 会导致异常

                        mov     ecx, [esp+3*4]                  ; -> CONTEXT
                        inc     [ecx.CONTEXT.regEip]            ; 跳过 int 3
                        xor     eax, eax
                        retn

__install_seh:          push    fs:[0]
                        mov     fs:[0], esp

                        mov     ebp, 'BCHK'
                        mov     ax, 4
                        int     3

                        pop     fs:[0]
                        add     esp, 4
                        cmp     al, 4                           ; SoftICE 存在时 AL 会发生变化
                        je      __no_debugger

                        push    0
                        push    0
                        push    CTXT("debugger detected!")
                        push    0
                        call    MessageBoxA
__no_debugger:
                        call    ExitProcess
