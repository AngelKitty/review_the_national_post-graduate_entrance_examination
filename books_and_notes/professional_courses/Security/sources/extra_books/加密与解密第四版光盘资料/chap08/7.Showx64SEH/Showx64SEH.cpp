// Showx64SEH.cpp : 定义控制台应用程序的入口点。
//
/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>
#include <ImageHlp.h>

#pragma comment(lib,"imagehlp.lib")
#pragma comment(linker,"/Base:0x10000000")

#define ALIGN_SIZEUP(Size,Alignment)  (((ULONG_PTR)(Size) + Alignment - 1) & ~(Alignment - 1))

typedef enum _UNWIND_OP_CODES {
	UWOP_PUSH_NONVOL = 0,
	UWOP_ALLOC_LARGE,       // 1
	UWOP_ALLOC_SMALL,       // 2
	UWOP_SET_FPREG,         // 3
	UWOP_SAVE_NONVOL,       // 4
	UWOP_SAVE_NONVOL_FAR,   // 5
	UWOP_SPARE_CODE1,       // 6
	UWOP_SPARE_CODE2,       // 7
	UWOP_SAVE_XMM128,       // 8
	UWOP_SAVE_XMM128_FAR,   // 9
	UWOP_PUSH_MACHFRAME     // 10
} UNWIND_OP_CODES, *PUNWIND_OP_CODES;

typedef union _UNWIND_CODE {
	struct {
		UCHAR CodeOffset;
		UCHAR UnwindOp : 4;
		UCHAR OpInfo : 4;
	};

	USHORT FrameOffset;

} UNWIND_CODE, *PUNWIND_CODE;

#define UNW_FLAG_NHANDLER 0x0
#define UNW_FLAG_EHANDLER 0x1
#define UNW_FLAG_UHANDLER 0x2
#define UNW_FLAG_CHAININFO 0x4

typedef struct _UNWIND_INFO {
	UCHAR Version : 3;
	UCHAR Flags : 5;
	UCHAR SizeOfProlog;
	UCHAR CountOfCodes;
	UCHAR FrameRegister : 4;
	UCHAR FrameOffset : 4;
	UNWIND_CODE UnwindCode[1];
} UNWIND_INFO, *PUNWIND_INFO;

typedef struct _SCOPE_TABLE {
	ULONG Count;
	struct
	{
		ULONG BeginAddress;
		ULONG EndAddress;
		ULONG HandlerAddress;
		ULONG JumpTarget;
	} ScopeRecord[1];
} SCOPE_TABLE, *PSCOPE_TABLE;

typedef struct _EH_RECORD 
{
	ULONG HandlerProc;
	SCOPE_TABLE ScopeTable;
}EH_RECORD,*PEH_RECORD;

VOID UnHandlerExpcetion(void);
VOID CatchException(VOID);
VOID ThreeException();
VOID ShowUnWindInfo(HMODULE hMod,ULONG_PTR FuncAddress);

int _tmain(int argc, _TCHAR* argv[])
{
	ULONG_PTR FuncAddress = (ULONG_PTR)UnHandlerExpcetion;

#ifdef _DEBUG
	FuncAddress = FuncAddress + *(ULONG*)(FuncAddress+1) + 5 ;
#endif
	printf("Sizeof UNWIND_CODE = %d\n",sizeof(UNWIND_CODE));
	printf("Sizeof UNWIND_INFO = %d\n",FIELD_OFFSET(UNWIND_INFO,UnwindCode));
	printf("Target FuncAddres = 0x%p\n",FuncAddress);
	ShowUnWindInfo(GetModuleHandle(NULL),FuncAddress);
	printf("=================Dump UnWindInfo End================\n");
	CatchException();
	printf("=================Catch Exception End================\n");
	ThreeException();
	getchar();
	return 0;
}

//本函数捕捉异常，但不处理异常
VOID UnHandlerExpcetion(void)
{
	__int64 *pValue = NULL;
	__try
	{
		*pValue = 0x555555555;
	}
	__except(printf("In Filter1.\n"),EXCEPTION_CONTINUE_SEARCH)
	{
		printf("In Handler1.\n");
	}
}

//捕捉子函数的异常
VOID CatchException(VOID)
{
	__try
	{
		UnHandlerExpcetion();
	}
	__except(printf("In Filter2.\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In Handler2.\n");
	}
}

//三个异常，一个独立，两个嵌套
VOID ThreeException()
{
	int *pValue = NULL ;

	__try
	{
		printf("In Try0.\n");
		*pValue = 0x55555555;
	}
	__except(printf("In Filter0\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In Handler0.\n");
	}

	__try
	{
		printf("In Try1.\n");
		__try
		{
			printf("In Try2.\n");

		}
		__except(printf("In Filter2\n"),EXCEPTION_CONTINUE_SEARCH)
		{
			printf("In Handler2.\n");
		}
	}
	__except(printf("In Filter1\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In Handler1.\n");
	}

	printf("After All Trys.\n");
}

