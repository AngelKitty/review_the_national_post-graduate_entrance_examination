// MsgHookInjectDlg.h : header file
//

#if !defined(AFX_MSGHOOKINJECTDLG_H__F7F41DE5_E79B_4F63_946A_831E62BAFC04__INCLUDED_)
#define AFX_MSGHOOKINJECTDLG_H__F7F41DE5_E79B_4F63_946A_831E62BAFC04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMsgHookInjectDlg dialog

class CMsgHookInjectDlg : public CDialog
{
// Construction
public:
	CMsgHookInjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgHookInjectDlg)
	enum { IDD = IDD_MSGHOOKINJECT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgHookInjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMsgHookInjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInstallHook();
	afx_msg void OnButtonUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGHOOKINJECTDLG_H__F7F41DE5_E79B_4F63_946A_831E62BAFC04__INCLUDED_)
