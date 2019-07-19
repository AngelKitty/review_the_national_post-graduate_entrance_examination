#ifndef _MORE_STRUCTS_H_
#define _MORE_STRUCTS_H_

#pragma pack(push, 1)

typedef struct EptTablePointer
{
    INT64 MemoryType :3;	 // EPT Paging structure memory type (0 for UC)
    INT64 PageWalkLength :3; // Page-walk length - 1
    INT64 reserved1 :6;		 // Reserved
    INT64 PhysAddr :24;		 // Physical address of the EPT PML4 table
    INT64 reserved2 :28;
} EPT_TABLE_POINTER, *PEPT_TABLE_POINTER;

typedef union EptPointer
{
    INT64 unsignedVal;
    EPT_TABLE_POINTER Bits;
} EPTP;


typedef struct EptPml4Entry
{
    INT64 Present :1;		// If the 512 GB region is present (read access)
    INT64 Write :1;			// If the 512 GB region is writable
    INT64 Execute :1;		// If the 512 GB region is executable
    INT64 reserved1 :9;		// Reserved
    INT64 PhysAddr :24;		// Physical address
    INT64 reserved2 :28;	// Reserved
} EPT_PML4_ENTRY, *PEPT_PML4_ENTRY;

typedef struct EptPdpteEntry1Gb
{
    INT64 Present :1;		// If the 1 GB region is present (read access)
    INT64 Write :1;			// If the 1 GB region is writable
    INT64 Execute :1;		// If the 1 GB region is executable
    INT64 MemoryType :3;	// EPT Memory type
    INT64 IgnorePat :1;		// Flag for whether to ignore PAT
    INT64 Size :1;			// Must be 1
    INT64 reserved1 :22;	// Reserved
    INT64 PhysAddr :6;		// Physical address
    INT64 reserved2 :28;	// Reserved
} EPT_PDPTE_ENTRY_1G, *PEPT_PDPTE_ENTRY_1G;


typedef struct EptPdpteEntry
{
    INT64 Present :1;	// If the 1 GB region is present (read access)
    INT64 Write :1;		// If the 1 GB region is writable
    INT64 Execute :1;	// If the 1 GB region is executable
    INT64 reserved1 :9; // Reserved
    INT64 PhysAddr :24; // Physical address
    INT64 reserved2 :28;// Reserved

} EPT_PDPTE_ENTRY, *PEPT_PDPTE_ENTRY;


typedef struct EptPdeEntry
{
    INT64 Present :1;	// If the 2 MB region is present (read access)
    INT64 Write :1;		// If the 2 MB region is writable
    INT64 Execute :1;	// If the 2 MB region is executable
    INT64 reserved1 :9; // Reserved
    INT64 PhysAddr :24; // Physical address
    INT64 reserved2 :28;// Reserved
} EPT_PDE_ENTRY, *PEPT_PDE_ENTRY;

typedef struct EptPdeEntry2Mb
{
    UINT64 Present:1;		// If the 1 GB region is present (read access)
    UINT64 Write:1;			// If the 1 GB region is writable
    UINT64 Execute:1;		// If the 1 GB region is executable
    UINT64 MemoryType:3;	// EPT Memory type
    UINT64 IgnorePat:1;		// Flag for whether to ignore PAT
    UINT64 Size:1;			// Must be 1
    UINT64 reserved1:13;	// Reserved
    UINT64 PhysAddr:15;		// Physical address
    UINT64 reserved2:28;	// Reserved
} EPT_PDE_ENTRY_2M, *PEPT_PDE_ENTRY_2M;


typedef struct EptPteEntry
{
    INT64 Present :1;	// If the 1 GB region is present (read access)
    INT64 Write :1;		// If the 1 GB region is writable
    INT64 Execute :1;	// If the 1 GB region is executable
    INT64 MemoryType :3;// EPT Memory type
    INT64 IgnorePat :1;	// Flag for whether to ignore PAT
    INT64 reserved1 :5;	// Reserved
    INT64 PhysAddr :24;	// Physical address
    INT64 reserved2 :28;// Reserved
} EPT_PTE_ENTRY, *PEPT_PTE_ENTRY;


#define DATA_EPT 0x1
#define CODE_EPT 0x2

#define VMCALL_INIT_INVALID_PAGE	0x100F
#define VMCALL_END_INVALID_PAGE		0x200F
#define VMCALL_GET_ESP				0x300F

/** Number of EPT PDE pages to allocate, one per GB of memory */
#define NUM_PD_PAGES 4
/** Maximum number of EPT page tables */
#define NUM_TABLES 512

//
// Defines a structure to store the data and code page translations
//
typedef struct Tlb_Translation
{
    UINT VirtualAddress;
    UINT PhysAddr;
    PEPT_PTE_ENTRY EptPte;
} TLB_TRANSLATION, *PTLB_TRANSLATION;

typedef struct Hook_Tlb_Translation
{
	UINT GuestPhysAddress;
	UINT PhysAddr;
	PEPT_PTE_ENTRY EptPte;
} HOOK_TLB_TRANSLATION, *PHOOK_TLB_TRANSLATION;

/**
    Structure defining the page directory entry
    from Intel Manual 3A - 4.3
*/
typedef struct PageDirectoryEntry
{
    UINT p :1;		// Present
    UINT rw :1;		// Read/Write
    UINT us :1;		// User/Superuser
    UINT pwt :1;	// Page write through
    UINT pcd :1;	// Page level cache disable
    UINT a :1;		// Accessed
    UINT d :1;		// Dirty
    UINT ps :1;		// Large page
    UINT g :1;		// Global
    UINT reserved1 :3; // Must be 0
    UINT pat :1;	// PAT must be 0
    UINT reserved2 :9; // Must be 0
    UINT address :10;	// Address of page.
}PAGE_DIR_ENTRY, *PPAGE_DIR_ENTRY;

