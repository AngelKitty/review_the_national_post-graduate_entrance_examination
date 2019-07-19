//===========================================================================
/*
 * 第10章  VT技术
 * 《加密与解密（第四版）》
 * (c)  看雪学院 www.kanxue.com 2000-2018
 *
 * HvmEpt	:EPT内存虚拟化处理函数集
 * Author	:Joen
 * QQ		:51753931
 * E-mail	:Joen@JoenChen.com
 * Website	:http://www.joenchen.com
 * 这个文件是处理ept内存分页有关的函数
 */
//===========================================================================

#include "Ntifs.h"
#include "stdint.h"
#include "structs.h"
#include "Assembly.h"
#include "Hypervisor.h"
#include "HvmEpt.h"

void * MapInMemory(
    PAGE_CONTEXT * Context,
    PHYSICAL_ADDRESS Phys,
    UINT size )
{
    return MmMapIoSpace( Phys, size, 0 );
}

PAGE_DIR_ENTRY *
MapInPdeDirql(
    UINT CR3,
    void *VirtualAddress,
    PAGE_CONTEXT * Context )
{
    UINT PdeOff;
    PHYSICAL_ADDRESS PageDirPhys = {0};


    PdeOff = ( ( UINT ) VirtualAddress & 0xFFC00000 ) >> 22;

    PageDirPhys.LowPart = ( CR3 & 0xFFFFF000 ) |
                          ( PdeOff * sizeof( PAGE_DIR_ENTRY ) );

    return ( PAGE_DIR_ENTRY * ) MapInMemory( Context, PageDirPhys,
            sizeof( PAGE_DIR_ENTRY ) );
}


void MapOutMemory(
    PAGE_CONTEXT * Context,
    void * Pointer,
    UINT size )
{
    if ( Context == NULL )
    {
        MmUnmapIoSpace( Pointer, size );
    }
}


void
MapOutEntryDirql(
    void *Pointer,
    PAGE_CONTEXT * Context
)
{
    if ( Pointer != NULL )
    {
        MapOutMemory( Context, Pointer, sizeof( PAGE_TABLE_ENTRY ) );
    }
}

void MapOutEntry( void *Pointer )
{
    MapOutEntryDirql( Pointer, NULL );
}

PAGE_TABLE_ENTRY *
MapInPteDirql(
    UINT CR3,
    void *VirtualAddress,
    PAGE_CONTEXT * Context
)
{
    UINT PteOff;
    PAGE_DIR_ENTRY_SMALL *PageDir;
    PPAGE_TABLE_ENTRY RetPte = NULL;
    PHYSICAL_ADDRESS PageTablePhys = {0};


    PageDir = ( PAGE_DIR_ENTRY_SMALL * )MapInPdeDirql(
                  CR3, VirtualAddress, Context );

    if ( PageDir == NULL )
    {
        return NULL;
    }

    //
    // 确定处理的是大页面或者小页面.
    //
    if ( PageDir->ps == 1 || PageDir->p == 0 )
    {
        RetPte = NULL;
    }
    else
    {
        PteOff = ( ( UINT ) VirtualAddress & 0x003FF000 ) >> 12;
        PageTablePhys.LowPart = ( PageDir->address << 12 ) |
                                ( PteOff * sizeof( PAGE_TABLE_ENTRY ) );

        RetPte = MapInMemory( Context, PageTablePhys, sizeof( *RetPte ) );
    }

    MapOutEntryDirql( ( void * ) PageDir, Context );
    return RetPte;
}


PAGE_TABLE_ENTRY *
MapInPte(
    UINT CR3,
    void *VirtualAddress
)
{
    return MapInPteDirql( CR3, VirtualAddress, NULL );
}

PAGE_DIR_ENTRY *
MapInPde(
    UINT CR3,
    void *virtualAddress
)
{
    return MapInPdeDirql( CR3, virtualAddress, NULL );
}


