/*-----------------------------------------------------------------------
第18章  反跟踪技术
code by forgot
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

ULONG GetPebBase(ULONG ProcessId)
{
    HANDLE hProcess = NULL;
    PROCESS_BASIC_INFORMATION pbi = {0};
    ULONG peb = 0;        
    ULONG cnt = 0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessId);
    if (hProcess != NULL) {
        if (NtQueryInformationProcess(
                hProcess,
                ProcessBasicInformation,
                &pbi,
                sizeof(PROCESS_BASIC_INFORMATION),
                &cnt) == 0) {
            PebBase = (ULONG)pbi.PebBaseAddress;
        }            
        CloseHandle(hProcess);
    }
}
