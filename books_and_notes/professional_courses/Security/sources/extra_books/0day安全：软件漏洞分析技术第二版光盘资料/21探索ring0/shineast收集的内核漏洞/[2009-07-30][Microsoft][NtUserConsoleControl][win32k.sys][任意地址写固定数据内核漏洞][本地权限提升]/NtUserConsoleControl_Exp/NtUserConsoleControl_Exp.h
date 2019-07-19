#define IMP_VOID __declspec(dllimport) VOID __stdcall
#define IMP_PVOID __declspec(dllimport) PVOID __stdcall
#define IMP_SYSCALL __declspec(dllimport) NTSTATUS __stdcall

#define PAGE_SIZE 0x1000

#define NtCurrentProcess() ((HANDLE)0xFFFFFFFF)

#define KERNEL_NAME_LENGTH 0x0D

#define STATUS_SUCCESS 0x00000000
#define STATUS_UNSUCCESSFUL 0xC0000001

#define SE_DEBUG_PRIVILEGE 0x14

#define IMAGE_NAME_LENGTH 0x18
#define IMAGE_NAME L"services.exe"

#define OBJECT_NAME_LENGTH sizeof(UNICODE_STRING) + (MAX_PATH + 1) * sizeof(WCHAR)

#define DUPLICATE_SAME_ATTRIBUTES   0x00000004

typedef ULONG NTSTATUS;

typedef struct ANSI_STRING
{
    /* 0x00 */ USHORT Length;
    /* 0x02 */ USHORT MaximumLength;
    /* 0x04 */ PCHAR Buffer;
    /* 0x08 */
}
    ANSI_STRING,
  *PANSI_STRING,
**PPANSI_STRING;

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
	/* 0x00 */ HANDLE UniqueProcess;
	/* 0x04 */ HANDLE UniqueThread;
	/* 0x08 */
}
	CLIENT_ID,
  *PCLIENT_ID,
**PPCLIENT_ID;

typedef struct _USER_STACK
{
	/* 0x00 */ PVOID FixedStackBase;
	/* 0x04 */ PVOID FixedStackLimit;
	/* 0x08 */ PVOID ExpandableStackBase;
	/* 0x0C */ PVOID ExpandableStackLimit;
	/* 0x10 */ PVOID ExpandableStackBottom;
	/* 0x14 */
}
	USER_STACK,
  *PUSER_STACK,
**PPUSER_STACK;
 
typedef struct _CSR_MESSAGE
{ 
    /* 0x00 */ USHORT DataLength;
    /* 0x02 */ USHORT TotalLength;
    /* 0x04 */ ULONG ZeroInit;
    /* 0x08 */ LARGE_INTEGER DoNotUseThisField;
    /* 0x10 */ ULONG MessageId;
    /* 0x14 */ ULONG ClientViewSize;

    /* 0x18 */ PVOID CaptureBuffer;
    /* 0x1C */ ULONG ApiNumber;
    /* 0x20 */ ULONG ReturnValue;
    /* 0x24 */ ULONG Reserved;
    /* 0x28 */ HANDLE ThreadHandle;
    /* 0x2C */ CLIENT_ID ClientId;
    /* 0x34 */ ULONG Unknown;
    /* 0x38 */
}
    CSR_MESSAGE,
  *PCSR_MESSAGE,
**PPCSR_MESSAGE;

