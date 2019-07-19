/*-----------------------------------------------------------------------
第18章  反跟踪技术
code by forgot 
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

typedef BOOL (WINAPI *CHECK_REMOTE_DEBUGGER_PRESENT)(HANDLE, PBOOL);
BOOL 
CheckDebugger(
    VOID)
{
    HANDLE      hProcess;
    HINSTANCE   hModule;    
    BOOL        bDebuggerPresent = FALSE;
    CHECK_REMOTE_DEBUGGER_PRESENT CheckRemoteDebuggerPresent;
    hModule = GetModuleHandleA("Kernel32");
    CheckRemoteDebuggerPresent = 
        (CHECK_REMOTE_DEBUGGER_PRESENT)GetProcAddress(hModule, "CheckRemoteDebuggerPresent");
    hProcess = GetCurrentProcess();
    return CheckRemoteDebuggerPresent(
                hProcess, 
                &bDebuggerPresent) ? bDebuggerPresent : FALSE;
}
