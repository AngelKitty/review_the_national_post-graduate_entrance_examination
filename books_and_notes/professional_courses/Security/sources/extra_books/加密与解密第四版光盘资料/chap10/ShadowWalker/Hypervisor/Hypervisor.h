#ifndef __HYPERVISOR__H__
#define __HYPERVISOR__H__

#define KEYBOARD_DATA_PORT                  0x60

#define DIVIDE_ERROR_EXCEPTION              0
#define DEBUG_EXCEPTION                     1
#define NMI_INTERRUPT                       2
#define BREAKPOINT_EXCEPTION                3
#define OVERFLOW_EXCEPTION                  4
#define BOUND_EXCEPTION                     5
#define INVALID_OPCODE_EXCEPTION            6
#define DEVICE_NOT_AVAILABLE_EXCEPTION      7
#define DOUBLE_FAULT_EXCEPTION              8
#define COPROCESSOR_SEGMENT_OVERRUN         9
#define INVALID_TSS_EXCEPTION               10
#define SEGMENT_NOT_PRESENT                 11
#define STACK_FAULT_EXCEPTION               12
#define GENERAL_PROTECTION_EXCEPTION        13
#define PAGE_FAULT_EXCEPTION                14
#define X87_FLOATING_POINT_ERROR            16
#define ALIGNMENT_CHECK_EXCEPTION           17
#define MACHINE_CHECK_EXCEPTION1            18
#define SIMD_FLOATING_POINT_EXCEPTION       19

#define EXTERNAL_INTERRUPT 0
#define HARDWARE_EXCEPTION 3
#define SOFTWARE_INTERRUPT 4
#define PRIVILEGED_SOFTWARE_EXCEPTION 5
#define SOFTWARE_EXCEPTION 6
#define OTHER_EVENT 7

//
// Intel CPU features in CR4
//

#define X86_CR4_VME			0x0001  /* enable vm86 extensions */
#define X86_CR4_PVI			0x0002  /* virtual interrupts flag enable */
#define X86_CR4_TSD			0x0004  /* disable time stamp at ipl 3 */
#define X86_CR4_DE			0x0008  /* enable debugging extensions */
#define X86_CR4_PSE			0x0010  /* enable page size extensions */
#define X86_CR4_PAE			0x0020  /* enable physical address extensions */
#define X86_CR4_MCE			0x0040  /* Machine check enable */
#define X86_CR4_PGE			0x0080  /* enable global pages */
#define X86_CR4_PCE			0x0100  /* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR		0x0200  /* enable fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT	0x0400  /* enable unmasked SSE exceptions */
#define X86_CR4_VMXE		0x2000  /* enable VMX */

//
// intel CPU  MSR
// MSRs & bits used for VMX enabling
//
#define MSR_IA32_VMX_BASIC   			0x480
#define MSR_IA32_FEATURE_CONTROL 		0x03a
#define MSR_IA32_VMX_PINBASED_CTLS		0x481
#define MSR_IA32_VMX_PROCBASED_CTLS		0x482
#define MSR_IA32_VMX_EXIT_CTLS			0x483
#define MSR_IA32_VMX_ENTRY_CTLS			0x484
#define MSR_IA32_VMX_PROCBASED_CTLS2    0x48B
#define IA32_VMX_EPT_VPID_CAP           0x48C
#define MSR_IA32_SYSENTER_CS			0x174
#define MSR_IA32_SYSENTER_ESP			0x175
#define MSR_IA32_SYSENTER_EIP			0x176
#define MSR_IA32_DEBUGCTL				0x1d9



