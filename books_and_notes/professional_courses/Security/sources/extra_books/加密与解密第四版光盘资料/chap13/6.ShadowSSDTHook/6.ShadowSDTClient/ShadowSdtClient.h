// ShadowSdtClient.h : main header file for the SHADOWSDTCLIENT application
//

#if !defined(AFX_SHADOWSDTCLIENT_H__11FDC97E_2AE7_4B5F_8EB3_824085BEE12D__INCLUDED_)
#define AFX_SHADOWSDTCLIENT_H__11FDC97E_2AE7_4B5F_8EB3_824085BEE12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CShadowSdtClientApp:
// See ShadowSdtClient.cpp for the implementation of this class
//

class CShadowSdtClientApp : public CWinApp
{
public:
	CShadowSdtClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowSdtClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShadowSdtClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWSDTCLIENT_H__11FDC97E_2AE7_4B5F_8EB3_824085BEE12D__INCLUDED_)
