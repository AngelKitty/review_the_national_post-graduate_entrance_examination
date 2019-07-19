#include "ntifs.h"
/********************************************************************
	created:	2008/08/08
	lastupd:	2016/03/08
	filename: 	Common.h
	author:		achillis
	purpose:	作为ntifs.h的补充
*********************************************************************/

//类型定义
typedef PVOID LPVOID;
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

//////////////////////////////////////////////////////////////////////////
//PEB And TEB
typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle; 
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList; 
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID        EntryInProgress;                                                                                                                 
} PEB_LDR_DATA,*PPEB_LDR_DATA; 

typedef struct _CURDIR              // 2 elements, 0xC bytes (sizeof) 
{                                                                     
	/*0x000*/     UNICODE_STRING DosPath; // 3 elements, 0x8 bytes (sizeof) 
	/*0x008*/     PVOID        Handle;                                              
}CURDIR, *PCURDIR;    

typedef struct _RTL_DRIVE_LETTER_CURDIR // 4 elements, 0x10 bytes (sizeof) 
{                                                                          
	/*0x000*/     UINT16       Flags;                                                    
	/*0x002*/     UINT16       Length;                                                   
	/*0x004*/     ULONG32      TimeStamp;                                                
	/*0x008*/     struct _STRING DosPath;             // 3 elements, 0x8 bytes (sizeof)  
}RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;  

typedef struct _RTL_USER_PROCESS_PARAMETERS                // 30 elements, 0x298 bytes (sizeof) 
{                                                                                               
	/*0x000*/     ULONG32      MaximumLength;                                                                 
	/*0x004*/     ULONG32      Length;                                                                        
	/*0x008*/     ULONG32      Flags;                                                                         
	/*0x00C*/     ULONG32      DebugFlags;                                                                    
	/*0x010*/     PVOID        ConsoleHandle;                                                                 
	/*0x014*/     ULONG32      ConsoleFlags;                                                                  
	/*0x018*/     PVOID        StandardInput;                                                                 
	/*0x01C*/     PVOID        StandardOutput;                                                                
	/*0x020*/     PVOID        StandardError;                                                                 
	/*0x024*/     CURDIR CurrentDirectory;                       // 2 elements, 0xC bytes (sizeof)    
	/*0x030*/     UNICODE_STRING DllPath;                        // 3 elements, 0x8 bytes (sizeof)    
	/*0x038*/     UNICODE_STRING ImagePathName;                  // 3 elements, 0x8 bytes (sizeof)    
	/*0x040*/     UNICODE_STRING CommandLine;                    // 3 elements, 0x8 bytes (sizeof)    
	/*0x048*/     PVOID        Environment;                                                                   
	/*0x04C*/     ULONG32      StartingX;                                                                     
	/*0x050*/     ULONG32      StartingY;                                                                     
	/*0x054*/     ULONG32      CountX;                                                                        
	/*0x058*/     ULONG32      CountY;                                                                        
	/*0x05C*/     ULONG32      CountCharsX;                                                                   
	/*0x060*/     ULONG32      CountCharsY;                                                                   
	/*0x064*/     ULONG32      FillAttribute;                                                                 
	/*0x068*/     ULONG32      WindowFlags;                                                                   
	/*0x06C*/     ULONG32      ShowWindowFlags;                                                               
	/*0x070*/     UNICODE_STRING WindowTitle;                    // 3 elements, 0x8 bytes (sizeof)    
	/*0x078*/     UNICODE_STRING DesktopInfo;                    // 3 elements, 0x8 bytes (sizeof)    
	/*0x080*/     UNICODE_STRING ShellInfo;                      // 3 elements, 0x8 bytes (sizeof)    
	/*0x088*/     UNICODE_STRING RuntimeData;                    // 3 elements, 0x8 bytes (sizeof)    
	/*0x090*/     RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];                                      
	/*0x290*/     ULONG32      EnvironmentSize;                                                               
	/*0x294*/     ULONG32      EnvironmentVersion;                                                            
}RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;                                     