enum
{
	VIRTUAL_PROCESSOR_ID= 0x00000000,
    GUEST_ES_SELECTOR = 0x00000800,
    GUEST_CS_SELECTOR = 0x00000802,
    GUEST_SS_SELECTOR = 0x00000804,
    GUEST_DS_SELECTOR = 0x00000806,
    GUEST_FS_SELECTOR = 0x00000808,
    GUEST_GS_SELECTOR = 0x0000080a,
    GUEST_LDTR_SELECTOR = 0x0000080c,
    GUEST_TR_SELECTOR = 0x0000080e,
    HOST_ES_SELECTOR = 0x00000c00,
    HOST_CS_SELECTOR = 0x00000c02,
    HOST_SS_SELECTOR = 0x00000c04,
    HOST_DS_SELECTOR = 0x00000c06,
    HOST_FS_SELECTOR = 0x00000c08,
    HOST_GS_SELECTOR = 0x00000c0a,
    HOST_TR_SELECTOR = 0x00000c0c,
    IO_BITMAP_A = 0x00002000,
    IO_BITMAP_A_HIGH = 0x00002001,
    IO_BITMAP_B = 0x00002002,
    IO_BITMAP_B_HIGH = 0x00002003,
    MSR_BITMAP = 0x00002004,
    MSR_BITMAP_HIGH = 0x00002005,
    VM_EXIT_MSR_STORE_ADDR = 0x00002006,
    VM_EXIT_MSR_STORE_ADDR_HIGH = 0x00002007,
    VM_EXIT_MSR_LOAD_ADDR = 0x00002008,
    VM_EXIT_MSR_LOAD_ADDR_HIGH = 0x00002009,
    VM_ENTRY_MSR_LOAD_ADDR = 0x0000200a,
    VM_ENTRY_MSR_LOAD_ADDR_HIGH = 0x0000200b,
    TSC_OFFSET = 0x00002010,
    TSC_OFFSET_HIGH = 0x00002011,
    VIRTUAL_APIC_PAGE_ADDR = 0x00002012,
    VIRTUAL_APIC_PAGE_ADDR_HIGH = 0x00002013,
	APIC_ACCESS_ADDR		= 0x00002014,
	APIC_ACCESS_ADDR_HIGH		= 0x00002015,
	EPT_POINTER			= 0x0000201a,
	EPT_POINTER_HIGH		= 0x0000201b,
	GUEST_PHYSICAL_ADDRESS		= 0x00002400,
	GUEST_PHYSICAL_ADDRESS_HIGH	= 0x00002401,
    VMCS_LINK_POINTER = 0x00002800,
    VMCS_LINK_POINTER_HIGH = 0x00002801,
    GUEST_IA32_DEBUGCTL = 0x00002802,
    GUEST_IA32_DEBUGCTL_HIGH = 0x00002803,
    PIN_BASED_VM_EXEC_CONTROL = 0x00004000,
    CPU_BASED_VM_EXEC_CONTROL = 0x00004002,
    EXCEPTION_BITMAP = 0x00004004,
    PAGE_FAULT_ERROR_CODE_MASK = 0x00004006,
    PAGE_FAULT_ERROR_CODE_MATCH = 0x00004008,
    CR3_TARGET_COUNT = 0x0000400a,
    VM_EXIT_CONTROLS = 0x0000400c,
    VM_EXIT_MSR_STORE_COUNT = 0x0000400e,
    VM_EXIT_MSR_LOAD_COUNT = 0x00004010,
    VM_ENTRY_CONTROLS = 0x00004012,
    VM_ENTRY_MSR_LOAD_COUNT = 0x00004014,
    VM_ENTRY_INTR_INFO_FIELD = 0x00004016,
    VM_ENTRY_EXCEPTION_ERROR_CODE = 0x00004018,
    VM_ENTRY_INSTRUCTION_LEN = 0x0000401a,
    TPR_THRESHOLD = 0x0000401c,
    SECONDARY_VM_EXEC_CONTROL = 0x0000401e,
    VM_INSTRUCTION_ERROR = 0x00004400,
    VM_EXIT_REASON = 0x00004402,
    VM_EXIT_INTR_INFO = 0x00004404,
    VM_EXIT_INTR_ERROR_CODE = 0x00004406,
    IDT_VECTORING_INFO_FIELD = 0x00004408,
    IDT_VECTORING_ERROR_CODE = 0x0000440a,
    VM_EXIT_INSTRUCTION_LEN = 0x0000440c,
    VMX_INSTRUCTION_INFO = 0x0000440e,
    
