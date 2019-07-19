// pediy.h : main header file for the PEDIY application
//

#if !defined(AFX_PEDIY_H__03D04109_0571_417D_852E_AAA905FCBCC0__INCLUDED_)
#define AFX_PEDIY_H__03D04109_0571_417D_852E_AAA905FCBCC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPediyApp:
// See pediy.cpp for the implementation of this class
//

class CPediyApp : public CWinApp
{
public:
	CPediyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPediyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPediyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEDIY_H__03D04109_0571_417D_852E_AAA905FCBCC0__INCLUDED_)