typedef struct _RTL_CRITICAL_SECTION_DEBUG         // 9 elements, 0x20 bytes (sizeof) 
{                                                                                     
	/*0x000*/     UINT16       Type;                                                                
	/*0x002*/     UINT16       CreatorBackTraceIndex;                                               
	/*0x004*/     struct _RTL_CRITICAL_SECTION* CriticalSection;                                    
	/*0x008*/     LIST_ENTRY   ProcessLocksList;           // 2 elements, 0x8 bytes (sizeof)  
	/*0x010*/     ULONG32      EntryCount;                                                          
	/*0x014*/     ULONG32      ContentionCount;                                                     
	/*0x018*/     ULONG32      Flags;                                                               
	/*0x01C*/     UINT16       CreatorBackTraceIndexHigh;                                           
	/*0x01E*/     UINT16       SpareUSHORT;                                                         
}RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;                                   
                                                                        
typedef struct _RTL_CRITICAL_SECTION               // 6 elements, 0x18 bytes (sizeof) 
{                                                                                     
	/*0x000*/     struct _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;                                    
	/*0x004*/     LONG32       LockCount;                                                           
	/*0x008*/     LONG32       RecursionCount;                                                      
	/*0x00C*/     HANDLE	   OwningThread; //handle type not sure                                                       
	/*0x010*/     PVOID        LockSemaphore;                                                       
	/*0x014*/     ULONG32      SpinCount;                                                           
}RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;      

typedef struct _PEB                                                      // 91 elements, 0x248 bytes (sizeof) 
{                                                                                                             
/*0x000*/     UINT8        InheritedAddressSpace;                                                                       
/*0x001*/     UINT8        ReadImageFileExecOptions;                                                                    
/*0x002*/     UINT8        BeingDebugged;                                                                               
              union                                                                // 2 elements, 0x1 bytes (sizeof)    
              {                                                                                                         
/*0x003*/         UINT8        BitField;                                                                                
                  struct                                                           // 6 elements, 0x1 bytes (sizeof)    
                  {                                                                                                     
/*0x003*/             UINT8        ImageUsesLargePages : 1;                        // 0 BitPosition                     
/*0x003*/             UINT8        IsProtectedProcess : 1;                         // 1 BitPosition                     
/*0x003*/             UINT8        IsLegacyProcess : 1;                            // 2 BitPosition                     
/*0x003*/             UINT8        IsImageDynamicallyRelocated : 1;                // 3 BitPosition                     
/*0x003*/             UINT8        SkipPatchingUser32Forwarders : 1;               // 4 BitPosition                     
/*0x003*/             UINT8        SpareBits : 3;                                  // 5 BitPosition                     
                  };                                                                                                    
              };                                                                                                        
/*0x004*/     PVOID        Mutant;                                                                                      
/*0x008*/     PVOID        ImageBaseAddress;                                                                            
/*0x00C*/     PEB_LDR_DATA* Ldr;                                                                                
/*0x010*/     RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                                                   
/*0x014*/     PVOID        SubSystemData;                                                                               
/*0x018*/     PVOID        ProcessHeap;                                                                                 
/*0x01C*/     RTL_CRITICAL_SECTION* FastPebLock;                                                                
/*0x020*/     PVOID        AtlThunkSListPtr;                                                                            
/*0x024*/     PVOID        IFEOKey;                                                                                     
              union                                                                // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                                         
/*0x028*/         ULONG32      CrossProcessFlags;                                                                       
                  struct                                                           // 6 elements, 0x4 bytes (sizeof)    
                  {                                                                                                     
/*0x028*/             ULONG32      ProcessInJob : 1;                               // 0 BitPosition                     
/*0x028*/             ULONG32      ProcessInitializing : 1;                        // 1 BitPosition                     
/*0x028*/             ULONG32      ProcessUsingVEH : 1;                            // 2 BitPosition                     
/*0x028*/             ULONG32      ProcessUsingVCH : 1;                            // 3 BitPosition                     
/*0x028*/             ULONG32      ProcessUsingFTH : 1;                            // 4 BitPosition                     
/*0x028*/             ULONG32      ReservedBits0 : 27;                             // 5 BitPosition                     
                  };                                                                                                    
              };                                                                                                        
              union                                                                // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                                         
