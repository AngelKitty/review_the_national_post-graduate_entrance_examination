/*-----------------------------------------------------------------------
第18章  反跟踪技术 
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

BOOL
CheckRemoteDebuggerPresent(
    HANDLE	hProcess, 
    PBOOL		pbDebuggerPresent)
{
    DWORD rv;
    if (hProcess & pbDebuggerPresent) {
        rv = NtQueryInformationProcess(hProcess, 7, &hProcess, 4, 0);
        if (rv < 0) {
            BaseSetLastNTError(rv); //事实上这个函数名是用IDA打开kernel32.dll得到的
            return FALSE;
        } else {
            pbDebuggerPresent = hProcess;
            return TRUE;
        }
    } else {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
}