/**
    Structure defining the page directory entry
    from Intel Manual 3A - 4.3 (for small pages)
*/
typedef struct PageDirectoryEntrySmallPage_s
{
    UINT p :1;			// Present
    UINT rw :1;			// Read/Write
    UINT us :1;			// User/Superuser
    UINT pwt :1;		// Page write through
    UINT pcd :1;		// Page level cache disable
    UINT a :1;			// Accessed
    UINT ignored :1;	// Ignored
    UINT ps :1;			// Page Size
    UINT reserved1 :4;	// Must be 0
    UINT address :20;	// Address of page.
}PAGE_DIR_ENTRY_SMALL, *PPAGE_DIR_ENTRY_SMALL;


/**
    Structure defining the page table entry
    from Intel Manual 3A
*/
typedef struct PageTableEntry
{
    UINT p :1;		// Present
    UINT rw :1;		// Read/Write
    UINT us :1;		// User/Superuser
    UINT pwt :1;	// Page write through
    UINT pcd :1;	// Page level cache disable
    UINT a :1;		// Accessed
    UINT d :1;		// Dirty
    UINT pat :1;	// PAT must be 0
    UINT g :1;		// G must be 0
    UINT reserved1 :3; // Must be 0
    UINT address :20;// Address of page.

} PAGE_TABLE_ENTRY, *PPAGE_TABLE_ENTRY;


/**
    Structure storing the needed information to complete a piecemeal walk of the
    page-tables
*/
typedef struct PageWalkContext
{
    PHYSICAL_ADDRESS targetAddress;
    PAGE_DIR_ENTRY *Pde;
    UINT PdeOff;
    PPAGE_TABLE_ENTRY Pte;
    UINT PteOff;
} PAGE_WALK_CONTEXT, *PPAGE_WALK_CONTEXT;

typedef struct PagingContext
{
	PPAGE_TABLE_ENTRY *PageTable;
	UINT VirtualPrefix;
	BYTE *PageArray;
	UINT NumPages;
	BYTE *PageArrayBitmap;
	UINT CR3Val;
}PAGE_CONTEXT, *PPAGE_CONTEXT;

typedef struct _PSPLIT_PROCESS_INFO
{
	PVOID ModuleBase;
	PVOID KernelModulePtr;

	BYTE* CodePtr;

	ULONG ImageSize;

	PHYSICAL_ADDRESS BasePhysicalAddress;
	ULONG cr3;

	PTLB_TRANSLATION Translation;
	PHYSICAL_ADDRESS *targetPhys;
	PPAGE_TABLE_ENTRY* targetPtes;

	PMDLX LockedMdl;

	BYTE* Opcode;

}SPLIT_PROCESS_INFO, *PSPLIT_PROCESS_INFO;

typedef struct _EPT_INFO
{
	// EPT指针
	EPT_PML4_ENTRY *EptPml4TablePointer;

	EPT_PDPTE_ENTRY *BkupPdptePtr;

	EPT_PDE_ENTRY_2M* BkupPdePtrs[NUM_PD_PAGES];

	UINT EptPageTableCounter;

	UINT TableVirtsCounter;

	PEPT_PTE_ENTRY EptTableArray[NUM_TABLES];
	
	PEPT_PTE_ENTRY EptTableVirts[NUM_TABLES];

	PTLB_TRANSLATION SplitPages;

	ULONG Violation;
	ULONG Execute;
	ULONG Data;

	ULONG ImageSize;

	PEPROCESS Eprocess;

}EPT_INFO, *PEPT_INFO;

typedef struct _CPU_VM_CONTEXT
{
	PVOID Self;

	//
	// VMCS 结构体的物理地址和虚拟地址
	//
	PHYSICAL_ADDRESS VmcsPyhicalAddress;
	PVOID VmcsVirtualAddress;

	//
	// VMON 物理地址和虚拟地址
	//
	PHYSICAL_ADDRESS VmxonPyhicalAddress;
	PVOID VmxonVirtualAddress;

	//
	//  Host Stack
	//
	PVOID HostEsp;

	//
	//  IOBitmapA, IOBitmapB. 地址
	//
	PHYSICAL_ADDRESS IOBitmapPyhicalAddressA;
	PVOID IOBitmapVirtualAddressA;
	PHYSICAL_ADDRESS IOBitmapPyhicalAddressB;
	PVOID IOBitmapVirtualAddressB;

	//
	//  异常位图
	//
	ULONG32 ExceptBitmap;

	//
	//  MSR 位图的虚拟地址和物理地址
	//
	PHYSICAL_ADDRESS MSRBitmapPyhicalAddress;
	PVOID MSRBitmapVirtualAddress;

	PEPT_INFO EptInfo;

} CPU_VM_CONTEXT, *P_CPU_VM_CONTEXT;

typedef struct _GUEST_REGS
{
	ULONG RegEax;
	ULONG RegEcx;
	ULONG RegEdx;
	ULONG RegEbx;
	ULONG RegEsp;
	ULONG RegEbp;
	ULONG RegEsi;
	ULONG RegEdi;

} GUEST_REGS, *PGUEST_REGS;

#pragma pack(pop)

NTKERNELAPI
UCHAR *
PsGetProcessImageFileName(
    __in PEPROCESS Process
);


PVOID
NTAPI
PsGetProcessSectionBaseAddress(
    PEPROCESS Process
);


#endif