	GUEST_ES_LIMIT = 0x00004800,
    GUEST_CS_LIMIT = 0x00004802,
    GUEST_SS_LIMIT = 0x00004804,
    GUEST_DS_LIMIT = 0x00004806,
    GUEST_FS_LIMIT = 0x00004808,
    GUEST_GS_LIMIT = 0x0000480a,
    GUEST_LDTR_LIMIT = 0x0000480c,
    GUEST_TR_LIMIT = 0x0000480e,
    GUEST_GDTR_LIMIT = 0x00004810,
    GUEST_IDTR_LIMIT = 0x00004812,
    GUEST_ES_AR_BYTES = 0x00004814,
    GUEST_CS_AR_BYTES = 0x00004816,
    GUEST_SS_AR_BYTES = 0x00004818,
    GUEST_DS_AR_BYTES = 0x0000481a,
    GUEST_FS_AR_BYTES = 0x0000481c,
    GUEST_GS_AR_BYTES = 0x0000481e,
    GUEST_LDTR_AR_BYTES = 0x00004820,
    GUEST_TR_AR_BYTES = 0x00004822,
    GUEST_INTERRUPTIBILITY_INFO = 0x00004824,
    GUEST_ACTIVITY_STATE = 0x00004826,
    GUEST_SM_BASE = 0x00004828,
    GUEST_SYSENTER_CS = 0x0000482A,

    HOST_IA32_SYSENTER_CS = 0x00004c00,
    CR0_GUEST_HOST_MASK = 0x00006000,
    CR4_GUEST_HOST_MASK = 0x00006002,
    CR0_READ_SHADOW = 0x00006004,
    CR4_READ_SHADOW = 0x00006006,
    CR3_TARGET_VALUE0 = 0x00006008,
    CR3_TARGET_VALUE1 = 0x0000600a,
    CR3_TARGET_VALUE2 = 0x0000600c,
    CR3_TARGET_VALUE3 = 0x0000600e,
    EXIT_QUALIFICATION = 0x00006400,
    GUEST_LINEAR_ADDRESS = 0x0000640a,
    GUEST_CR0 = 0x00006800,
    GUEST_CR3 = 0x00006802,
    GUEST_CR4 = 0x00006804,
    GUEST_ES_BASE = 0x00006806,
    GUEST_CS_BASE = 0x00006808,
    GUEST_SS_BASE = 0x0000680a,
    GUEST_DS_BASE = 0x0000680c,
    GUEST_FS_BASE = 0x0000680e,
    GUEST_GS_BASE = 0x00006810,
    GUEST_LDTR_BASE = 0x00006812,
    GUEST_TR_BASE = 0x00006814,
    GUEST_GDTR_BASE = 0x00006816,
    GUEST_IDTR_BASE = 0x00006818,
    GUEST_DR7 = 0x0000681a,
    GUEST_ESP = 0x0000681c,
    GUEST_EIP = 0x0000681e,
    GUEST_RFLAGS = 0x00006820,
    GUEST_PENDING_DBG_EXCEPTIONS = 0x00006822,
    GUEST_SYSENTER_ESP = 0x00006824,
    GUEST_SYSENTER_EIP = 0x00006826,
    HOST_CR0 = 0x00006c00,
    HOST_CR3 = 0x00006c02,
    HOST_CR4 = 0x00006c04,
    HOST_FS_BASE = 0x00006c06,
    HOST_GS_BASE = 0x00006c08,
    HOST_TR_BASE = 0x00006c0a,
    HOST_GDTR_BASE = 0x00006c0c,
    HOST_IDTR_BASE = 0x00006c0e,
    HOST_IA32_SYSENTER_ESP = 0x00006c10,
    HOST_IA32_SYSENTER_EIP = 0x00006c12,
    HOST_ESP = 0x00006c14,
    HOST_EIP = 0x00006c16,
};


