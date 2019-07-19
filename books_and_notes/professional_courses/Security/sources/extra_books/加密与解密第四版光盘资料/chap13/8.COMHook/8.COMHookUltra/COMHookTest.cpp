/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// COMHookTest.cpp : Defines the entry point for the console application.
//

#include "stdafX.h"
#include "COMHookTest.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

VOID InstallComHook();
ULONG_PTR* GetComProcAddrPointer( HMODULE hMod,LPCTSTR szClsGuid,LPCTSTR szDispatchGuid,LPCTSTR szFunctionName,LPSTR szErrMsg/*=NULL*/ );
ULONG_PTR HookComFunction( HMODULE hMod,LPCTSTR szClsGuid,LPCTSTR szDispatchGuid,LPCTSTR szFunctionName,ULONG_PTR dwNewFunAddr );
VOID CONVERT_STR_2_GUID(LPTSTR cstr, const GUID *stGuid);
VOID ShellTest();
HRESULT STDMETHODCALLTYPE My_FindFiles( IShellDispatch * This);

typedef HRESULT
( STDMETHODCALLTYPE *PFN_FindFiles )(IShellDispatch * This);

PFN_FindFiles g_OriginalFindFiles;


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
		LoadLibrary(_T("shell32.dll"));
		InstallComHook();
		ShellTest();
	}

	return nRetCode;
}

VOID InstallComHook()
{
	ULONG_PTR dwOldAddr = 0 ;
	TCHAR strCLSID[1024]={0};
	TCHAR strDisaptchGUID[1024]={0};

	CONVERT_STR_2_GUID(strCLSID,&CLSID_Shell); //uuid(13709620-C279-11CE-A49E-444553540000)
	CONVERT_STR_2_GUID(strDisaptchGUID,&IID_IShellDispatch); //uuid(D8F015C0-C278-11CE-A49E-444553540000)
	dwOldAddr = HookComFunction(GetModuleHandle(_T("shell32.dll")),
		strCLSID,
		strDisaptchGUID,
		_T("FindFiles"),
		(ULONG_PTR)My_FindFiles);
	

	g_OriginalFindFiles = (PFN_FindFiles)dwOldAddr;

	printf("OldAddr = 0x%p\n",dwOldAddr);
}

VOID CONVERT_STR_2_GUID(LPTSTR cstr, const GUID *stGuid)
{
    _stprintf(cstr, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
    stGuid->Data1,stGuid->Data2,stGuid->Data3,
    stGuid->Data4[0],stGuid->Data4[1],stGuid->Data4[2],stGuid->Data4[3],
    stGuid->Data4[4],stGuid->Data4[5],stGuid->Data4[6],stGuid->Data4[7]);
}

