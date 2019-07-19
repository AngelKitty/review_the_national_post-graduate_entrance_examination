/*-----------------------------------------------------------------------
第18章  反跟踪技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

BOOL
HideDebugger(
    HANDLE hThread,
    HANDLE hProcess)
{
    CONTEXT ctx;
    LDT_ENTRY sel;
    DWORD fs;
    DWORD peb;
    SIZE_T bytesrw;
    WORD flag;
    ctx.ContextFlags = CONTEXT_SEGMENTS;
    if (!GetThreadContext(hThread, &ctx))
        return FALSE;
    if (!GetThreadSelectorEntry(hThread, ctx.SegFs, &sel))
        return FALSE;
    fs = (sel.HighWord.Bytes.BaseHi << 8 | sel.HighWord.Bytes.BaseMid) << 16 |
        sel.BaseLow;    
    if (!ReadProcessMemory(hProcess, (LPCVOID)(fs + 0x30), &peb, 4, &bytesrw) ||
        bytesrw != 4)
        return FALSE;
    if (!ReadProcessMemory(hProcess, (LPCVOID)(peb + 0x2), &flag, 2, &bytesrw) ||
        bytesrw != 2)
        return FALSE;
    flag = 0;
    if (!WriteProcessMemory(hProcess, (LPCVOID)(peb + 0x2), &flag, 2, &bytesrw) ||
        bytesrw != 2)
        return FALSE;
    return TRUE;
}
