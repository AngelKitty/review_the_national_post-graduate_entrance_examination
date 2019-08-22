#if !defined(AFX_MINE_H__009533F8_ACB7_4BD3_A0BA_A8D5925D624D__INCLUDED_)
#define AFX_MINE_H__009533F8_ACB7_4BD3_A0BA_A8D5925D624D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MineWnd.h"

/////////////////////////////////////////////////////////////////////////////
class CMineApp : public CWinApp
{
public:
	CMineApp();
	CMineWnd *m_pMineWnd;
	//{{AFX_VIRTUAL(CMineApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMineApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT m_uXMineNum;		
	UINT m_uYMineNum;		
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_MINE_H__009533F8_ACB7_4BD3_A0BA_A8D5925D624D__INCLUDED_)