/*0x02C*/         PVOID        KernelCallbackTable;                                                                     
/*0x02C*/         PVOID        UserSharedInfoPtr;                                                                       
              };                                                                                                        
/*0x030*/     ULONG32      SystemReserved[1];                                                                           
/*0x034*/     ULONG32      AtlThunkSListPtr32;                                                                          
/*0x038*/     PVOID        ApiSetMap;                                                                                   
/*0x03C*/     ULONG32      TlsExpansionCounter;                                                                         
/*0x040*/     PVOID        TlsBitmap;                                                                                   
/*0x044*/     ULONG32      TlsBitmapBits[2];                                                                            
/*0x04C*/     PVOID        ReadOnlySharedMemoryBase;                                                                    
/*0x050*/     PVOID        HotpatchInformation;                                                                         
/*0x054*/     PVOID*       ReadOnlyStaticServerData;                                                                    
/*0x058*/     PVOID        AnsiCodePageData;                                                                            
/*0x05C*/     PVOID        OemCodePageData;                                                                             
/*0x060*/     PVOID        UnicodeCaseTableData;                                                                        
/*0x064*/     ULONG32      NumberOfProcessors;                                                                          
/*0x068*/     ULONG32      NtGlobalFlag;                                                                                
/*0x06C*/     UINT8        _PADDING0_[0x4];                                                                             
/*0x070*/     LARGE_INTEGER CriticalSectionTimeout;                         // 4 elements, 0x8 bytes (sizeof)    
/*0x078*/     ULONG32      HeapSegmentReserve;                                                                          
/*0x07C*/     ULONG32      HeapSegmentCommit;                                                                           
/*0x080*/     ULONG32      HeapDeCommitTotalFreeThreshold;                                                              
/*0x084*/     ULONG32      HeapDeCommitFreeBlockThreshold;                                                              
/*0x088*/     ULONG32      NumberOfHeaps;                                                                               
/*0x08C*/     ULONG32      MaximumNumberOfHeaps;                                                                        
/*0x090*/     PVOID*       ProcessHeaps;                                                                                
/*0x094*/     PVOID        GdiSharedHandleTable;                                                                        
/*0x098*/     PVOID        ProcessStarterHelper;                                                                        
/*0x09C*/     ULONG32      GdiDCAttributeList;                                                                          
/*0x0A0*/     RTL_CRITICAL_SECTION* LoaderLock;                                                                 
/*0x0A4*/     ULONG32      OSMajorVersion;                                                                              
/*0x0A8*/     ULONG32      OSMinorVersion;                                                                              
/*0x0AC*/     UINT16       OSBuildNumber;                                                                               
/*0x0AE*/     UINT16       OSCSDVersion;                                                                                
/*0x0B0*/     ULONG32      OSPlatformId;                                                                                
/*0x0B4*/     ULONG32      ImageSubsystem;                                                                              
/*0x0B8*/     ULONG32      ImageSubsystemMajorVersion;                                                                  
/*0x0BC*/     ULONG32      ImageSubsystemMinorVersion;                                                                  
/*0x0C0*/     ULONG32      ActiveProcessAffinityMask;                                                                   
/*0x0C4*/     ULONG32      GdiHandleBuffer[34];                                                                         
/*0x14C*/     PVOID		   PostProcessInitRoutine;                                           
/*0x150*/     PVOID        TlsExpansionBitmap;                                                                          
/*0x154*/     ULONG32      TlsExpansionBitmapBits[32];                                                                  
/*0x1D4*/     ULONG32      SessionId;                                                                                   
/*0x1D8*/     ULARGE_INTEGER AppCompatFlags;                                // 4 elements, 0x8 bytes (sizeof)    
/*0x1E0*/     ULARGE_INTEGER AppCompatFlagsUser;                            // 4 elements, 0x8 bytes (sizeof)    
/*0x1E8*/     PVOID        pShimData;                                                                                   
/*0x1EC*/     PVOID        AppCompatInfo;                                                                               
/*0x1F0*/     UNICODE_STRING CSDVersion;                                   // 3 elements, 0x8 bytes (sizeof)    
/*0x1F8*/     PVOID		   ActivationContextData;                                                   
/*0x1FC*/     PVOID		   ProcessAssemblyStorageMap;                                                  
/*0x200*/     PVOID		   SystemDefaultActivationContextData;                                      
/*0x204*/     PVOID		   SystemAssemblyStorageMap;                                                   
/*0x208*/     ULONG32      MinimumStackCommit;                                                                          
/*0x20C*/     PVOID		   FlsCallback;                                                                   
/*0x210*/     LIST_ENTRY   FlsListHead;                                      // 2 elements, 0x8 bytes (sizeof)    
/*0x218*/     PVOID        FlsBitmap;                                                                                   
/*0x21C*/     ULONG32      FlsBitmapBits[4];                                                                            
/*0x22C*/     ULONG32      FlsHighIndex;                                                                                
/*0x230*/     PVOID        WerRegistrationData;                                                                         
/*0x234*/     PVOID        WerShipAssertPtr;                                                                            
/*0x238*/     PVOID        pContextData;                                                                                
/*0x23C*/     PVOID        pImageHeaderHash;                                                                            
              union                                                                // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                                         
