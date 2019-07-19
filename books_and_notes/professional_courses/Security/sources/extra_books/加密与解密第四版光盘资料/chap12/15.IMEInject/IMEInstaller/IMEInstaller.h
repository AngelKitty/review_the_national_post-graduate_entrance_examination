// IMEInstaller.h : main header file for the IMEINSTALLER application
//

#if !defined(AFX_IMEINSTALLER_H__9D298E3B_69A2_40A9_910C_C02F6052DB86__INCLUDED_)
#define AFX_IMEINSTALLER_H__9D298E3B_69A2_40A9_910C_C02F6052DB86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIMEInstallerApp:
// See IMEInstaller.cpp for the implementation of this class
//

class CIMEInstallerApp : public CWinApp
{
public:
	CIMEInstallerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIMEInstallerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIMEInstallerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMEINSTALLER_H__9D298E3B_69A2_40A9_910C_C02F6052DB86__INCLUDED_)