UINT
GetNext(
    PAGE_WALK_CONTEXT *Context
)
{
    PHYSICAL_ADDRESS ptePhys = {0};

    if ( Context->Pde == NULL )
        return 0;

    if ( Context->Pte == NULL )
    {
        for ( ; Context->PdeOff < 1024; Context->PdeOff++ )
        {
            // Large pages (could be paged out)
            if ( ( Context->Pde[Context->PdeOff] ).ps == 1 )
            {
                if ( Context->targetAddress.LowPart >> 22 ==
                        ( Context->Pde[Context->PdeOff] ).address )
                {
                    // Prevent an endless loop
                    Context->PdeOff++;
                    return ( ( Context->PdeOff - 1 ) << 22 ) |
                           ( Context->targetAddress.LowPart & 0x003FFFFF );
                }
            }

            // Small pages
            else if ( ( Context->Pde[Context->PdeOff] ).p == 1 &&
                      ( Context->Pde[Context->PdeOff] ).ps == 0 )
            {
                ptePhys.LowPart = ( ( PAGE_DIR_ENTRY_SMALL * )
                                    &( Context->Pde[Context->PdeOff] ) )->address << 12;
                Context->Pte = MmMapIoSpace( ptePhys, 1024 * sizeof( PAGE_TABLE_ENTRY ), 0 );

                // Loop through a page table
                for ( Context->PteOff = 0; Context->PteOff < 1024; Context->PteOff++ )
                {
                    if ( ( Context->Pte[Context->PteOff] ).address ==
                            Context->targetAddress.LowPart >> 12 )
                    {

                        Context->PteOff++;
                        return ( Context->PdeOff << 22 ) | ( ( Context->PteOff - 1 ) << 12 ) |
                               ( Context->targetAddress.LowPart & 0x00000FFF );
                    }
                }

                MmUnmapIoSpace( Context->Pte, 1024 * sizeof( PAGE_TABLE_ENTRY ) );
                Context->Pte = NULL;
                Context->PteOff = 0;
            }
        }
    }
    else
    {
        // Loop through a page table
        for ( ; Context->PteOff < 1024; Context->PteOff++ )
        {
            if ( ( Context->Pte[Context->PteOff] ).address ==
                    Context->targetAddress.LowPart >> 12 )
            {

                Context->PteOff++;
                return ( Context->PdeOff << 22 ) | ( ( Context->PteOff - 1 ) << 12 ) |
                       ( Context->targetAddress.LowPart & 0x00000FFF );
            }
        }

        MmUnmapIoSpace( Context->Pte, 1024 * sizeof( PAGE_TABLE_ENTRY ) );
        Context->Pte = NULL;
        Context->PteOff = 0;
        Context->PdeOff++;

        // Recurse to the next PDE
        return GetNext( Context );
    }

    return 0;
}

void
UnlockProcessMemory(
    PEPROCESS Proc,
    PKAPC_STATE ApcState,
    PMDLX mdl )
{
    // Attach to process to ensure virtual addresses are correct
    KeStackAttachProcess( Proc, ApcState );

    // Unlock & free MDL and corresponding pages
    MmUnlockPages( mdl );
    IoFreeMdl( mdl );

    KeUnstackDetachProcess( ApcState );
}

void
InitMappingOperations(
    PAGE_CONTEXT *Context,
    UINT numPages )
{
    UINT i, cr3Val;
    const UINT tag = 'Page';
    PHYSICAL_ADDRESS phys = {0};
    PAGE_DIR_ENTRY_SMALL *pde;

    cr3Val = _GetCr3();

    Context->CR3Val = cr3Val;

    phys.LowPart = cr3Val & 0xFFFFF000;

    Context->PageArray = ( BYTE * ) ExAllocatePoolWithTag( NonPagedPool,
                         numPages * PAGE_SIZE, tag );

    Context->NumPages = numPages;

    Context->PageArrayBitmap = ( BYTE * ) ExAllocatePoolWithTag( NonPagedPool,
                               numPages, tag );

    RtlZeroMemory( Context->PageArrayBitmap, numPages );

}

void
EndMappingOperations(
    PAGE_CONTEXT *Context
)
{
    const UINT tag = 'Page';
    PHYSICAL_ADDRESS phys = {0};
    PAGE_DIR_ENTRY_SMALL *pde;

    ExFreePoolWithTag( Context->PageArray, tag );
    Context->NumPages = 0;
    ExFreePoolWithTag( Context->PageArrayBitmap, tag );

}

void *
AllocPage(
    PAGE_CONTEXT *Context
)
{
    UINT i;

    for ( i = 0; i < Context->NumPages; i++ )
    {
        if ( Context->PageArrayBitmap[i] == 0 )
        {
            // Mark page as taken
            Context->PageArrayBitmap[i] = 1;
            return Context->PageArray + ( i * PAGE_SIZE );
        }
    }

    // No memory left
    return NULL;
}