typedef struct _PEB_LDR_DATA
{
    /* 0x0000 */ ULONG Length;
    /* 0x0004 */ BOOLEAN Initialized;
    /* 0x0008 */ HANDLE SsHandle;
    /* 0x000C */ LIST_ENTRY InLoadOrderModuleList;
    /* 0x0014 */ LIST_ENTRY InMemoryOrderModuleList;
    /* 0x001C */ LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    /* 0x0000 */ LIST_ENTRY InLoadOrderLinks;
    /* 0x0008 */ LIST_ENTRY InMemoryOrderLinks;
    /* 0x0010 */ LIST_ENTRY InInitializationOrderLinks;
    /* 0x0018 */ PVOID DllBase;
    /* 0x001C */ PVOID EntryPoint;
    /* 0x0020 */ ULONG SizeOfImage;
    /* 0x0024 */ UNICODE_STRING FullDllName;
    /* 0x002C */ UNICODE_STRING BaseDllName;
    /* 0x0034 */ ULONG Flags;
    /* 0x0038 */ USHORT LoadCount;
    /* 0x003A */ USHORT TlsIndex;
    union
	{
        /* 0x003C */ LIST_ENTRY HashLinks;
        struct
		{
            /* 0x003C */ PVOID SectionPointer;
            /* 0x0040 */ ULONG CheckSum;
        };
    };
    /* 0x0044 */ ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
	/* 0x0000 */ USHORT Flags;
	/* 0x0002 */ USHORT Length;
	/* 0x0004 */ ULONG TimeStamp;
	/* 0x0008 */ UNICODE_STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	/* 0x0000 */ ULONG MaximumLength;
	/* 0x0004 */ ULONG Length;
	/* 0x0008 */ ULONG Flags;
	/* 0x000C */ ULONG DebugFlags;
	/* 0x0010 */ PVOID ConsoleHandle;
	/* 0x0014 */ ULONG ConsoleFlags;
	/* 0x0018 */ HANDLE StdInputHandle;
	/* 0x001C */ HANDLE StdOutputHandle;
	/* 0x0020 */ HANDLE StdErrorHandle;
	/* 0x0024 */ UNICODE_STRING CurrentDirectoryPath;
	/* 0x002C */ HANDLE CurrentDirectoryHandle;
	/* 0x0030 */ UNICODE_STRING DllPath;
	/* 0x0038 */ UNICODE_STRING ImagePathName;
	/* 0x0040 */ UNICODE_STRING CommandLine;
	/* 0x0048 */ PVOID Environment;
	/* 0x004C */ ULONG StartingPositionLeft;
	/* 0x0050 */ ULONG StartingPositionTop;
	/* 0x0054 */ ULONG Width;
	/* 0x0058 */ ULONG Height;
	/* 0x005C */ ULONG CharWidth;
	/* 0x0060 */ ULONG CharHeight;
	/* 0x0064 */ ULONG ConsoleTextAttributes;
	/* 0x0068 */ ULONG WindowFlags;
	/* 0x006C */ ULONG ShowWindowFlags;
	/* 0x0070 */ UNICODE_STRING WindowTitle;
	/* 0x0078 */ UNICODE_STRING DesktopName;
	/* 0x0080 */ UNICODE_STRING ShellInfo;
	/* 0x0088 */ UNICODE_STRING RuntimeData;
	/* 0x0090 */ RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB
{
	/* 0x0000 */ BOOLEAN InheritedAddressSpace;
	/* 0x0001 */ BOOLEAN ReadImageFileExecOptions;
	/* 0x0002 */ BOOLEAN BeingDebugged;
	/* 0x0003 */ BOOLEAN Spare;
	/* 0x0004 */ HANDLE Mutant;
	/* 0x0008 */ PVOID ImageBaseAddress;
	/* 0x000C */ PPEB_LDR_DATA LoaderData;
	/* 0x0010 */ PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	/* 0x0014 */ PVOID SubSystemData;
	/* 0x0018 */ PVOID ProcessHeap;
  	/* 0x001C */ PVOID FastPebLock;
  	/* 0x0020 */ PVOID FastPebLockRoutine;
  	/* 0x0024 */ PVOID FastPebUnlockRoutine;
  	/* 0x0028 */ ULONG EnvironmentUpdateCount;
  	/* 0x002C */ PVOID *KernelCallbackTable;
  	/* 0x0030 */ PVOID EventLogSection;
  	/* 0x0034 */ PVOID EventLog;
  	/* 0x0038 */ PVOID FreeList;
  	/* 0x003C */ ULONG TlsExpansionCounter;
  	/* 0x0040 */ PVOID TlsBitmap;
  	/* 0x0044 */ ULONG TlsBitmapBits[2];
  	/* 0x004C */ PVOID ReadOnlySharedMemoryBase;
  	/* 0x0050 */ PVOID ReadOnlySharedMemoryHeap;
  	/* 0x0054 */ PVOID *ReadOnlyStaticServerData;
  	/* 0x0058 */ PVOID AnsiCodePageData;
  	/* 0x005C */ PVOID OemCodePageData;
  	/* 0x0060 */ PVOID UnicodeCaseTableData;
  	/* 0x0064 */ ULONG NumberOfProcessors;
  	/* 0x0068 */ ULONG NtGlobalFlag;
  	/* 0x006C */ BYTE Spare2[4];
  	/* 0x0070 */ LARGE_INTEGER CriticalSectionTimeout;
  	/* 0x0078 */ ULONG HeapSegmentReserve;
  	/* 0x007C */ ULONG HeapSegmentCommit;
  	/* 0x0080 */ ULONG HeapDeCommitTotalFreeThreshold;
  	/* 0x0084 */ ULONG HeapDeCommitFreeBlockThreshold;
  	/* 0x0088 */ ULONG NumberOfHeaps;
  	/* 0x008C */ ULONG MaximumNumberOfHeaps;
  	/* 0x0090 */ PVOID **ProcessHeaps;
  	/* 0x0094 */ PVOID GdiSharedHandleTable;
  	/* 0x0098 */ PVOID ProcessStarterHelper;
  	/* 0x009C */ PVOID GdiDCAttributeList;
  	/* 0x00A0 */ PVOID LoaderLock;
  	/* 0x00A4 */ ULONG OSMajorVersion;
  	/* 0x00A8 */ ULONG OSMinorVersion;
  	/* 0x00AC */ USHORT OSBuildNumber;
  	/* 0x00AE */ USHORT OSCSDVersion;
  	/* 0x00B0 */ ULONG OSPlatformId;
  	/* 0x00B4 */ ULONG ImageSubSystem;
  	/* 0x00B8 */ ULONG ImageSubSystemMajorVersion;
  	/* 0x00BC */ ULONG ImageSubSystemMinorVersion;
  	/* 0x00C0 */ ULONG ImageProcessAffinityMask;
  	/* 0x00C4 */ ULONG GdiHandleBuffer[34];
  	/* 0x014C */ ULONG PostProcessInitRoutine;
	/* 0x0150 */ ULONG TlsExpansionBitmap;
	/* 0x0154 */ ULONG TlsExpansionBitmapBits[32];
	/* 0x01D4 */ ULONG SessionId;
	/* 0x01D8 */ PVOID AppCompatInfo;
	/* 0x01DC */ UNICODE_STRING CSDVersion;
} PEB, *PPEB;

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

typedef LONG KPRIORITY;

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
    HANDLE ProcessId;
    HANDLE InheritedFromProcessId;
    ULONG HandleCount;
    ULONG Reserved2[2];
    VM_COUNTERS VmCounters;
    IO_COUNTERSEX IoCounters;
    SYSTEM_THREADS Threads[1];
}
    SYSTEM_PROCESSES,
  *PSYSTEM_PROCESSES,
