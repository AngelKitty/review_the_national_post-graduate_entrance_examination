/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    DlgCustom.cpp

Abstract:

       the class of Dialog for setting user's favor-config

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/

#include "stdafx.h"
#include "mine.h"
#include "DlgCustom.h"
#include "MineWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCustom dialog


CDlgCustom::CDlgCustom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCustom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCustom)
	m_uHeight = 0;
	m_uNumber = 0;
	m_uWidth = 0;
	//}}AFX_DATA_INIT
}

void CDlgCustom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCustom)
	DDX_Text(pDX, IDC_HEIGHT, m_uHeight);
	DDX_Text(pDX, IDC_NUMBER, m_uNumber);
	DDX_Text(pDX, IDC_WIDTH, m_uWidth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCustom, CDialog)
	//{{AFX_MSG_MAP(CDlgCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////

void CDlgCustom::OnOK() 
{
	UpdateData();
	if (m_uWidth < 9) m_uWidth = 9;
	if (m_uWidth > 30) m_uWidth = 30;
	if (m_uHeight < 9) m_uHeight = 9;
	if (m_uHeight > 24) m_uHeight = 24;
	if (m_uNumber < 10) m_uNumber = 10;
	if (m_uNumber > m_uWidth * m_uHeight) m_uNumber = m_uWidth * m_uHeight - 1;
	CMineWnd *pMine = (CMineWnd*)AfxGetMainWnd();
	pMine->SetCustom(m_uWidth, m_uHeight, m_uNumber);
	
	CDialog::OnOK();
}

void CDlgCustom::InitData(UINT xNum, UINT yNum, UINT mNum)
{
	m_uWidth = xNum;
	m_uHeight = yNum;
	m_uNumber = mNum;
}
