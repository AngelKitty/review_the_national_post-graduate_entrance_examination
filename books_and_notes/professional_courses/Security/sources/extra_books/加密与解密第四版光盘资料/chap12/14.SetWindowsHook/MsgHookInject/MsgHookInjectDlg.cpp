// MsgHookInjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsgHookInject.h"
#include "MsgHookInjectDlg.h"

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
// CMsgHookInjectDlg dialog

CMsgHookInjectDlg::CMsgHookInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgHookInjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgHookInjectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMsgHookInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgHookInjectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgHookInjectDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgHookInjectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, OnButtonInstallHook)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL, OnButtonUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgHookInjectDlg message handlers

BOOL CMsgHookInjectDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNINSTALL)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsgHookInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMsgHookInjectDlg::OnPaint() 
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
HCURSOR CMsgHookInjectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

typedef VOID (*PFN_Func)(VOID) ;

void CMsgHookInjectDlg::OnButtonInstallHook() 
{
	// TODO: Add your control notification handler code here
#ifdef _WIN64
	HMODULE hMod = LoadLibrary("MsgHook64.dll");
#else
	HMODULE hMod = LoadLibrary("MsgHook.dll");
#endif
	if (hMod == NULL)
	{
		AfxMessageBox("¼ÓÔØMsgHook.dllÊ§°Ü!");
		return ;
	}

	PFN_Func pfnInstall = (PFN_Func)GetProcAddress(hMod,"InstallHook");
	pfnInstall();

	AfxMessageBox("Hook°²×°Íê±Ï!");

	GetDlgItem(IDC_BUTTON_INSTALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNINSTALL)->EnableWindow(TRUE);
	
}

void CMsgHookInjectDlg::OnButtonUninstall() 
{
	// TODO: Add your control notification handler code here
#ifdef _WIN64
	HMODULE hMod = LoadLibrary("MsgHook64.dll");
#else
	HMODULE hMod = LoadLibrary("MsgHook.dll");
#endif
	if (hMod == NULL)
	{
		AfxMessageBox("¼ÓÔØMsgHook.dllÊ§°Ü!");
		return ;
	}
	
	PFN_Func pfnUnInstall = (PFN_Func)GetProcAddress(hMod,"UnInstallHook");
	pfnUnInstall();
	
	AfxMessageBox("HookÐ¶ÔØÍê±Ï!");
	
	GetDlgItem(IDC_BUTTON_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNINSTALL)->EnableWindow(FALSE);
}