/*0x240*/         ULONG32      TracingFlags;                                                                            
                  struct                                                           // 3 elements, 0x4 bytes (sizeof)    
                  {                                                                                                     
/*0x240*/             ULONG32      HeapTracingEnabled : 1;                         // 0 BitPosition                     
/*0x240*/             ULONG32      CritSecTracingEnabled : 1;                      // 1 BitPosition                     
/*0x240*/             ULONG32      SpareTracingBits : 30;                          // 2 BitPosition                     
                  };                                                                                                    
              };                                                                                                        
}PEB, *PPEB;                                                                                                  

typedef struct _GDI_TEB_BATCH // 3 elements, 0x4E0 bytes (sizeof) 
{                                                                 
	/*0x000*/     ULONG32      Offset;                                          
	/*0x004*/     ULONG32      HDC;                                             
	/*0x008*/     ULONG32      Buffer[310];                                     
}GDI_TEB_BATCH, *PGDI_TEB_BATCH;     

typedef struct _PROCESSOR_NUMBER // 3 elements, 0x4 bytes (sizeof) 
{                                                                  
	/*0x000*/     UINT16       Group;                                            
	/*0x002*/     UINT8        Number;                                           
	/*0x003*/     UINT8        Reserved;                                         
}PROCESSOR_NUMBER, *PPROCESSOR_NUMBER;     

typedef struct _TEB_ACTIVE_FRAME_CONTEXT // 2 elements, 0x8 bytes (sizeof) 
{                                                                          
	/*0x000*/     ULONG32      Flags;                                                    
	/*0x004*/     CHAR*        FrameName;                                                
}TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;   

typedef struct _TEB_ACTIVE_FRAME               // 3 elements, 0xC bytes (sizeof) 
{                                                                                
	/*0x000*/     ULONG32      Flags;                                                          
	/*0x004*/     struct _TEB_ACTIVE_FRAME* Previous;                                          
	/*0x008*/     struct _TEB_ACTIVE_FRAME_CONTEXT* Context;                                   
}TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;   