ULONG_PTR* GetComProcAddrPointer( HMODULE hMod,LPCTSTR szClsGuid,LPCTSTR szDispatchGuid,LPCTSTR szFunctionName,LPSTR szErrMsg/*=NULL*/ )
{
  ULONG_PTR *pdwResult=NULL;
	
  USES_CONVERSION;
  //得到模块全路径
  TCHAR strDllFilePath[MAX_PATH]={0};
  DWORD dwSize=::GetModuleFileName(hMod,strDllFilePath,MAX_PATH);

  //载入TypeLib
  ITypeLib *pLib;
#ifdef _UNICODE
  HRESULT ret=LoadTypeLib(strDllFilePath,&pLib);
#else
  HRESULT ret=LoadTypeLib(A2W(strDllFilePath),&pLib);
#endif
  
  if (ret!=S_OK)
  {
    if(szErrMsg) _tcscpy(szErrMsg,_T("载入TypeLib失败"));
    return pdwResult;
  }

  //得到接口函数偏移
  int nFunctionOffset=-1;

  int nTypeInfoCount=pLib->GetTypeInfoCount();
  for (int i=0;i<nTypeInfoCount;i++)
  {
    ITypeInfo *pTypeInfo;
    ret=pLib->GetTypeInfo(i,&pTypeInfo);

    TYPEATTR* pta;
    ret=pTypeInfo->GetTypeAttr(&pta);

    if (pta->typekind==TKIND_DISPATCH)
    {
		TCHAR strGuid[256]={0};
		_stprintf(strGuid,_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
        pta->guid.Data1,pta->guid.Data2,pta->guid.Data3  
        ,pta->guid.Data4[0],pta->guid.Data4[1],pta->guid.Data4[2],pta->guid.Data4[3],
        pta->guid.Data4[4],pta->guid.Data4[5],pta->guid.Data4[6],pta->guid.Data4[7]);
      
	  //_tprintf(_T("比较 %s\n"),strGuid);
      if (_tcsicmp(szDispatchGuid,strGuid)==0)  //比较DispatchGuid
      {
        int nFunCount=pta->cFuncs;
        for (int j=0;j<nFunCount;j++)
        {
          FUNCDESC* pfd;
          pTypeInfo->GetFuncDesc(j, &pfd);

          BSTR bstrName;
          pTypeInfo->GetDocumentation(pfd->memid, &bstrName,NULL, NULL, NULL);
          pTypeInfo->ReleaseFuncDesc(pfd);
          CString strFunName(W2A(bstrName));
          if (strFunName.CompareNoCase(szFunctionName)==0)  //比较函数名
          {
            nFunctionOffset=j;
            break;
          }
        }
      }
    }
    pTypeInfo->ReleaseTypeAttr(pta);
    if (nFunctionOffset!=-1) break;
  }
  if (nFunctionOffset==-1)
  {
    if(szErrMsg) _tcscpy(szErrMsg,_T("查找接口函数失败"));
    return pdwResult;
  }

  typedef int (WINAPI *DllGetClassObjectPtr)(REFCLSID, REFIID, LPVOID*);

  //得到DllGetClassObject函数地址
  DllGetClassObjectPtr pDllGetClassObject = (DllGetClassObjectPtr)GetProcAddress(hMod, "DllGetClassObject");

  CoInitialize(NULL);

  CLSID rclsid;
  IID riid;

#ifdef _UNICODE
  ::CLSIDFromString((LPOLESTR)szClsGuid, &rclsid);//类ID
  ::IIDFromString((LPOLESTR)szDispatchGuid, &riid);//接口ID
#else
  ::CLSIDFromString((LPOLESTR)A2W(szClsGuid), &rclsid);//类ID
  ::IIDFromString((LPOLESTR)A2W(szDispatchGuid), &riid);//接口ID
#endif

  IClassFactory *pICF = NULL;//类工厂接口指针
  
  if(pDllGetClassObject(rclsid, IID_IClassFactory, (void**)(&pICF))==S_OK)//获取类工厂接口指针
  {
    ULONG_PTR *pClassObject=NULL;
    if(pICF->CreateInstance(NULL, riid, (void**)&pClassObject)==S_OK)//创建COM对象  
    {
      pdwResult=(ULONG_PTR *)*pClassObject+nFunctionOffset;
    }else{
      if(szErrMsg) _tcscpy(szErrMsg,_T("创建COM对象失败"));
    }
    pICF->Release();
  }else{
    if(szErrMsg) _tcscpy(szErrMsg,_T("获取类工厂接口指针失败"));
  }
  
  CoUninitialize();
  return pdwResult;
}

ULONG_PTR HookComFunction( HMODULE hMod,LPCTSTR szClsGuid,LPCTSTR szDispatchGuid,LPCTSTR szFunctionName,ULONG_PTR dwNewFunAddr )
{
	CHAR szErrMsg[64]={0};
	SIZE_T dwCnt = 0 ;
	BOOL bResult = FALSE;
	ULONG_PTR *pAddress=GetComProcAddrPointer(hMod,szClsGuid,szDispatchGuid,szFunctionName,szErrMsg);
	if (!pAddress) 
	{
		_tprintf(_T("szErr = %s\n"),szErrMsg);
		return 0 ;
	}

	printf("Pointer = 0x%p\n",pAddress);

	ULONG_PTR dwResult=*pAddress;
	printf("Original FunAddr = 0x%p\n",dwResult);

	DWORD dwOLD;
	MEMORY_BASIC_INFORMATION  mbi;
	VirtualQuery((LPCVOID)pAddress,&mbi,sizeof(mbi));
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
	*pAddress  = dwNewFunAddr ;
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	printf("Install Hook succefully.\n");
	return dwResult;
}

VOID ShellTest()
{
	HRESULT hr;
	IShellDispatch *pShellDispatch = NULL ;
	
	if (CoInitialize(NULL) != S_OK)
	{
		_tprintf(_T("CoInitialize failed!\n"));
		return ;
	}
	
	hr = CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,
		IID_IShellDispatch,(LPVOID*)&pShellDispatch);
	if (hr == S_OK)
	{
		_tprintf(_T("pShellDispatch = 0x%X\n"),pShellDispatch);
		pShellDispatch->FindFiles();
		pShellDispatch->Release();
	}
	else
	{
		_tprintf(("Create Instance failed!\n"));
	}
	
	CoUninitialize();
}

HRESULT STDMETHODCALLTYPE My_FindFiles( IShellDispatch * This)
{
	_tprintf(_T("Calling My_FindFiles:: this = 0x%p\n"),This);
	return g_OriginalFindFiles(This);
}