// HostProc.h : main header file for the HOSTPROC application
//

#if !defined(AFX_HOSTPROC_H__0585D0B6_0E79_4BEA_91E2_989EE2EC6A9D__INCLUDED_)
#define AFX_HOSTPROC_H__0585D0B6_0E79_4BEA_91E2_989EE2EC6A9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHostProcApp:
// See HostProc.cpp for the implementation of this class
//

class CHostProcApp : public CWinApp
{
public:
	CHostProcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostProcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHostProcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTPROC_H__0585D0B6_0E79_4BEA_91E2_989EE2EC6A9D__INCLUDED_)
