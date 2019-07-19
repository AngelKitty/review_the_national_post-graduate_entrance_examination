#ifndef __RSNTGDI_POC__
#define __RSNTGDI_POC__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define IMP_VOID __declspec(dllimport) VOID __stdcall
#define IMP_SYSCALL __declspec(dllimport) NTSTATUS __stdcall

#define PAGE_SIZE 0x1000

#define OBJ_CASE_INSENSITIVE 0x00000040
#define FILE_OPEN_IF 0x00000003

#define NtCurrentProcess() ((HANDLE)0xFFFFFFFF)

#define KERNEL_NAME_LENGTH 0x0D

#define STATUS_SUCCESS 0x00000000
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

typedef ULONG NTSTATUS;

typedef struct _ANSI_STRING
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

typedef struct _IO_STATUS_BLOCK
{ 
    union
    { 
        /* 0x00 */ NTSTATUS Status; 
        /* 0x00 */ PVOID Pointer; 
    }; 

    /* 0x04 */ ULONG Information;
    /* 0x08 */
}
    IO_STATUS_BLOCK,
  *PIO_STATUS_BLOCK,
**PPIO_STATUS_BLOCK;

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

typedef VOID (NTAPI *PIO_APC_ROUTINE)
(
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
);

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

IMP_SYSCALL NtQuerySystemInformation
(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

IMP_SYSCALL NtCreateFile
(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);

IMP_SYSCALL NtDeviceIoControlFile
(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
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

IMP_SYSCALL NtClose
(
    IN HANDLE Handle
);

#endif