//
// 初始化ept页表结构
//
EPT_PML4_ENTRY *
InitEptIdentityMap(
    PEPT_INFO EptInfo
)
{
    UINT i, j, PdeCounter = 0;
    EPT_PML4_ENTRY *Pml4Ptr = NULL;
    EPT_PDPTE_ENTRY *PdptePtr = NULL;
    PHYSICAL_ADDRESS phys = {0};
    PHYSICAL_ADDRESS Highest = {0};
    PHYSICAL_ADDRESS Lowest = {0};

    Highest.LowPart = ~0;

    //
    //分配连续非分页内存
    //
    Pml4Ptr = ( EPT_PML4_ENTRY * ) MmAllocateContiguousMemorySpecifyCache(
                  sizeof( EPT_PML4_ENTRY ) * 512,
                  Lowest,
                  Highest,
                  Lowest,
                  0 );

    if ( Pml4Ptr == NULL )
    {
        return NULL;
    }

    PdptePtr = ( EPT_PDPTE_ENTRY * ) MmAllocateContiguousMemorySpecifyCache(
                   sizeof( EPT_PDPTE_ENTRY ) * 512,
                   Lowest,
                   Highest,
                   Lowest,
                   0 );

    //
    //保存PdptePtr的虚拟地址，方便后续释放
    //
    EptInfo->BkupPdptePtr = PdptePtr;

    if ( PdptePtr == NULL )
    {
        MmFreeContiguousMemory( Pml4Ptr );
        return NULL;
    }

    //
    //这里是分配内存, 2MB×512=1GB
    //
    for ( i = 0; i < NUM_PD_PAGES; i++ )
    {
        //
        //申请512个PDE，每个PDE映射2MB的空间，512×2MB=1GB
        //
        EptInfo->BkupPdePtrs[i] = ( EPT_PDE_ENTRY_2M * ) MmAllocateContiguousMemorySpecifyCache(
                                      sizeof( EPT_PDE_ENTRY_2M ) * 512,
                                      Lowest,
                                      Highest,
                                      Lowest,
                                      0 );

        if ( EptInfo->BkupPdePtrs[i] != NULL )
        {
            RtlZeroMemory( ( void * ) EptInfo->BkupPdePtrs[i],
                           sizeof( EPT_PDE_ENTRY_2M ) * 512 );
        }
        else
        {
            MmFreeContiguousMemory( Pml4Ptr );
            MmFreeContiguousMemory( PdptePtr );

            for ( j = 0; j < i; j++ )
            {
                MmFreeContiguousMemory( EptInfo->BkupPdePtrs[j] );
            }

            return NULL;
        }
    }

    phys = MmGetPhysicalAddress( ( void * ) PdptePtr );

    RtlZeroMemory( ( void * ) Pml4Ptr, sizeof( EPT_PML4_ENTRY ) * 512 );
    RtlZeroMemory( ( void * ) PdptePtr, sizeof( EPT_PDPTE_ENTRY ) * 512 );

    //
    //每个PMLE4管理512GB内存，第0位置1表示可读
    //
    Pml4Ptr->Present  = 1;

    //
    //第1位置1表示可写
    //
    Pml4Ptr->Write    = 1;

    //
    //第2位置1表示可执行，此Pml4Ptr管理的512GB内存可读、可写、可执行
    //
    Pml4Ptr->Execute  = 1;
    Pml4Ptr->PhysAddr = phys.LowPart >> 12;

    //
    //外层循环填写4个页面，一个1GB，32位系统只有4GB内存，所以这里循环4次
    //
    for ( i = 0; i < NUM_PD_PAGES; i++ )
    {
        phys = MmGetPhysicalAddress( ( PVOID ) EptInfo->BkupPdePtrs[i] );

        PdptePtr[i].Present = 1;
        PdptePtr[i].Write = 1;
        PdptePtr[i].Execute = 1;

        //
        //设置4个PDPTE项，一项对应管理1GB内存
        //
        PdptePtr[i].PhysAddr = phys.LowPart >> 12;

        //
        //内层循环填写2MB页面，2MB×512=1GB
        //
        for ( j = 0; j < 512; j++ )
        {

            //
            //i代表PDPTE页，管理1GB内存，每个PDPTE页里包含512项PDE，每项PDE管理2MB内存
            //j代表PDE项，为每个PDE项设置属性，第0位置1可读
            //
            EptInfo->BkupPdePtrs[i][j].Present = 1;

            //
            //每个PDE项第1位置1可写
            //
            EptInfo->BkupPdePtrs[i][j].Write = 1;

            //
            //每个PDE项第2位置1可执行
            //
            EptInfo->BkupPdePtrs[i][j].Execute = 1;

            //
            // EPT_MEMORY_TYPE_WB = 6, WriteBack类型，
            //
            EptInfo->BkupPdePtrs[i][j].MemoryType = EPT_MEMORY_TYPE_WB;

            //
            //第7位指示是否使用大页面，即1GB或2MB页面
            //此位只存在于PDPTE与PDE上，在PML4E上是保留位（必须为0），
            //PDPTE[7]=1时使用1GB页面，PDE[7]=1时使用2MB页面
            //
            EptInfo->BkupPdePtrs[i][j].Size = 1;

            EptInfo->BkupPdePtrs[i][j].PhysAddr = PdeCounter;

            PdeCounter++;
        }
    }


    // KdPrint( ( "SW-> ept 1G Entry 512 Item[pml4Ptr]: %p  \n", pml4Ptr  ) );
    // KdPrint( ( "SW-> ept 2M Entry 4 Item[pdptePtr]: %p 2M Entry[pdptePtr]:%p  \n", pml4Ptr, pdptePtr  ) );

    return Pml4Ptr;
}

