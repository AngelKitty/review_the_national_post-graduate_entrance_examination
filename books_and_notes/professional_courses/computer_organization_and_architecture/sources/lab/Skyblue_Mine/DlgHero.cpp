/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    DlgHero.cpp

Abstract:

       the class of Dialog for showing the best sore of expert users.

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/

#include "stdafx.h"
#include "mine.h"
#include "DlgHero.h"
#include "MineWnd.h"
#include "MineDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CDlgHero::CDlgHero(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHero::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHero)
	m_szBHolder = _T("");
	m_szEHolder = _T("");
	m_szBRecord = _T("");
	m_szERecord = _T("");
	m_szIHolder = _T("");
	m_szIRecord = _T("");
	//}}AFX_DATA_INIT
}

void CDlgHero::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHero)
	DDX_Text(pDX, IDC_B_H, m_szBHolder);
	DDX_Text(pDX, IDC_E_H, m_szEHolder);
	DDX_Text(pDX, IDC_B_R, m_szBRecord);
	DDX_Text(pDX, IDC_E_R, m_szERecord);
	DDX_Text(pDX, IDC_I_H, m_szIHolder);
	DDX_Text(pDX, IDC_I_R, m_szIRecord);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgHero, CDialog)
	//{{AFX_MSG_MAP(CDlgHero)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////

void CDlgHero::SetBHolder(CString holder)
{
	m_szBHolder = holder;
}

void CDlgHero::SetBRecord(UINT record)
{
	m_szBRecord.Format("%d취", record);
}

void CDlgHero::SetIHolder(CString holder)
{
	m_szIHolder = holder;
}

void CDlgHero::SetIRecord(UINT record)
{
	m_szIRecord.Format("%d취", record);
}

void CDlgHero::SetEHolder(CString holder)
{
	m_szEHolder = holder;
}

void CDlgHero::SetERecord(UINT record)
{
	m_szERecord.Format("%d취", record);
}

void CDlgHero::OnReset() 
{
	CMineWnd *pMine = (CMineWnd*)AfxGetMainWnd();
	pMine->ResetRecord();
	m_szBHolder = DEFAULT_HOLDER;
	m_szBRecord.Format("%d취", DEFAULT_RECORD);
	m_szIHolder = DEFAULT_HOLDER;
	m_szIRecord.Format("%d취", DEFAULT_RECORD);
	m_szEHolder = DEFAULT_HOLDER;
	m_szERecord.Format("%d취", DEFAULT_RECORD);
	UpdateData(FALSE);
}
