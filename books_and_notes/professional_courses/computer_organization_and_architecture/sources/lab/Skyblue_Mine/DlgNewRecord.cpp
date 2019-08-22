/*++

Copyright (c) 2004-2005  Micro-soft  

Module Name:

    DlgNewRecord.cpp

Abstract:

       the class of Dialog for recording the best sore of expert users.
		by pop up way.

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/
#include "stdafx.h"
#include "mine.h"
#include "DlgNewRecord.h"
#include "MineDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CDlgNewRecord::CDlgNewRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewRecord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewRecord)
	m_szName = _T("匿名");
	m_szDesc = _T("");
	//}}AFX_DATA_INIT
}

void CDlgNewRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewRecord)
	DDX_Text(pDX, IDC_EDIT_NAME, m_szName);
	DDX_Text(pDX, IDC_DESCRIBE, m_szDesc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgNewRecord, CDialog)
	//{{AFX_MSG_MAP(CDlgNewRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CDlgNewRecord::SetLevel(UINT level)
{
	CString szLevel;
	if (level == LEVEL_PRIMARY) szLevel = "初级";
	else if (level == LEVEL_SECONDRY) szLevel = "中级";
	else if (level == LEVEL_ADVANCE) szLevel = "高级";
	m_szDesc.Format("已破%s记录\n\r请留尊姓大名", szLevel);
}

CString CDlgNewRecord::GetName()
{
	return m_szName;
}
