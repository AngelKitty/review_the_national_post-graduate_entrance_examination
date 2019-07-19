// mfctestDlg.h : header file
//

#if !defined(AFX_MFCTESTDLG_H__636E7B1D_BB6D_451D_9241_EDAADA31808F__INCLUDED_)
#define AFX_MFCTESTDLG_H__636E7B1D_BB6D_451D_9241_EDAADA31808F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMfctestDlg dialog

class CMfctestDlg : public CDialog
{
// Construction
public:
	CMfctestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMfctestDlg)
	enum { IDD = IDD_MFCTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfctestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMfctestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCTESTDLG_H__636E7B1D_BB6D_451D_9241_EDAADA31808F__INCLUDED_)
