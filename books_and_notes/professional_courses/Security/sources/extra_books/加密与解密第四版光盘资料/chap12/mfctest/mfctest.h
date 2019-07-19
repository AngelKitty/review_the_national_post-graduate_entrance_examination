// mfctest.h : main header file for the MFCTEST application
//

#if !defined(AFX_MFCTEST_H__952D6238_6259_4A8C_9F18_B8378A687EA9__INCLUDED_)
#define AFX_MFCTEST_H__952D6238_6259_4A8C_9F18_B8378A687EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfctestApp:
// See mfctest.cpp for the implementation of this class
//

class CMfctestApp : public CWinApp
{
public:
	CMfctestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfctestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMfctestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCTEST_H__952D6238_6259_4A8C_9F18_B8378A687EA9__INCLUDED_)
