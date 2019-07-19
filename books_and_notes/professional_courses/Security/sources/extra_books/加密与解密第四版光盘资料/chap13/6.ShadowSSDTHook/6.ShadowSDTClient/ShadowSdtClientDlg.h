// ShadowSdtClientDlg.h : header file
//

#if !defined(AFX_SHADOWSDTCLIENTDLG_H__750E61E9_26BA_4B7E_BDA0_C37C3016D6E3__INCLUDED_)
#define AFX_SHADOWSDTCLIENTDLG_H__750E61E9_26BA_4B7E_BDA0_C37C3016D6E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShadowSdtClientDlg dialog

class CShadowSdtClientDlg : public CDialog
{
// Construction
public:
	BOOL SetProtectSwitch(BOOL bProtectOn);
	HANDLE m_hDevice;
	VOID OpenDeviceHandle();
	VOID AppendMessage(LPCTSTR lpMsg);
	BOOL SetTrustProcess();
	CShadowSdtClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CShadowSdtClientDlg)
	enum { IDD = IDD_SHADOWSDTCLIENT_DIALOG };
	CString	m_strMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowSdtClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShadowSdtClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonEnableProt();
	virtual void OnCancel();
	afx_msg void OnButtonDisableProt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWSDTCLIENTDLG_H__750E61E9_26BA_4B7E_BDA0_C37C3016D6E3__INCLUDED_)