**PPSYSTEM_PROCESSES;

typedef struct _SYSTEM_MODULE_INFORMATION
{
    /* 0x0000 */ ULONG Reserved[2];
    /* 0x0008 */ PVOID Base;
    /* 0x000C */ ULONG Size;
    /* 0x0010 */ ULONG Flags;
    /* 0x0014 */ USHORT Index;
    /* 0x0016 */ USHORT Unknown;
    /* 0x0018 */ USHORT LoadCount;
    /* 0x001A */ USHORT ModuleNameOffset;
    /* 0x001C */ UCHAR ImageName[256];
    /* 0x011C */
}
    SYSTEM_MODULE_INFORMATION,
  *PSYSTEM_MODULE_INFORMATION,
**PPSYSTEM_MODULE_INFORMATION;

typedef struct _SYSTEM_MODULE_INFORMATION_EX
{
    /* 0x00 */ ULONG ModulesCount;
    /* 0x04 */ SYSTEM_MODULE_INFORMATION Modules[0];
    /* 0xXX */
}
    SYSTEM_MODULE_INFORMATION_EX,
  *PSYSTEM_MODULE_INFORMATION_EX,
**PPSYSTEM_MODULE_INFORMATION_EX;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    /* 0x00 */ ULONG ProcessId;
    /* 0x04 */ UCHAR ObjectTypeNumber;
    /* 0x05 */ UCHAR Flags;
    /* 0x06 */ USHORT Handle;
    /* 0x08 */ ULONG Object;
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
    UNICODE_STRING Name;
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
    /* 0x40 */ ULONG ValidAccess;
    /* 0x44 */ UCHAR Unknown;
    /* 0x45 */ BOOLEAN MaintainHandleDatabase;
    /* 0x46 */ UCHAR Reserved3[2];
    /* 0x48 */ ULONG PoolType;
    /* 0x4C */ ULONG PagedPoolUsage;
    /* 0x50 */ ULONG NonPagedPoolUsage;
	/* 0x54 */
}
	OBJECT_TYPE_INFORMATION,
  *POBJECT_TYPE_INFORMATION,