VOID ShowUnWindInfo(HMODULE hMod,ULONG_PTR FuncAddress)
{
	ULONG_PTR ControlPc = (ULONG_PTR)FuncAddress - (ULONG_PTR)hMod;
	PVOID pExcepDir = NULL ;
	ULONG DirSize = 0;
	ULONG i = 0 ;
	PRUNTIME_FUNCTION pTargetEntry = NULL ;

	pExcepDir = ImageDirectoryEntryToData(hMod,TRUE,IMAGE_DIRECTORY_ENTRY_EXCEPTION,&DirSize);
	if (pExcepDir == NULL || DirSize == 0)
	{
		printf("No Exception Directory.");
		return ;
	}

	printf("Exception Dir = 0x%p , Size = 0x%X\n",pExcepDir,DirSize);

	PRUNTIME_FUNCTION pFuncTable = (PRUNTIME_FUNCTION)pExcepDir;

	
	printf("All RunTimeFunction Info:\n");
	for (i=0;i< DirSize / sizeof(RUNTIME_FUNCTION);i++)
	{
		printf("[%2d] StartVa:0x%08X EndVa:0x%08X UnWindDataVA:0x%08X\n",
			i,pFuncTable->BeginAddress,pFuncTable->EndAddress,pFuncTable->UnwindData);

		//比较并定位函数所在的RUNTIME_FUNCTION结构，实际使用中是用RtlLookupFunctionEntry来查找的
		if (ControlPc >= pFuncTable->BeginAddress
			&& ControlPc < pFuncTable->EndAddress)
		{
			
			pTargetEntry = pFuncTable;
		}
		pFuncTable++;
	}

	if (pTargetEntry == NULL)
	{
		printf("Can not found RUNTIME_FUNCTION for addr 0x%p\n",FuncAddress);
		return ;
	}

	//读取UNWIND_INFO结构
	PUNWIND_INFO pUnWindInfo = (PUNWIND_INFO)((ULONG_PTR)hMod + pTargetEntry->UnwindData);
	printf("Unwind Info at            : 0x%p\n",pUnWindInfo);
	printf("Unwind Info Version       : %d\n",pUnWindInfo->Version);
	printf("Unwind Info Flag          : %d\n",pUnWindInfo->Flags);
	printf("Unwind Info SizeOfProlog  : %d\n",pUnWindInfo->SizeOfProlog);
	printf("Unwind Info CountOfCodes  : %d\n",pUnWindInfo->CountOfCodes);
	printf("Unwind Info FrameRegister : %d\n",pUnWindInfo->FrameRegister);
	printf("Unwind Info FrameOffset   : %d\n",pUnWindInfo->FrameOffset);

	ULONG CodeCount = pUnWindInfo->CountOfCodes ;

	//读取UNWIND_CODE结构
	PUNWIND_CODE pUnWindCode = (PUNWIND_CODE)&pUnWindInfo->UnwindCode ;
	
	for (i=0;i<CodeCount;i++)
	{
		printf("Unwind Code[%d] at   : 0x%p\n",i,pUnWindCode);
		printf(" CodeOffset          : %d\n",pUnWindCode->CodeOffset);
		printf(" UnwindOp            : %d\n",pUnWindCode->UnwindOp);
		printf(" OpInfo              : %d\n",pUnWindCode->OpInfo);
		pUnWindCode++;
	}
	
	//读取Handler及ScopeTable
	//printf("pUnWindCode :0x%p\n",pUnWindCode);
	PEH_RECORD pEhRecord = (PEH_RECORD)(ALIGN_SIZEUP(pUnWindCode,sizeof(ULONG)));//需要按4字节对齐
	ULONG ScopeTableItemCnt = pEhRecord->ScopeTable.Count;
	printf("EH_RECORD at            : 0x%p\n",pEhRecord);
	printf("HandlerProc             : 0x%p\n",pEhRecord->HandlerProc + (ULONG_PTR)hMod);
	printf("ScopeTableEntryCount    : %d\n",ScopeTableItemCnt);
	for (i=0;i<ScopeTableItemCnt;i++)
	{
		printf("ScopeRecord[%d]      :\n",i);
		printf(" TryBeginAddress     : 0x%p\n",(ULONG_PTR)hMod + pEhRecord->ScopeTable.ScopeRecord[i].BeginAddress);
		printf(" TryEndAddress       : 0x%p\n",(ULONG_PTR)hMod + pEhRecord->ScopeTable.ScopeRecord[i].EndAddress);
		printf(" HandlerAddress      : 0x%p\n",(ULONG_PTR)hMod + pEhRecord->ScopeTable.ScopeRecord[i].HandlerAddress);
		printf(" JumpTarget          : 0x%p\n",(ULONG_PTR)hMod + pEhRecord->ScopeTable.ScopeRecord[i].JumpTarget);
	}

}