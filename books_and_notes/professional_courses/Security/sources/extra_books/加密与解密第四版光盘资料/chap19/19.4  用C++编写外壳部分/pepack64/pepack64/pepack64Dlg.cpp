
// pepack64Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pepack64.h"
#include "pepack64Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cpepack64Dlg 对话框



Cpepack64Dlg::Cpepack64Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cpepack64Dlg::IDD, pParent)
	, m_strPath(_T(""))
	, m_strKey(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cpepack64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_TEXT, m_strPath);
	DDX_Text(pDX, EDT_KEY, m_strKey);
}

BEGIN_MESSAGE_MAP(Cpepack64Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_OPENFILE, &Cpepack64Dlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(BTN_PACK, &Cpepack64Dlg::OnBnClickedPack)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// Cpepack64Dlg 消息处理程序

BOOL Cpepack64Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_strKey = L"15";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cpepack64Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cpepack64Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cpepack64Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cpepack64Dlg::OnBnClickedOpenfile()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	static TCHAR BASED_CODE szFilter[] = _T("可执行文件 (*.exe)|*.exe|")
		_T("DLL文件 (*.dll)|*.dll|exe Files (*.exe;*.exe)|")
		_T("*.exe; *.exe|All Files (*.*)|*.*||");

	CFileDialog fileDlg(TRUE, _T("my"), _T(""),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);

	fileDlg.DoModal();

	m_strPath = fileDlg.GetPathName();

	UpdateData(FALSE);
}

#include "../Pack/Pack.h"
#ifdef DEBUG
#pragma comment(lib,"../x64/Debug/Pack.lib")
#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib,"../x64/release/Pack.lib")
#endif // NDEBUG


void Cpepack64Dlg::OnBnClickedPack()
{
	//将控件的信息更新到变量中
	UpdateData(TRUE);
	// 获取加密Key的文本
	TCHAR szKey[20] = { 0 };
	_tcscpy_s(szKey, 20, m_strKey.GetBuffer());
	ULONGLONG nKey = 0;
	// 转换key为16进制
	_stscanf_s(szKey, L"%p", &nKey);
	// 调用Pack模块
	if (Pack(m_strPath, (BYTE)nKey))
	{
		AfxMessageBox(L"加壳成功");
	}
	//将变量中的信息更新到控件中
	UpdateData(FALSE);
}

void Cpepack64Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	TCHAR szFilePath[MAX_PATH+2] = {0};
	DragQueryFile(hDropInfo, 0, szFilePath, _MAX_PATH);  //获取拖拽的文件名
	DragFinish(hDropInfo);
	m_strPath = szFilePath;  // 赋值给成员变量
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}
