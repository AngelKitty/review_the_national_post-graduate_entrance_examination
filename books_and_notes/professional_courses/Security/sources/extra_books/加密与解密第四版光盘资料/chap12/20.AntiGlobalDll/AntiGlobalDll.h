// AntiGlobalDll.h : main header file for the ANTIGLOBALDLL application
//

#if !defined(AFX_ANTIGLOBALDLL_H__D1AD6A66_DDA6_4297_89AD_9C3D5E1E93A6__INCLUDED_)
#define AFX_ANTIGLOBALDLL_H__D1AD6A66_DDA6_4297_89AD_9C3D5E1E93A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAntiGlobalDllApp:
// See AntiGlobalDll.cpp for the implementation of this class
//

class CAntiGlobalDllApp : public CWinApp
{
public:
	CAntiGlobalDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiGlobalDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAntiGlobalDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIGLOBALDLL_H__D1AD6A66_DDA6_4297_89AD_9C3D5E1E93A6__INCLUDED_)
