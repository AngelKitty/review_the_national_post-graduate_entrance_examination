/*******************************************************
/*《加密与解密》第三版配套实例
/*  第15章 反跟踪技术
/*15.1.2 NtGlobalFlag
/* code by forgot 2008.3
/*(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

BOOL
MyIsDebuggerPresentEx(
    VOID)
{
    __asm {
        mov eax, fs:[0x30]
        mov eax, [eax+0x68]
        and eax, 0x70
    }
}