#define CPU_BASED_VIRTUAL_INTR_PENDING          0x00000004
#define CPU_BASED_USE_TSC_OFFSETING             0x00000008
#define CPU_BASED_HLT_EXITING                   0x00000080
#define CPU_BASED_INVLPG_EXITING                0x00000200
#define CPU_BASED_MWAIT_EXITING                 0x00000400
#define CPU_BASED_RDPMC_EXITING                 0x00000800
#define CPU_BASED_RDTSC_EXITING                 0x00001000
#define CPU_BASED_CR3_LOAD_EXITING		        0x00008000
#define CPU_BASED_CR3_STORE_EXITING		       0x00010000
#define CPU_BASED_CR8_LOAD_EXITING              0x00080000
#define CPU_BASED_CR8_STORE_EXITING             0x00100000
#define CPU_BASED_TPR_SHADOW                    0x00200000
#define CPU_BASED_VIRTUAL_NMI_PENDING		     0x00400000
#define CPU_BASED_MOV_DR_EXITING                0x00800000
#define CPU_BASED_UNCOND_IO_EXITING             0x01000000
#define CPU_BASED_USE_IO_BITMAPS                0x02000000
#define CPU_BASED_ACTIVATE_MSR_BITMAP           0x10000000
#define CPU_BASED_MTF_TRAP_EXITING              0x08000000
#define CPU_BASED_USE_MSR_BITMAPS               0x10000000
#define CPU_BASED_MONITOR_EXITING               0x20000000
#define CPU_BASED_PAUSE_EXITING                 0x40000000
#define CPU_BASED_ACTIVATE_SECONDARY_CONTROLS   0x80000000




#define VM_EXIT_IA32E_MODE              0x00000200
#define VM_EXIT_ACK_INTR_ON_EXIT        0x00008000


#define EXIT_REASON_EXCEPTION_NMI       0
#define EXIT_REASON_EXTERNAL_INTERRUPT  1
#define EXIT_REASON_TRIPLE_FAULT        2
#define EXIT_REASON_INIT                3
#define EXIT_REASON_SIPI                4
#define EXIT_REASON_IO_SMI              5
#define EXIT_REASON_OTHER_SMI           6
#define EXIT_REASON_PENDING_INTERRUPT   7
#define EXIT_REASON_TASK_SWITCH         9
#define EXIT_REASON_CPUID               10
#define EXIT_REASON_HLT                 12
#define EXIT_REASON_INVD                13
#define EXIT_REASON_INVLPG              14
#define EXIT_REASON_RDPMC               15
#define EXIT_REASON_RDTSC               16
#define EXIT_REASON_RSM                 17
#define EXIT_REASON_VMCALL              18
#define EXIT_REASON_VMCLEAR             19
#define EXIT_REASON_VMLAUNCH            20
#define EXIT_REASON_VMPTRLD             21
#define EXIT_REASON_VMPTRST             22
#define EXIT_REASON_VMREAD              23
#define EXIT_REASON_VMRESUME            24
#define EXIT_REASON_VMWRITE             25
#define EXIT_REASON_VMXOFF              26
#define EXIT_REASON_VMXON               27
#define EXIT_REASON_CR_ACCESS           28
#define EXIT_REASON_DR_ACCESS           29
#define EXIT_REASON_IO_INSTRUCTION      30
#define EXIT_REASON_MSR_READ            31
#define EXIT_REASON_MSR_WRITE           32
#define EXIT_REASON_INVALID_GUEST_STATE 33
#define EXIT_REASON_MSR_LOADING         34
#define EXIT_REASON_MWAIT_INSTRUCTION   36
#define EXIT_REASON_MONITOR_INSTRUCTION 39
#define EXIT_REASON_PAUSE_INSTRUCTION   40
#define EXIT_REASON_MACHINE_CHECK       41
#define EXIT_REASON_TPR_BELOW_THRESHOLD 43
#define	EXIT_REASON_APIC_ACCESS			44
#define	EXIT_REASON_EPT_VIOLATION		48
#define	EXIT_REASON_EPT_MISCONFIG		49
#define	EXIT_REASON_WBINVD				54
#define	MAX_VM_EXIT_NUMBER				55