void
FreeEptIdentityMap(
    PEPT_INFO EptInfo,
    EPT_PML4_ENTRY * Pointer
)
{
    UINT i;

    if ( EptInfo->BkupPdptePtr != NULL )
    {
        MmFreeContiguousMemory( ( void * ) EptInfo->BkupPdptePtr );
    }

    if ( Pointer != NULL ) 
	{
		MmFreeContiguousMemory( ( void * ) Pointer );
	}

    for ( i = 0; i < NUM_PD_PAGES; i++ )
    {
        if ( NULL != EptInfo->BkupPdePtrs[i] )
        {
            MmFreeContiguousMemory( EptInfo->BkupPdePtrs[i] );
        }
    }

    for ( i = 0; i < EptInfo->EptPageTableCounter; i++ )
    {
        if ( NULL != ( void * ) EptInfo->EptTableArray[i] )
        {
            MmFreeContiguousMemory( ( void * )EptInfo->EptTableArray[i] );
        }
    }
}

PEPT_PTE_ENTRY
EptMapAddressToPteDirql(
    PEPT_INFO EptInfo,
    UINT GuestPhysAddr,
    EPT_PML4_ENTRY * Pml4Ptr,
    PAGE_CONTEXT * Context
)
{
    UINT i, PdpteOff,PdeOff,PteOff;
    EPT_PDE_ENTRY_2M* Pde = NULL;
    EPT_PDPTE_ENTRY *Pdpte = NULL;
    PEPT_PTE_ENTRY PageTable = NULL;
    PHYSICAL_ADDRESS Phys = {0};

    PdpteOff = ( ( GuestPhysAddr >> 30 ) & 0x3 );
    PdeOff   = ( ( GuestPhysAddr >> 21 ) & 0x1FF );
    PteOff   = ( ( GuestPhysAddr >> 12 ) & 0x1FF );

    //
    // 获取PDE指针
    //
    Pde = EptInfo->BkupPdePtrs[PdpteOff];

    //
    // 根据是2MB页面或者是页表做不同处理
    //
    if ( Pde[PdeOff].Size == 1 )
    {
        //
        // 如果前面没有做PDE的页表映射, 这里我们重新建立一下
        //
        Phys.LowPart = ~0;

        if ( Context == NULL )
        {
            PageTable = ( PEPT_PTE_ENTRY ) MmAllocateContiguousMemory(
                            sizeof( EPT_PTE_ENTRY ) * 512, Phys );
        }
        else
        {
            PageTable = ( PEPT_PTE_ENTRY ) AllocPage( Context );
        }

        if ( PageTable == NULL )
        {
            return NULL;
        }

        RtlZeroMemory( ( void * ) PageTable, sizeof( EPT_PTE_ENTRY ) * 512 );

        //
        // 填写页表属性.
        //
        for ( i = 0; i < 512; i++ )
        {
            PageTable[i].Present = 1;
            PageTable[i].Write = 1;
            PageTable[i].MemoryType = EPT_MEMORY_TYPE_WB;
            PageTable[i].Execute = 1;
            PageTable[i].PhysAddr = ( ( ( Pde[PdeOff].PhysAddr << 21 ) & 0xFFFFFFFF ) >> 12 ) + i;
        }

        Pde[PdeOff].Size = 0;
        Pde[PdeOff].IgnorePat = 0;
        Pde[PdeOff].MemoryType = 0;

        Phys = MmGetPhysicalAddress( ( void * ) PageTable );
        ( ( EPT_PDE_ENTRY * ) Pde )[PdeOff].PhysAddr = Phys.LowPart >> 12;

        EptInfo->EptTableVirts[EptInfo->TableVirtsCounter] = PageTable;
        EptInfo->TableVirtsCounter++;

        if ( Context == NULL )
        {
            EptInfo->EptTableArray[EptInfo->EptPageTableCounter] = PageTable;
            EptInfo->EptPageTableCounter++;
        }

        return &PageTable[PteOff];
    }

    //
    // 查找PTE
    //
    for ( i = 0; i < EptInfo->TableVirtsCounter; i++ )
    {
        if ( EptInfo->EptTableVirts[i][0].PhysAddr << 12 <= GuestPhysAddr &&
                EptInfo->EptTableVirts[i][511].PhysAddr << 12 >= GuestPhysAddr )
        {
            return &EptInfo->EptTableVirts[i][PteOff];
        }
    }

    return NULL;
}