typedef struct _AC_TEB                                                  // 99 elements, 0xFE4 bytes (sizeof) 
{                                                                                                         
/*0x000*/     struct _NT_TIB NtTib;                                            // 8 elements, 0x1C bytes (sizeof)   
/*0x01C*/     PVOID        EnvironmentPointer;                                                                      
/*0x020*/     CLIENT_ID	   ClientId;                                      // 2 elements, 0x8 bytes (sizeof)    
/*0x028*/     PVOID        ActiveRpcHandle;                                                                         
/*0x02C*/     PVOID        ThreadLocalStoragePointer;                                                               
/*0x030*/     struct _PEB* ProcessEnvironmentBlock;                                                                 
/*0x034*/     ULONG32      LastErrorValue;                                                                          
/*0x038*/     ULONG32      CountOfOwnedCriticalSections;                                                            
/*0x03C*/     PVOID        CsrClientThread;                                                                         
/*0x040*/     PVOID        Win32ThreadInfo;                                                                         
/*0x044*/     ULONG32      User32Reserved[26];                                                                      
/*0x0AC*/     ULONG32      UserReserved[5];                                                                         
/*0x0C0*/     PVOID        WOW32Reserved;                                                                           
/*0x0C4*/     ULONG32      CurrentLocale;                                                                           
/*0x0C8*/     ULONG32      FpSoftwareStatusRegister;                                                                
/*0x0CC*/     PVOID        SystemReserved1[54];                                                                     
/*0x1A4*/     LONG32       ExceptionCode;                                                                           
/*0x1A8*/     PVOID		   ActivationContextStackPointer;                                      
/*0x1AC*/     UINT8        SpareBytes[36];                                                                          
/*0x1D0*/     ULONG32      TxFsContext;                                                                             
/*0x1D4*/     struct _GDI_TEB_BATCH GdiTebBatch;                               // 3 elements, 0x4E0 bytes (sizeof)  
/*0x6B4*/     struct _CLIENT_ID RealClientId;                                  // 2 elements, 0x8 bytes (sizeof)    
/*0x6BC*/     PVOID        GdiCachedProcessHandle;                                                                  
/*0x6C0*/     ULONG32      GdiClientPID;                                                                            
/*0x6C4*/     ULONG32      GdiClientTID;                                                                            
/*0x6C8*/     PVOID        GdiThreadLocalInfo;                                                                      
/*0x6CC*/     ULONG32      Win32ClientInfo[62];                                                                     
/*0x7C4*/     PVOID        glDispatchTable[233];                                                                    
/*0xB68*/     ULONG32      glReserved1[29];                                                                         
/*0xBDC*/     PVOID        glReserved2;                                                                             
/*0xBE0*/     PVOID        glSectionInfo;                                                                           
/*0xBE4*/     PVOID        glSection;                                                                               
/*0xBE8*/     PVOID        glTable;                                                                                 
/*0xBEC*/     PVOID        glCurrentRC;                                                                             
/*0xBF0*/     PVOID        glContext;                                                                               
/*0xBF4*/     ULONG32      LastStatusValue;                                                                         
/*0xBF8*/     UNICODE_STRING StaticUnicodeString;                      // 3 elements, 0x8 bytes (sizeof)    
/*0xC00*/     WCHAR        StaticUnicodeBuffer[261];                                                                
/*0xE0A*/     UINT8        _PADDING0_[0x2];                                                                         
/*0xE0C*/     PVOID        DeallocationStack;                                                                       
/*0xE10*/     PVOID        TlsSlots[64];                                                                            
/*0xF10*/     LIST_ENTRY   TlsLinks;                                     // 2 elements, 0x8 bytes (sizeof)    
/*0xF18*/     PVOID        Vdm;                                                                                     
/*0xF1C*/     PVOID        ReservedForNtRpc;                                                                        
/*0xF20*/     PVOID        DbgSsReserved[2];                                                                        
/*0xF28*/     ULONG32      HardErrorMode;                                                                           
/*0xF2C*/     PVOID        Instrumentation[9];                                                                      
/*0xF50*/     GUID		   ActivityId;                                         // 4 elements, 0x10 bytes (sizeof)   
/*0xF60*/     PVOID        SubProcessTag;                                                                           
/*0xF64*/     PVOID        EtwLocalData;                                                                            
/*0xF68*/     PVOID        EtwTraceData;                                                                            
/*0xF6C*/     PVOID        WinSockData;                                                                             
/*0xF70*/     ULONG32      GdiBatchCount;                                                                           
              union                                                            // 3 elements, 0x4 bytes (sizeof)    
              {                                                                                                     
/*0xF74*/         struct _PROCESSOR_NUMBER CurrentIdealProcessor;              // 3 elements, 0x4 bytes (sizeof)    
/*0xF74*/         ULONG32      IdealProcessorValue;                                                                 
                  struct                                                       // 4 elements, 0x4 bytes (sizeof)    
                  {                                                                                                 
/*0xF74*/             UINT8        ReservedPad0;                                                                    
/*0xF75*/             UINT8        ReservedPad1;                                                                    
/*0xF76*/             UINT8        ReservedPad2;                                                                    
/*0xF77*/             UINT8        IdealProcessor;                                                                  
                  };                                                                                                
              };                                                                                                    
/*0xF78*/     ULONG32      GuaranteedStackBytes;                                                                    
/*0xF7C*/     PVOID        ReservedForPerf;                                                                         
/*0xF80*/     PVOID        ReservedForOle;                                                                          
/*0xF84*/     ULONG32      WaitingOnLoaderLock;                                                                     
/*0xF88*/     PVOID        SavedPriorityState;                                                                      
/*0xF8C*/     ULONG32      SoftPatchPtr1;                                                                           
/*0xF90*/     PVOID        ThreadPoolData;                                                                          
/*0xF94*/     PVOID*       TlsExpansionSlots;                                                                       
/*0xF98*/     ULONG32      MuiGeneration;                                                                           
/*0xF9C*/     ULONG32      IsImpersonating;                                                                         
/*0xFA0*/     PVOID        NlsCache;                                                                                
/*0xFA4*/     PVOID        pShimData;                                                                               
/*0xFA8*/     ULONG32      HeapVirtualAffinity;                                                                     
/*0xFAC*/     PVOID        CurrentTransactionHandle;                                                                
/*0xFB0*/     struct _TEB_ACTIVE_FRAME* ActiveFrame;                                                                
/*0xFB4*/     PVOID        FlsData;                                                                                 
/*0xFB8*/     PVOID        PreferredLanguages;                                                                      
/*0xFBC*/     PVOID        UserPrefLanguages;                                                                       
/*0xFC0*/     PVOID        MergedPrefLanguages;                                                                     
/*0xFC4*/     ULONG32      MuiImpersonation;                                                                        
              union                                                            // 2 elements, 0x2 bytes (sizeof)    
              {                                                                                                     
/*0xFC8*/         UINT16       CrossTebFlags;                                                                       
/*0xFC8*/         UINT16       SpareCrossTebBits : 16;                         // 0 BitPosition                     
              };                                                                                                    
              union                                                            // 2 elements, 0x2 bytes (sizeof)    
              {                                                                                                     
/*0xFCA*/         UINT16       SameTebFlags;                                                                        
                  struct                                                       // 12 elements, 0x2 bytes (sizeof)   
                  {                                                                                                 
/*0xFCA*/             UINT16       SafeThunkCall : 1;                          // 0 BitPosition                     
/*0xFCA*/             UINT16       InDebugPrint : 1;                           // 1 BitPosition                     
/*0xFCA*/             UINT16       HasFiberData : 1;                           // 2 BitPosition                     
/*0xFCA*/             UINT16       SkipThreadAttach : 1;                       // 3 BitPosition                     
/*0xFCA*/             UINT16       WerInShipAssertCode : 1;                    // 4 BitPosition                     
/*0xFCA*/             UINT16       RanProcessInit : 1;                         // 5 BitPosition                     
/*0xFCA*/             UINT16       ClonedThread : 1;                           // 6 BitPosition                     
/*0xFCA*/             UINT16       SuppressDebugMsg : 1;                       // 7 BitPosition                     
/*0xFCA*/             UINT16       DisableUserStackWalk : 1;                   // 8 BitPosition                     
/*0xFCA*/             UINT16       RtlExceptionAttached : 1;                   // 9 BitPosition                     
/*0xFCA*/             UINT16       InitialThread : 1;                          // 10 BitPosition                    
/*0xFCA*/             UINT16       SpareSameTebBits : 5;                       // 11 BitPosition                    
                  };                                                                                                
              };                                                                                                    
/*0xFCC*/     PVOID        TxnScopeEnterCallback;                                                                   
/*0xFD0*/     PVOID        TxnScopeExitCallback;                                                                    
/*0xFD4*/     PVOID        TxnScopeContext;                                                                         
/*0xFD8*/     ULONG32      LockCount;                                                                               
/*0xFDC*/     ULONG32      SpareUlong0;                                                                             
/*0xFE0*/     PVOID        ResourceRetValue;                                                                        
}AcTEB, *PACTEB;                                                                                              
                                                                                                                    


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

//通过遍历Ldr的方式，在内存中查找指定名称的PE文件，在Ldr初始化之后使用
ULONG_PTR KeGetUserModuleHandle(WCHAR *wModuleName);
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