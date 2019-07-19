
// pepack64Dlg.h : 头文件
//

#pragma once


// Cpepack64Dlg 对话框
class Cpepack64Dlg : public CDialogEx
{
// 构造
public:
	Cpepack64Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY15PBPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedPack();
	CString m_strKey;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