//
// 查找GPA在EPT页表中的PTE
//
PEPT_PTE_ENTRY
EptMapAddressToPte(
    PEPT_INFO EptInfo,
    UINT guestPhysicalAddress,
    EPT_PML4_ENTRY * Pml4Ptr
)
{
    return EptMapAddressToPteDirql( EptInfo,guestPhysicalAddress,
                                    Pml4Ptr, NULL );
}

//
// 确定这是否是一个2MB的大页面或点映射到一个页表
//
BYTE
EptPtExists(
    PEPT_INFO EptInfo,
    UINT guestPhysicalAddress
)
{
    UINT PdpteOff;
    UINT PdeOff;
    EPT_PDE_ENTRY_2M *Pde = NULL;

    PdpteOff = ( ( guestPhysicalAddress >> 30 ) & 0x3 ),
    PdeOff = ( ( guestPhysicalAddress >> 21 ) & 0x1FF );

    // 获取PDE指针
    Pde = EptInfo->BkupPdePtrs[PdpteOff];

    return !( Pde[PdeOff].Size );
}


void
EptUnmapPteDirql(
    PEPT_PTE_ENTRY Pointer,
    PAGE_CONTEXT * Context )
{
    if ( Pointer == NULL )
    {
        return;
    }

    MapOutMemory( Context, ( void * ) Pointer, sizeof( EPT_PTE_ENTRY ) );
}

void EptUnmapPte(
    PEPT_PTE_ENTRY Pointer
)
{
    EptUnmapPteDirql( Pointer, NULL );
}


TLB_TRANSLATION *
GetTranslationInfo(
    TLB_TRANSLATION * TransArr,
    UINT GuestPhysAddr )
{
    UINT i = 0;
    GuestPhysAddr &= 0xFFFFF000;

    if( TransArr == NULL )
	{
		return NULL;
	}

    //
    // 在TransArr中查表, 如果找到表示是我们在EPT中做了GPA->HPA的映射
	// 根据DATA和CODE返回不同的Translation结构
    //
    while( TransArr[i].DataPhys != 0 )
    {
        if( TransArr[i].CodeOrData == DATA_EPT && ( GuestPhysAddr == TransArr[i].DataPhys ) )
        {
            return &TransArr[i];
        }
        else if ( TransArr[i].CodeOrData == CODE_EPT && ( GuestPhysAddr == TransArr[i].CodePhys )  )
        {
            return &TransArr[i];
        }

        i++;
    }

    return NULL;
}

