/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    DlgNewRecord.h

Abstract:

       the class of Dialog for recording the best sore of expert users,
	   by pop up way.

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn


Revision History:      1.0

--*/
#if !defined(AFX_DLGNEWRECORD_H__7948FA27_5DED_4553_9933_EC2B38F7256E__INCLUDED_)
#define AFX_DLGNEWRECORD_H__7948FA27_5DED_4553_9933_EC2B38F7256E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
class CDlgNewRecord : public CDialog
{
public:
	CDlgNewRecord(CWnd* pParent = NULL);   // standard constructor
	void SetLevel(UINT level);
	CString GetName();
	//{{AFX_DATA(CDlgNewRecord)
	enum { IDD = IDD_DLG_NEWRECORD };
	CString	m_szName;
	CString	m_szDesc;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CDlgNewRecord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CDlgNewRecord)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLGNEWRECORD_H__7948FA27_5DED_4553_9933_EC2B38F7256E__INCLUDED_)
