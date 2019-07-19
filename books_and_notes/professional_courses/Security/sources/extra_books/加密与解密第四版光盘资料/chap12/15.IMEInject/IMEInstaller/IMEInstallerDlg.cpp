// IMEInstallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMEInstaller.h"
#include "IMEInstallerDlg.h"
#include <imm.h>

#pragma comment(lib,"imm32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEInstallerDlg dialog

CIMEInstallerDlg::CIMEInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIMEInstallerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIMEInstallerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMEInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIMEInstallerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIMEInstallerDlg, CDialog)
	//{{AFX_MSG_MAP(CIMEInstallerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, OnButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL, OnButtonUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEInstallerDlg message handlers

BOOL CIMEInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIMEInstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIMEInstallerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIMEInstallerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//参考了 http://blog.csdn.net/swanabin/article/details/21473045
void CIMEInstallerDlg::OnButtonInstall() 
{
	// TODO: Add your control notification handler code here
	//得到默认的输入法句柄并保存  
    SystemParametersInfo(SPI_GETDEFAULTINPUTLANG,0,&m_retV, 0); 
	
	//装载输入法，必须放在system32目录下，否则会找不到文件
	//这里写全路径也没有用，因为系统加载的时候只会根据文件名查找
    m_hImeFile = ImmInstallIME(
		"C:\\Windows\\System32\\HackerIME.ime",  
        "黑客输入法");  
	
	DWORD dwErr = GetLastError();
	CString msg;
    if( ImmIsIME(m_hImeFile) )  
    {  
        //设置为默认输入法  
        SystemParametersInfo(SPI_SETDEFAULTINPUTLANG,0,  &m_hImeFile,  SPIF_SENDWININICHANGE);  
        MessageBox(_T("安装输入法成功"));  
    }
	else
	{
		msg.Format("安装失败! Err = %d",dwErr);
		MessageBox(msg);
	}
}

void CIMEInstallerDlg::OnButtonUninstall() 
{
	// TODO: Add your control notification handler code here
	::SystemParametersInfo(  
        SPI_SETDEFAULTINPUTLANG,  
        0,  
        &m_retV,  
        SPIF_SENDWININICHANGE);  
    if (UnloadKeyboardLayout(m_hImeFile))  
    {  
        MessageBox(_T("输入法卸载成功"));  
    }  
}
