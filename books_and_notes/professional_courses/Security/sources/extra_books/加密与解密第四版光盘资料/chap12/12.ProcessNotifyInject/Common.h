#include "ntifs.h"
/********************************************************************
	created:	2008/08/08
	lastupd:	2016/03/08
	filename: 	Common.h
	author:		achillis
	purpose:	作为ntifs.h的补充
*********************************************************************/

//类型定义
typedef void* LPVOID;
typedef PVOID* PHANDLE;
typedef ULONG DWORD;
typedef LONG BOOL;
typedef BOOL* PBOOL;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef HANDLE HWND;
typedef HWND HDESK;
typedef unsigned int UINT;
typedef UINT_PTR WPARAM;
typedef ULONG_PTR LPARAM;
typedef BYTE* PBYTE;


#define MAX_PATH          260
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define ZeroMemory RtlZeroMemory

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle; 
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList; 
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA,*PPEB_LDR_DATA; 

//结构声明
typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    union {
        LIST_ENTRY HashLinks;
        struct {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    union {
        struct {
            ULONG TimeDateStamp;
        };
        struct {
            PVOID LoadedImports;
        };
    };
    struct _ACTIVATION_CONTEXT * EntryPointActivationContext;

    PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    ULONG AllocationProtect;
    SIZE_T RegionSize;
    ULONG State;
    ULONG Protect;
    ULONG Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _OBJECT_HEADER_CREATOR_INFO {
    LIST_ENTRY TypeList;
    HANDLE CreatorUniqueProcess;
    USHORT CreatorBackTraceIndex;
    USHORT Reserved;
} OBJECT_HEADER_CREATOR_INFO, *POBJECT_HEADER_CREATOR_INFO;

typedef struct _OBJECT_HEADER_NAME_INFO {
    PVOID Directory;
    UNICODE_STRING Name;
    ULONG QueryReferences;
} OBJECT_HEADER_NAME_INFO, *POBJECT_HEADER_NAME_INFO;

typedef struct _OBJECT_DIRECTORY_ENTRY {
    struct _OBJECT_DIRECTORY_ENTRY *ChainLink;
    PVOID Object;
    ULONG HashValue;
} OBJECT_DIRECTORY_ENTRY, *POBJECT_DIRECTORY_ENTRY;

#define  SYSTEM_MODE_IMAGE 0x100

typedef struct _SERVICE_DESCRIPTOR_TABLE_SHADOW
{
	SERVICE_DESCRIPTOR_TABLE SSDT;  // ntoskrnl.exe ( native api )
	SERVICE_DESCRIPTOR_TABLE SSDTShadow;    // win32k.sys (gdi/user support)
	SERVICE_DESCRIPTOR_TABLE UnUsed1;
	SERVICE_DESCRIPTOR_TABLE UnUsed2;
}SERVICE_DESCRIPTOR_TABLE_SHADOW, *PSERVICE_DESCRIPTOR_TABLE_SHADOW;

typedef struct
{
	UNICODE_STRING	SectionFileName;
	WCHAR			NameBuffer[ANYSIZE_ARRAY];
} MEMORY_SECTION_NAME,*PMEMORY_SECTION_NAME;

typedef enum _MEMORY_INFORMATION_CLASS 
{
	MemoryBasicInformation,
	MemoryWorkingSetList,
	MemorySectionName
}MEMORY_INFORMATION_CLASS;

typedef struct _CONTROL_AREA {
	//CONTROL_AREA Strutct for winxp
    PVOID Segment; //PSEGMENT
    LIST_ENTRY DereferenceList;
    ULONG NumberOfSectionReferences;    // All section refs & image flushes
    ULONG NumberOfPfnReferences;        // valid + transition prototype PTEs
    ULONG NumberOfMappedViews;          // total # mapped views, including
	// system cache & system space views
    USHORT NumberOfSubsections;     // system cache views only
	USHORT FlushInProgressCount;
    ULONG NumberOfUserReferences;       // user section & view references
    ULONG LongFlags;
    PFILE_OBJECT FilePointer;
    PVOID WaitingForDeletion; //PEVENT_COUNTER
    USHORT ModifiedWriteCount;
    USHORT NumberOfSystemCacheViews;
} CONTROL_AREA, *PCONTROL_AREA;

//函数声明
ULONG g_uCr0;

extern POBJECT_TYPE NTSYSAPI PsProcessType;
extern POBJECT_TYPE NTSYSAPI PsThreadType;

#ifndef _AMD64_
__inline void WPOFF()
{
	
    ULONG uAttr;
	
    _asm
    {
        cli
		push eax
        mov eax, cr0
        mov g_uCr0, eax
        and eax, 0FFFEFFFFh // CR0 16 BIT = 0
        mov cr0, eax
        pop eax
        
    };
	
}

__inline void WPON()
{
	
    _asm
    {
        
		push eax
        mov eax, g_uCr0 //恢復原有 CR0 屬性
        mov cr0, eax
        pop eax
		sti
    };
	
}

#endif
//一些函数的声明

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile (
			HANDLE FileHandle,
			HANDLE Event,
			PIO_APC_ROUTINE ApcRoutine,
			PVOID ApcContext,
			PIO_STATUS_BLOCK IoStatusBlock,
			ULONG IoControlCode,
			PVOID InputBuffer,
			ULONG InputBufferLength,
			PVOID OutputBuffer,
			ULONG OutputBufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
		HANDLE FileHandle,
		HANDLE Event,
		PIO_APC_ROUTINE ApcRoutine,
		PVOID ApcContext,
		PIO_STATUS_BLOCK IoStatusBlock,
		ULONG IoControlCode,
		PVOID InputBuffer,
		ULONG InputBufferLength,
		PVOID OutputBuffer,
		ULONG OutputBufferLength
    );

//一些函数的声明
NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile (
	IN HANDLE FileHandle,
	IN HANDLE Event,
	IN PIO_APC_ROUTINE ApcRoutine,
	IN PVOID ApcContext,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset,
	IN PULONG Key
    );

NTSYSAPI
NTSTATUS
NTAPI
ObOpenObjectByName (
	POBJECT_ATTRIBUTES ObjectAttributes,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PACCESS_STATE AccessState,
	ACCESS_MASK DesiredAccess,
	PVOID ParseContext,
	PHANDLE Handle
    );

NTKERNELAPI
NTSTATUS
NTAPI
ObReferenceObjectByName (
  IN PUNICODE_STRING ObjectName,
  IN ULONG Attributes,
  IN PACCESS_STATE PassedAccessState OPTIONAL,
  IN ACCESS_MASK DesiredAccess OPTIONAL,
  IN POBJECT_TYPE ObjectType OPTIONAL,
  IN KPROCESSOR_MODE AccessMode,
  IN OUT PVOID ParseContext OPTIONAL,
  OUT PVOID *Object
  );
						    
NTSYSAPI
UCHAR *
NTAPI
PsGetProcessImageFileName(
		PEPROCESS Process
    );
    
NTSYSAPI
HANDLE
NTAPI
PsGetThreadId(
    PETHREAD Thread
     );

NTSYSAPI
NTSTATUS
NTAPI
PsGetContextThread(
	IN PETHREAD Thread,
	OUT PCONTEXT ThreadContext,
	IN KPROCESSOR_MODE Mode
    );

NTSYSAPI
NTSTATUS
NTAPI
PsSetContextThread(
	IN PETHREAD Thread,
	IN PCONTEXT ThreadContext,
	IN KPROCESSOR_MODE Mode
    );

NTSYSAPI
PPEB
NTAPI
PsGetProcessPeb(
	IN PEPROCESS Process
    );

NTKERNELAPI
VOID
KeAttachProcess (
    IN PEPROCESS Process
);

NTKERNELAPI
VOID
KeDetachProcess (
    VOID
);

NTKERNELAPI
NTSTATUS
KeUserModeCallback(
	IN ULONG ApiNumber,
	IN PVOID InputBuffer,
	IN ULONG InputLength,
	OUT PVOID *OutputBuffer,
	IN PULONG OutputLength
    );

NTSYSAPI   
BOOLEAN
NTAPI
KeAddSystemServiceTable (
	IN PULONG_PTR Base,
	IN PULONG Count OPTIONAL,
	IN ULONG Limit,
	IN PUCHAR Number,
	IN ULONG Index
    );


//////////////////////////////////////////////////////////////////////////
//以下为自定义函数的声明
#define DO_COMMON_INIT (1)

typedef struct _MAP_IMAGE_INFO 
{
	HANDLE hFile;
	HANDLE hMap;
	PVOID MappedBase;
}MAP_IMAGE_INFO,*PMAP_IMAGE_INFO;

typedef NTSTATUS
(NTAPI *PFN_ZwQueryVirtualMemory)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
	OUT PVOID MemoryInformation,
	IN SIZE_T MemoryInformationLength,
	OUT PSIZE_T ReturnLength
    );

NTSTATUS
MapImageFile(
	IN char *szFilePath ,
	OUT MAP_IMAGE_INFO *pImageInfo
	);

VOID UnMapImageFile(PMAP_IMAGE_INFO pImageInfo);

//通过搜索内存的方式，在内存中查找指定名称的PE文件，在Ldr未初始化时使用
ULONG_PTR FindImageBase(HANDLE hProc , WCHAR *wModuleName);
//通过搜索内存的方式，在内存中查找指定名称的PE文件，在Ldr初始化之后使用
ULONG_PTR KeGetUserModuleHandle(PEPROCESS Process, WCHAR *wModuleName);
//32/64通用
ULONG_PTR KeGetProcAddress(ULONG_PTR hModule,char *FuncName);
//仅用于64BitDriver中
#ifdef _AMD64_
ULONG KeWow64GetProcAddress(ULONG hModule,char *FuncName);
#endif


VOID UnhookSSDTServiceByIndex(ULONG_PTR ServiceIndex,ULONG_PTR OriginalFunAddr );
ULONG HookSSDTServiceByIndex(ULONG_PTR ServiceIndex,ULONG_PTR FakeFunAddr );
DWORD GetServiceIndexByName(ULONG_PTR hModule,char *szServiceName);

//根据给定的服务索引，获取内核中任意Zw函数的地址
ULONG_PTR GetZwServiceAddr(DWORD ServiceIndex);

ULONG_PTR GetZwProtectVirtualMemoryAddr();

NTSTATUS
NTAPI KeGetMappedFileName(
	IN HANDLE hProcess, 
	IN LPVOID lpv, 
	OUT LPWSTR lpFilename, 
	IN DWORD nSize , 
	OUT DWORD *ResultLength
	);

//用于在LoadImageRoutine中使用，修改用户态内存
PMDL MapUserModeAddrWritable(PVOID BaseAddr,ULONG Length, PVOID *pNewAddr);
VOID UnmapMemory(PVOID pAddr, PMDL pMdl);

//判断进程是不是Wow64进程
NTSTATUS NTAPI IsWow64Process(HANDLE hProcess, PBOOL Wow64Process);