#define INTR_INFO_VECTOR_MASK           0xff            /* bits 0:7 */
#define INTR_INFO_INTR_TYPE_MASK        0x700           /* bits 8:10 */
#define INTR_INFO_DELIVER_CODE_MASK     0x800           /* bit 11 must be set to push error code on guest stack*/
#define INTR_INFO_VALID_MASK            0x80000000      /* bit 31 must be set to identify valid events */
#define INTR_TYPE_EXT_INTR              (0 << 8)        /* external interrupt */
#define INTR_TYPE_NMI                   (2 << 8)        /* NMI */
#define INTR_TYPE_HW_EXCEPTION          (3 << 8)        /* hardware exception */
#define INTR_TYPE_SW_EXCEPTION          (6 << 8)        /* software exception */
#define HVM_DELIVER_NO_ERROR_CODE (-1)

#define MSR_TSC						0x10
#define	MSR_EFER					0xc0000080
#define	MSR_FS_BASE					0xc0000100
#define	MSR_GS_BASE					0xc0000101
#define MSR_LSTAR					0xC0000082
#define	MSR_SHADOW_GS_BASE			0xc0000102
#define	MSR_VM_HSAVE_PA				0xC0010117

#define TRAP_DEBUG						1
#define TRAP_INT3						3
#define TRAP_PAGE_FAULT					14
#define TRAP_INVALID_OP					6

//---------------------------------------------------------------------------
#pragma pack (push, 1)

typedef struct _VMX_BASIC_MSR
{
    unsigned RevId: 32;
    unsigned szVmxOnRegion: 12;
    unsigned ClearBit: 1;
    unsigned Reserved: 3;
    unsigned PhysicalWidth: 1;
    unsigned DualMonitor: 1;
    unsigned MemoryType: 4;
    unsigned VmExitInformation: 1;
    unsigned Reserved2: 9;
} VMX_BASIC_MSR, *PVMX_BASIC_MSR;

typedef struct _IA32_VMX_PROCBASED_CTLS2_MSR
{
	unsigned Reserved1:32;	
	unsigned Reserved2:1;     
	unsigned EnableEpt:1;     
	unsigned Reserved3:3;     
	unsigned EnableVpid:1;     
	unsigned Reserved4:26;	

} IA32_VMX_PROCBASED_CTLS2_MSR;

typedef struct _IA32_VMX_EPT_VPID_CAP_MSR
{
	unsigned ExecuteOnly	:1;		// Bit 0 defines if the EPT implementation supports execute-only translation
	unsigned Reserved1		:31;	// Undefined
	unsigned Reserved2		:8;	// Undefined
	unsigned IndividualAddressInvVpid   :1; // Bit 40 defines if type 0 INVVPID instructions are supported
	unsigned Reserved3      :23;

} IA32_VMX_EPT_VPID_CAP_MSR;
typedef struct _IA32_VMX_PROCBASED_CTLS_MSR
{
	unsigned Reserved1:32;	
	unsigned Reserved2:31;	
	unsigned SControls:1;		

} IA32_VMX_PROCBASED_CTLS_MSR;

typedef struct _MOV_CR_QUALIFICATION {
	unsigned ControlRegister:4;
	unsigned AccessType:2;
	unsigned LMSWOperandType:1;
	unsigned Reserved1:1;
	unsigned Register:4;
	unsigned Reserved2:4;
	unsigned LMSWSourceData:16;
} MOV_CR_QUALIFICATION, *PMOV_CR_QUALIFICATION;

typedef struct _MOV_DR_QUALIFICATION {
	unsigned DebugRegIndex:3;
	unsigned Reserved1:1;
	unsigned DirectionAccess:1;
	unsigned Reserved2:3;
	unsigned GeneralReg:4;
	unsigned Reserved3:20;
	unsigned Reserved4:32;
} MOV_DR_QUALIFICATION, *PMOV_DR_QUALIFICATION;

typedef struct _INTERRUPT_INFO_FIELD {
	unsigned Vector:8;
	unsigned InterruptionType:3;
	unsigned ErrorCodeValid:1;
	unsigned NMIUnblocking:1;
	unsigned Reserved:18;
	unsigned Valid:1;
} INTERRUPT_INFO_FIELD, *PINTERRUPT_INFO_FIELD;

