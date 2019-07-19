// MsgHookInject.h : main header file for the MSGHOOKINJECT application
//

#if !defined(AFX_MSGHOOKINJECT_H__268E12EF_B8D7_481F_80E3_78397ED637EB__INCLUDED_)
#define AFX_MSGHOOKINJECT_H__268E12EF_B8D7_481F_80E3_78397ED637EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMsgHookInjectApp:
// See MsgHookInject.cpp for the implementation of this class
//

class CMsgHookInjectApp : public CWinApp
{
public:
	CMsgHookInjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgHookInjectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMsgHookInjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGHOOKINJECT_H__268E12EF_B8D7_481F_80E3_78397ED637EB__INCLUDED_)
