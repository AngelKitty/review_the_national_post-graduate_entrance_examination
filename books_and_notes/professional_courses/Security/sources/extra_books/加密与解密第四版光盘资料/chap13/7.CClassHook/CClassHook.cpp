/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// CClassHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CClassHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
要解决两个问题
1.如何获取类成员函数的地址
2.如何使用普通函数替换类成员函数
*/
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


class base
{
public:
    virtual int Add(int a,int b);
    virtual void g(){cout<<"base::g"<<endl;};
    virtual void h(){cout<<"base::h"<<endl;};
	void novirtual(){cout<<"base::not virtual"<<endl;};
};

int base::Add(int a,int b)
{
	printf("base::Add\n");
	return a + b ;
}

class DetourClass
{
public:
    virtual int DetourFun(int a,int b);
};

class TrampolineClass
{
public:
	virtual int TrampolineFun(int a,int b){printf("TrampolineClass\n");return 0 ;};//原型与被Hook函数相同
};

DetourClass Detour;
TrampolineClass Trampoline;

int DetourClass::DetourFun(int a,int b)
{
	//TrampolineFun();  //由于这里的类实际上是base,所以调用TrampolineFun即调用第二个虚函数，相当于调用pbase->g()
	TrampolineClass *pTrampoline = new TrampolineClass;
	int result = pTrampoline->TrampolineFun(a,b);
	printf("DetourClass:: OriginalFun returned %d\n",result);
	result += 10 ;
	delete pTrampoline;
	return result;
}



typedef void (*pfun)();
LPVOID GetClassVirtualFnAddress(LPVOID pthis,int Index);
VOID HookClassMemberByAnotherClassMember();


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		HookClassMemberByAnotherClassMember();
		getchar();
	}

	return nRetCode;
}


void HookClassMemberByAnotherClassMember()
{
	base b;
	base *pbase=&b;
	
	DWORD dwOLD;
	MEMORY_BASIC_INFORMATION  mbi;

	printf("pbase = 0x%X\n",pbase);
	
	ULONG_PTR *vfTableToHook = (ULONG_PTR*)*(ULONG_PTR*)pbase;
	printf("vfTable = 0x%x\n",vfTableToHook);

	ULONG_PTR *vfTableTrampoline = (ULONG_PTR*)*(ULONG_PTR*)&Trampoline;
	
	//先将原函数的地址保存到当前类的表中，作为调用原函数的入口
	VirtualQuery(vfTableTrampoline,&mbi,sizeof(mbi));
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
	//保存原始数据
	//原函数位于第几个这里就是第几个，必须保证位置一样
	vfTableTrampoline[0] = (ULONG_PTR)GetClassVirtualFnAddress(pbase,0);
	printf("Base::Add()  %p\n",vfTableTrampoline[0]);
	TrampolineClass *p = &Trampoline;
	//恢复内存页的属性
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	//修改内存页的属性
	
	VirtualQuery(vfTableToHook,&mbi,sizeof(mbi));
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
	//保存原始数据
	vfTableToHook[0] = (ULONG_PTR)GetClassVirtualFnAddress(&Detour,0);
	printf("Detour::Add()  %p\n",vfTableToHook[0]);
	//恢复内存页的属性
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	int result = pbase->Add(1,2);    //调用第3个虚函数，实际调用的是HookClass::DetourFun()
	printf("result = %d  \nafter call member fun.\n",result);
}

//获得类虚拟成员函数指针
LPVOID GetClassVirtualFnAddress(LPVOID pthis,int Index) 
{
	ULONG_PTR *vfTable = (ULONG_PTR*)*(ULONG_PTR*)pthis;
	return (LPVOID)vfTable[Index];
}