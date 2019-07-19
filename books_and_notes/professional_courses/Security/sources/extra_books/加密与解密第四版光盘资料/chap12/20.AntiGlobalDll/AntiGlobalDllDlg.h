// AntiGlobalDllDlg.h : header file
//

#if !defined(AFX_ANTIGLOBALDLLDLG_H__474E813B_E943_49E6_A519_45F40EA02226__INCLUDED_)
#define AFX_ANTIGLOBALDLLDLG_H__474E813B_E943_49E6_A519_45F40EA02226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAntiGlobalDllDlg dialog

class CAntiGlobalDllDlg : public CDialog
{
// Construction
public:
	CAntiGlobalDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAntiGlobalDllDlg)
	enum { IDD = IDD_ANTIGLOBALDLL_DIALOG };
	CString	m_strMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiGlobalDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAntiGlobalDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIGLOBALDLLDLG_H__474E813B_E943_49E6_A519_45F40EA02226__INCLUDED_)
