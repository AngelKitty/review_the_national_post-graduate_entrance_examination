#ifndef _HVM_EPT_H_
#define _HVM_EPT_H_

/** Intel-defined EPT memory types */
enum EPT_MEMORY_TYPE_E
{
    EPT_MEMORY_TYPE_UC = 0,
    EPT_MEMORY_TYPE_WC = 1,
    EPT_MEMORY_TYPE_WT = 4,
    EPT_MEMORY_TYPE_WP = 5,
    EPT_MEMORY_TYPE_WB = 6,
} EPT_MEMORY_TYPE;


/** Maximum addressing width for the processor */
#define PHYSICAL_ADDRESS_WIDTH 36
/** Guest VPID value (must be non-zero) */
#define VM_VPID 1

/** Number of pages to pre-allocate for later use */
#define NUM_PAGES_ALLOC 1024

// Defines for parsing the EPT violation exit qualification
/** Bitmask for data read violation */
#define EPT_MASK_DATA_READ 0x1
/** Bitmask for data write violation */
#define EPT_MASK_DATA_WRITE (1 << 1)
/** Bitmask for data execute violation */
#define EPT_MASK_DATA_EXEC (1 << 2)
/** Bitmask for if the guest linear address is valid */
#define EPT_MASK_GUEST_LINEAR_VALID (1 << 7)


void MapOutEntry(
    void *ptr
);

void
UnlockProcessMemory(
    PEPROCESS Proc,
    PKAPC_STATE ApcState,
    PMDLX mdl );

void InitMappingOperations(
    PAGE_CONTEXT *Context,
    UINT numPages );

void EndMappingOperations(
    PAGE_CONTEXT *Context
);

EPT_PML4_ENTRY *
InitEptIdentityMap(
    PEPT_INFO EptInfo
);

void
FreeEptIdentityMap(
    PEPT_INFO EptInfo,
    EPT_PML4_ENTRY * ptr
);

void Handle_Exec_Ept(
    PEPT_INFO EptInfo,
    PGUEST_REGS GuestReg
);


void Init_InvalidPage(
    PEPT_INFO EptInfo,
    PTLB_TRANSLATION arrPtr,
    ULONG ImageSize,
	ULONG OpcodeAddr
);

void End_InvalidPage(
    PEPT_INFO EptInfo,
    PTLB_TRANSLATION arrPtr,
    ULONG ImageSize
);


VOID freeTranslation(
    ULONG ImageSize,
    PTLB_TRANSLATION arr,
    PHYSICAL_ADDRESS *targetPhys,
    PPAGE_TABLE_ENTRY* targetPtes );

void MapOutImageHeader(
    BYTE *peBaseAddr
);

PTLB_TRANSLATION
InitTranslation(
    BYTE *codePtr,
    UINT len,
    PEPROCESS proc,
    PKAPC_STATE apc,
    PSPLIT_PROCESS_INFO SplitInfo
);
#endif
