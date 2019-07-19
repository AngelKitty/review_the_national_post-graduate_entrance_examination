// VulnerAX.cpp : Implementation of CVulnerAXApp and DLL registration.

#include "stdafx.h"
#include "VulnerAX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVulnerAXApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x92B6E994, 0x9F43, 0x413C, { 0x88, 0x8A, 0x94, 0xBB, 0x52, 0xEB, 0x57, 0x9C } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CVulnerAXApp::InitInstance - DLL initialization

BOOL CVulnerAXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CVulnerAXApp::ExitInstance - DLL termination

int CVulnerAXApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
