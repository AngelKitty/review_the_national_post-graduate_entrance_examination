#pragma once

// VulnerAX_SEH.h : main header file for VulnerAX_SEH.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols


// CVulnerAX_SEHApp : See VulnerAX_SEH.cpp for implementation.

class CVulnerAX_SEHApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

