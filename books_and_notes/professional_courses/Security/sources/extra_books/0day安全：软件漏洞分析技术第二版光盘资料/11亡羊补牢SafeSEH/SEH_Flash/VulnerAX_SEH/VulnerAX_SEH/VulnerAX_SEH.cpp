// VulnerAX_SEH.cpp : Implementation of CVulnerAX_SEHApp and DLL registration.

#include "stdafx.h"
#include "VulnerAX_SEH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVulnerAX_SEHApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x2102398C, 0x45B6, 0x4276, { 0x98, 0x59, 0xCE, 0x35, 0xB3, 0xF, 0x52, 0x5A } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CVulnerAX_SEHApp::InitInstance - DLL initialization

BOOL CVulnerAX_SEHApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CVulnerAX_SEHApp::ExitInstance - DLL termination

int CVulnerAX_SEHApp::ExitInstance()
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