//
// Handle the EPT violation
//
void Handle_Exec_Ept(
    PEPT_INFO EptInfo,
    PGUEST_REGS GuestReg )
{
    UINT GuestPhys;
    UINT GuestLinear;
    UINT ExitQualification;
    PEPT_PTE_ENTRY PtePtr = NULL;
    TLB_TRANSLATION *translationPtr;

    GuestPhys     = _ReadVMCS( GUEST_PHYSICAL_ADDRESS );
    ExitQualification = _ReadVMCS( EXIT_QUALIFICATION );
    GuestLinear       = _ReadVMCS( GUEST_LINEAR_ADDRESS );
    translationPtr    = GetTranslationInfo( EptInfo->SplitPages, GuestPhys );

    //
    // 如果无法找到正确的地址翻译, 那么页表建立是出错了
    //
    if ( translationPtr == NULL )
    {
        End_Split( EptInfo, EptInfo->SplitPages, EptInfo->ImageSize );
        return;
    }

	//
    // Get the faulting EPT PTE
	//
    PtePtr = translationPtr->EptPte;

    if ( PtePtr != NULL && PtePtr->Present == 1 && PtePtr->Execute == 1 )
    {
        return;
    }

    EptInfo->Violation++;

    KdPrint( ( "SW-> Guest Linear Address: %p\n", GuestLinear ) );
    KdPrint( ( "SW-> Guest Physical      : %p\n", GuestPhys ) );
    KdPrint( ( "SW-> Guest Eip           : %p\n", _ReadVMCS( GUEST_EIP ) ) );

    if ( ExitQualification & EPT_MASK_DATA_EXEC )
    {
        // 代码执行
        EptInfo->Execute++;

        PtePtr->PhysAddr = translationPtr->CodePhys >> 12;

        KdPrint( ( "SW-> Handle_Exec_Ept Code Execute Phys Addr:%p\n", PtePtr->PhysAddr  ) );

        PtePtr->Execute = 1;
    }
    else if ( ExitQualification & EPT_MASK_DATA_READ ||
              ExitQualification & EPT_MASK_DATA_WRITE )
    {
        //数据访问
        EptInfo->Data++;
        PtePtr->PhysAddr = translationPtr->DataPhys >> 12;

        KdPrint( ( "SW-> Handle_Exec_Ept Data access Phys Addr:%p\n", PtePtr->PhysAddr  ) );

        PtePtr->Present = 1;
        PtePtr->Write = 1;
    }

    KdPrint( ( "SW-> -----------------------------------------------------------------\n" ) );
}

//
// 初始化分割TLB缓存
//
void Init_Split(
    PEPT_INFO EptInfo,
    PTLB_TRANSLATION TranslationPtr,
    ULONG ImageSize
)
{
    UINT i = 0;
    PEPT_PTE_ENTRY Pte = NULL;

    EptInfo->Data = 0;
    EptInfo->Execute = 0;
    EptInfo->Violation = 0;
    EptInfo->SplitPages = TranslationPtr;

    //
    // 遍历所有PE文件页面
    //
    while( TranslationPtr[i].DataPhys != 0 && i < ImageSize / PAGE_SIZE )
    {
    	//
        // 在EPT页表中查询Guest OS的物理地址映射的PTE, GPA到HPA的映射
        // 找到后将PTE bit2:0置0, 这样就会造成#VmExit陷入
        //
        if ( TranslationPtr[i].CodeOrData == CODE_EPT )
        {
            KdPrint( ( "SW-> Map Code address:%p Phys:%p\n",
                       TranslationPtr[i].VirtualAddress, TranslationPtr[i].CodePhys ) );

            Pte = EptMapAddressToPte( EptInfo, TranslationPtr[i].CodePhys, NULL );
        }
        else
        {
            KdPrint( ( "SW-> Map Data address:%p Phys:%p\n",
                       TranslationPtr[i].VirtualAddress, TranslationPtr[i].DataPhys ) );

            Pte = EptMapAddressToPte( EptInfo, TranslationPtr[i].DataPhys, NULL );
        }

		//
		//  bit2:0为0时表示页面不存在，造成#VmExit陷入
		//
        Pte->Present = 0;
        Pte->Write = 0;
        Pte->Execute = 0;
        TranslationPtr[i].EptPte = Pte;
        i++;
    }

}

