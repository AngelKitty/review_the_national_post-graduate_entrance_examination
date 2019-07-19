#include "StdAfx.h"

//定义如下结构，保存一次InlineHook所需要的信息
typedef struct _HOOK_DATA{
	char szApiName[128];	//待Hook的API名字
	char szModuleName[64];	//待Hook的API所属模块的名字
	int  HookCodeLen;		//Hook长度
	BYTE oldEntry[16];		//保存Hook位置的原始指令
	BYTE newEntry[16];		//保存要写入Hook位置的新指令
	BYTE HotPatchCode[8];
	ULONG_PTR HookPoint;		//待HOOK的位置
	ULONG_PTR JmpBackAddr;		//回跳到原函数中的位置
	ULONG_PTR pfnTrampolineFun;	//调用原始函数的通道
	ULONG_PTR pfnDetourFun;		//HOOK过滤函数
}HOOK_DATA,*PHOOK_DATA;

LPVOID GetAddress(char *,char *);
BOOL InstallCodeHook(PHOOK_DATA pHookData);
BOOL UninstallCodeHook(PHOOK_DATA pHookData);