**PPOBJECT_TYPE_INFORMATION;

typedef enum _KPROFILE_SOURCE
{
    ProfileTime,
    ProfileAlignmentFixup,
    ProfileTotalIssues,
    ProfilePipelineDry,
    ProfileLoadInstructions,
    ProfilePipelineFrozen,
    ProfileBranchInstructions,
    ProfileTotalNonissues,
    ProfileDcacheMisses,
    ProfileIcacheMisses,
    ProfileCacheMisses,
    ProfileBranchMispredictions,
    ProfileStoreInstructions,
    ProfileFpInstructions,
    ProfileIntegerInstructions,
    Profile2Issue,
    Profile3Issue,
    Profile4Issue,
    ProfileSpecialInstructions,
    ProfileTotalCycles,
    ProfileIcacheIssues,
    ProfileDcacheAccesses,
    ProfileMemoryBarrierCycles,
    ProfileLoadLinkedIssues,
    ProfileMaximum
} KPROFILE_SOURCE;


IMP_VOID RtlInitAnsiString
(
    IN OUT PANSI_STRING  DestinationString,
    IN PUCHAR SourceString
);

IMP_VOID RtlInitUnicodeString
(
    IN OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString
);

IMP_VOID RtlCreateUnicodeStringFromAsciiz
(
    OUT PUNICODE_STRING DestinationString,  
    IN PUCHAR SourceString 
);

IMP_VOID RtlFreeUnicodeString
(
    IN PUNICODE_STRING UnicodeString
);

IMP_VOID RtlFreeAnsiString
(
    IN PANSI_STRING AnsiString
);

IMP_SYSCALL LdrLoadDll
(
    IN PWSTR DllPath OPTIONAL,
    IN PULONG DllCharacteristics OPTIONAL,
    IN PUNICODE_STRING DllName,
    OUT PVOID *DllHandle
);

IMP_SYSCALL LdrUnloadDll
(
    IN PVOID DllHandle
);

IMP_SYSCALL LdrGetProcedureAddress
(
    IN PVOID DllHandle,
    IN PANSI_STRING ProcedureName OPTIONAL,
    IN ULONG ProcedureNumber OPTIONAL,
    OUT PVOID *ProcedureAddress
);

/*
IMP_PVOID NtCurrentTeb
(
    VOID
);
*/
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

IMP_SYSCALL NtProtectVirtualMemory
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG ProtectSize,
	IN ULONG NewProtect,
	OUT PULONG OldProtect
);

IMP_SYSCALL NtWriteVirtualMemory
(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress,
    IN PVOID Buffer,
    IN ULONG BufferLength,
    OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtQuerySystemInformation
(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtOpenProcess
(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID ClientId OPTIONAL
);

IMP_SYSCALL NtCreateThread
(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ProcessHandle,
	OUT PCLIENT_ID ClientId,
	IN PCONTEXT ThreadContext,
	IN PUSER_STACK UserStack,
	IN BOOLEAN CreateSuspended
);

IMP_SYSCALL NtResumeThread
(
	IN HANDLE ThreadHandle,
	OUT PULONG PreviousSuspendCount OPTIONAL
);

IMP_SYSCALL NtRequestWaitReplyPort
(
    IN HANDLE CsrPortHandle,
    IN PCSR_MESSAGE RequestMessage,
    OUT PCSR_MESSAGE ReplyMessage
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

IMP_SYSCALL NtQueryIntervalProfile
(
    IN KPROFILE_SOURCE Source,
    OUT PULONG Interval
);

IMP_SYSCALL NtQueryObject
(
    IN HANDLE ObjectHandle,
    IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
    OUT PVOID ObjectInformation,
    IN ULONG ObjectInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtDuplicateObject
(
    IN HANDLE SourceProcessHandle,
    IN HANDLE SourceHandle,
    IN HANDLE TargetProcessHandle,
    OUT PHANDLE TargetHandle OPTIONAL,
    IN ACCESS_MASK DesiredAccess,
    IN ULONG Attributes,
    IN ULONG Options
);

IMP_SYSCALL NtClose
(
    IN HANDLE Handle
);