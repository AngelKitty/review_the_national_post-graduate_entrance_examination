// IMEInstallerDlg.h : header file
//

#if !defined(AFX_IMEINSTALLERDLG_H__E8D25BDF_B058_45A1_AD80_7DFD8C8D318E__INCLUDED_)
#define AFX_IMEINSTALLERDLG_H__E8D25BDF_B058_45A1_AD80_7DFD8C8D318E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIMEInstallerDlg dialog

class CIMEInstallerDlg : public CDialog
{
// Construction
public:
	HKL m_hImeFile;
	ULONG m_retV;
	CIMEInstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIMEInstallerDlg)
	enum { IDD = IDD_IMEINSTALLER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIMEInstallerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIMEInstallerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInstall();
	afx_msg void OnButtonUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMEINSTALLERDLG_H__E8D25BDF_B058_45A1_AD80_7DFD8C8D318E__INCLUDED_)