typedef struct _INTERRUPT_INJECT_INFO_FIELD{
	unsigned Vector:8;
	unsigned InterruptionType:3;
	unsigned DeliverErrorCode:1;
	unsigned Reserved:19;
	unsigned Valid:1;
} INTERRUPT_INJECT_INFO_FIELD, *PINTERRUPT_INJECT_INFO_FIELD;
//===========================================================================
typedef union
{
    USHORT UCHARs;
    struct
    {
        USHORT type: 4;             /* 0;  Bit 40-43 */
        USHORT s: 1;                /* 4;  Bit 44 */
        USHORT dpl: 2;              /* 5;  Bit 45-46 */
        USHORT p: 1;                /* 7;  Bit 47 */
        // gap!
        USHORT avl: 1;              /* 8;  Bit 52 */
        USHORT l: 1;                /* 9;  Bit 53 */
        USHORT db: 1;               /* 10; Bit 54 */
        USHORT g: 1;                /* 11; Bit 55 */
        USHORT Gap: 4;
    } fields;
} SEGMENT_ATTRIBUTES;

typedef struct _SEGMENT_SELECTOR
{
    USHORT Selector;
    SEGMENT_ATTRIBUTES Attributes;
    ULONG Limit;
    ULONG64 Base;
} SEGMENT_SELECTOR, *PSEGMENT_SELECTOR;

typedef struct
{
    unsigned	LimitLo	: 16;
    unsigned	BaseLo	: 16;
    unsigned	BaseMid	: 8;
    unsigned	Type	: 4;
    unsigned	System	: 1;
    unsigned	DPL	: 2;
    unsigned	Present	: 1;
    unsigned	LimitHi	: 4;
    unsigned	AVL	: 1;
    unsigned	L	: 1;
    unsigned	DB	: 1;
    unsigned	Gran	: 1;	// Granularity
    unsigned	BaseHi	: 8;
} SEGMENT_DESCRIPTOR, *PSEGMENT_DESCRIPTOR;

typedef struct
{
    USHORT limit0;
    USHORT base0;
    UCHAR  base1;
    UCHAR  attr0;
    UCHAR  limit1attr1;
    UCHAR  base2;
} SEGMENT_DESCRIPTOR2, *PSEGMENT_DESCRIPTOR2;

typedef struct _IDT_ENTRY_ {
	SHORT OffsetLow;
	SHORT Selector;
	BYTE Reserved;
	BYTE Type: 4;
	BYTE Always0: 1;
	BYTE Dpl: 2;
	BYTE Present: 1;
	SHORT OffsetHigh;
} IDT_ENTRY, *PIDT_ENTRY;

typedef struct _IDTR
{
    unsigned	Limit		: 16;
    unsigned	BaseLo		: 16;
    unsigned	BaseHi		: 16;
} IDTR;


#define LA_ACCESSED		0x01
#define LA_READABLE		0x02    // for code segments
#define LA_WRITABLE		0x02    // for data segments
#define LA_CONFORMING	0x04    // for code segments
#define LA_EXPANDDOWN	0x04    // for data segments
#define LA_CODE			0x08
#define LA_STANDARD		0x10
#define LA_DPL_0		0x00
#define LA_DPL_1		0x20
#define LA_DPL_2		0x40
#define LA_DPL_3		0x60
#define LA_PRESENT		0x80

#define LA_LDT64		0x02
#define LA_ATSS64		0x09
#define LA_BTSS64		0x0b
#define LA_CALLGATE64	0x0c
#define LA_INTGATE64	0x0e
#define LA_TRAPGATE64	0x0f

#define HA_AVAILABLE	0x01
#define HA_LONG			0x02
#define HA_DB			0x04
#define HA_GRANULARITY	0x08

#pragma pack (pop)


VOID _ExitEventHandler( VOID );
NTSTATUS NTAPI UnLoadHypervisor( );
NTSTATUS CheckVtSupport();

#endif __HYPERVISOR__H__

