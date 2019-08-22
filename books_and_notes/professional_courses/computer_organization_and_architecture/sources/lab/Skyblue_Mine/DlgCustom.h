/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    DlgCustom.h

Abstract:

       the class of Dialog for setting user's favor-config

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/

#if !defined(AFX_DLGCUSTOM_H__41295931_93B3_4FC7_9ED4_809D161527F1__INCLUDED_)
#define AFX_DLGCUSTOM_H__41295931_93B3_4FC7_9ED4_809D161527F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
class CDlgCustom : public CDialog
{
public:
	void InitData(UINT xNum, UINT yNum, UINT mNum);
	CDlgCustom(CWnd* pParent = NULL);   // standard constructor
	//{{AFX_DATA(CDlgCustom)
	enum { IDD = IDD_DLG_CUSTOM };
	UINT	m_uHeight;
	UINT	m_uNumber;
	UINT	m_uWidth;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CDlgCustom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CDlgCustom)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLGCUSTOM_H__41295931_93B3_4FC7_9ED4_809D161527F1__INCLUDED_)
