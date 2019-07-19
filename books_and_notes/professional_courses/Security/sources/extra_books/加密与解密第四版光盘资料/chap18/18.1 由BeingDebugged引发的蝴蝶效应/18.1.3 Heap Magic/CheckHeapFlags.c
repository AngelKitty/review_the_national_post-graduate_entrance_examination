/*******************************************************
/*《加密与解密》第三版配套实例
/*  第15章 反跟踪技术
/*15.1.3 Heap Magic
/* code by forgot 2008.3
/*(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

BOOL
CheckHeapFlags(
    VOID)
{
    __asm {
        mov eax, fs:[0x30]
        mov eax, [eax+0x18]
        cmp dword ptr [eax+0x0C], 2
        jne __debugger_detected
        cmp dword ptr [eax+0x10], 0
        jne __debugger_detected
        xor eax, eax
__debugger_detected:
    }
}