//
// 停止内存页面分割
//
void End_Split(
    PEPT_INFO EptInfo,
    TLB_TRANSLATION * TranslationPtr,
    ULONG ImageSize
)
{
    UINT i = 0;
    PEPT_PTE_ENTRY Pte = NULL;

    KdPrint( ( "SW-> Tear-down TLB split Violations:%d Data:%d Exec:%d\n",
               EptInfo->Violation,
               EptInfo->Data,
               EptInfo->Execute ) );

    if ( TranslationPtr != NULL )
    {
        while( TranslationPtr[i].DataPhys != 0 && i < ImageSize / PAGE_SIZE )
        {
            Pte = TranslationPtr[i].EptPte;

            if ( TranslationPtr[i].CodeOrData == CODE_EPT )
            {
                Pte->PhysAddr = TranslationPtr[i].CodePhys >> 12;
            }
            else
            {
                Pte->PhysAddr = TranslationPtr[i].DataPhys >> 12;
            }

            Pte->Present = 1;
            Pte->Write = 1;
            Pte->Execute = 1;
            i++;
        }
    }

    EptInfo->SplitPages = NULL;
}



VOID
freeTranslation(
    ULONG ImageSize,
    PTLB_TRANSLATION TranslationPtr,
    PHYSICAL_ADDRESS *targetPhys,
    PPAGE_TABLE_ENTRY* targetPtes )
{
    const UINT tag = 'Joen';
    UINT i = 0;

    for( i = 0; i < ImageSize / 0x1000; i++ )
    {
        MapOutEntry( targetPtes[i] );
    }

    ExFreePoolWithTag( targetPhys, tag );
	ExFreePoolWithTag( TranslationPtr, tag );
}


void
MapOutImageHeader(
    BYTE *peBaseAddr
)
{
    MmUnmapIoSpace( peBaseAddr, PAGE_SIZE );
}

//
// 初始化Translation结构
//
PTLB_TRANSLATION
InitTranslation(
    BYTE *codePtr,
    BYTE *dataPtr,
    UINT len,
    PEPROCESS proc,
    PKAPC_STATE apc,
    PSPLIT_PROCESS_INFO SplitInfo )

{
    const UINT tag = 'Joen';
    UINT i = 0, numPages = len / 0x1000;
    PHYSICAL_ADDRESS tmpPhys = {0};
    TLB_TRANSLATION nullTranslation = {0};
    PTLB_TRANSLATION arr;

	arr = ( PTLB_TRANSLATION ) ExAllocatePoolWithTag( NonPagedPool,
		( numPages + 1 ) * sizeof( TLB_TRANSLATION ),
		tag );


    SplitInfo->targetPhys = ( PHYSICAL_ADDRESS * ) ExAllocatePoolWithTag( NonPagedPool,
                            ( numPages + 1 ) * sizeof( PHYSICAL_ADDRESS ),
                            tag );

    SplitInfo->targetPtes = ( PPAGE_TABLE_ENTRY* ) ExAllocatePoolWithTag( NonPagedPool,
                            ( numPages + 1 ) * sizeof( PPAGE_TABLE_ENTRY ),
                            tag );

    if( arr == NULL || SplitInfo->targetPtes == NULL || SplitInfo->targetPhys == NULL )
    {
        return NULL;
    }

    RtlZeroMemory( arr, ( numPages + 1 ) * sizeof( TLB_TRANSLATION ) );

    //
    // Loop through the VA space of the PE image and get the physical addresses
    //
    for( i = 0; i < numPages; i++ )
    {
        KeStackAttachProcess( proc, apc );

        tmpPhys = MmGetPhysicalAddress( ( PVOID )( ( UINT ) codePtr + ( i * PAGE_SIZE ) ) );

        KeUnstackDetachProcess( apc );

        //
        // 将代码地址映射到页表中
        //
        SplitInfo->targetPtes[i] = MapInPte( SplitInfo->cr3, ( BYTE * ) codePtr + ( i * PAGE_SIZE ) );

        arr[i].CodePhys = tmpPhys.LowPart;
        SplitInfo->targetPhys[i] = tmpPhys;

        arr[i].CodeOrData = CODE_EPT;

        arr[i].VirtualAddress = ( ( UINT ) codePtr + ( i * PAGE_SIZE ) );

        //
        // 数据地址是我们刚在非分页内存上申请的
        //
        tmpPhys = MmGetPhysicalAddress( ( PVOID )( ( UINT ) dataPtr + ( i * PAGE_SIZE ) ) );

        arr[i].DataPhys = tmpPhys.LowPart;

        KdPrint( ( "SW-> Virt:%p Code Phys Addr:%8x Data Phys Addr:%8x\n",
                   arr[i].VirtualAddress, arr[i].CodePhys, arr[i].DataPhys ) );
    }

    //
    // Zero out the last element
    //
    arr[numPages] = nullTranslation;
    return arr;
}
