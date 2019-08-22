/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    DlgHero.h

Abstract:

       the class of Dialog for showing the best sore of expert users.

Author:

    Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/
#if !defined(AFX_DLGHERO_H__650E97B9_97EB_476A_A11A_A4CB3F41917E__INCLUDED_)
#define AFX_DLGHERO_H__650E97B9_97EB_476A_A11A_A4CB3F41917E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
class CDlgHero : public CDialog
{
public:
	void SetBHolder(CString holder);
	void SetBRecord(UINT record);
	void SetIHolder(CString holder);
	void SetIRecord(UINT record);
	void SetEHolder(CString holder);
	void SetERecord(UINT record);
	CDlgHero(CWnd* pParent = NULL);   // standard constructor
	//{{AFX_DATA(CDlgHero)
	enum { IDD = IDD_DLG_HERO };
	CString	m_szBHolder;
	CString	m_szEHolder;
	CString	m_szBRecord;
	CString	m_szERecord;
	CString	m_szIHolder;
	CString	m_szIRecord;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgHero)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CDlgHero)
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLGHERO_H__650E97B9_97EB_476A_A11A_A4CB3F41917E__INCLUDED_)
