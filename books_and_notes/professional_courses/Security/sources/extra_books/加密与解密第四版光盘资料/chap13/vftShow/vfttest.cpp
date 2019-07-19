/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// vfttest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vfttest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


class PEDIYBase
{
public:
    virtual void fun1(){cout<<"PEDIYBase::fun1"<<endl;};
    virtual void fun2(){cout<<"PEDIYBase::fun2"<<endl;};
    virtual void fun3(){cout<<"PEDIYBase::fun3"<<endl;};
	void fun4(){cout<<"PEDIYBase::fun4"<<endl;};
};

class PEDIYEx : public PEDIYBase  
{
public:
	virtual void fun1();	
};

void PEDIYEx::fun1()
{
	cout<<"PEDIYEx::fun1"<<endl;
}

typedef void (*pfun)();

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
		
		pfun fun=NULL;
		PEDIYBase *pbase  = new PEDIYBase;
		PEDIYEx   *pDIYEx = new PEDIYEx;
		
		cout << "pbase = 0x" << std::hex << pbase << endl;
		
		
		pbase->fun1();
		pDIYEx->fun1();

		delete pDIYEx;
		delete pbase;
	}

	return nRetCode;
}


