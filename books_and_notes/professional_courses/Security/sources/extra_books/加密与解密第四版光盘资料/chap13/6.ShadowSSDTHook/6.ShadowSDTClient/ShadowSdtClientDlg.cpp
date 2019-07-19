// ShadowSdtClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowSdtClient.h"
#include "ShadowSdtClientDlg.h"
#include "../ShadowSDTHook/ShadowSDTHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// ClientId
//

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

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
// CShadowSdtClientDlg dialog

CShadowSdtClientDlg::CShadowSdtClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShadowSdtClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShadowSdtClientDlg)
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShadowSdtClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShadowSdtClientDlg)
	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShadowSdtClientDlg, CDialog)
	//{{AFX_MSG_MAP(CShadowSdtClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, OnButtonEnableProt)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, OnButtonDisableProt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowSdtClientDlg message handlers

BOOL CShadowSdtClientDlg::OnInitDialog()
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
	OpenDeviceHandle();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShadowSdtClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CShadowSdtClientDlg::OnPaint() 
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
HCURSOR CShadowSdtClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CShadowSdtClientDlg::SetTrustProcess()
{
	BOOL bResult = FALSE ;
	DWORD dwIoCnt = 0 ;
	CLIENT_ID Cid;
	CString strMsg;

	Cid.UniqueProcess = (HANDLE)GetCurrentProcessId();
	Cid.UniqueThread  = (HANDLE)GetCurrentThreadId();


	bResult = DeviceIoControl(m_hDevice,IOCTL_SHADOWSDTHOOK_SETTRUSTPROC,&Cid,sizeof(CLIENT_ID),NULL,0,&dwIoCnt,NULL);
	if (bResult)
	{
		AppendMessage("成功设置当前进程为被保护进程!");
		bResult = TRUE;
	}
	else
	{
		strMsg.Format("设置当前进程为被保护进程时失败! err=%d",GetLastError());
		AppendMessage(strMsg);
	}
	
	return bResult;
}

VOID CShadowSdtClientDlg::AppendMessage(LPCTSTR lpMsg)
{
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format("[%02d-%02d %02d:%02d:%02d.%03d] ",st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	GetDlgItemText(IDC_EDIT_MSG,m_strMsg);
	m_strMsg += strTime;
	m_strMsg += lpMsg;
	m_strMsg += "\r\n";
	SetDlgItemText(IDC_EDIT_MSG,m_strMsg);

	int len = m_strMsg.GetLength();
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MSG);
	pEdit->SetSel(len,len);
}

void CShadowSdtClientDlg::OnButtonEnableProt() 
{
	// TODO: Add your control notification handler code here
	BOOL bResult = FALSE ;
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		SetTrustProcess();
		bResult = SetProtectSwitch(TRUE);
		if (bResult)
		{
			GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DISABLE)->EnableWindow(TRUE);
		}
	}
	else
	{
		AppendMessage("设备句柄无效，请确认驱动已经加载。");
	}
	
}

void CShadowSdtClientDlg::OnButtonDisableProt() 
{
	// TODO: Add your control notification handler code here
	BOOL bResult = FALSE;
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		bResult = SetProtectSwitch(FALSE);
		if (bResult)
		{
			GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DISABLE)->EnableWindow(FALSE);
		}
	}
	else
	{
		AppendMessage("设备句柄无效，请确认驱动已经加载。");
	}
}


VOID CShadowSdtClientDlg::OpenDeviceHandle()
{
	CString strMsg;
	m_hDevice = CreateFile(SHADOWSDTHOOK_WIN32_DEVICE_NAME,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hDevice == INVALID_HANDLE_VALUE)
	{
		strMsg.Format("打开设备失败! errCode = %d",GetLastError());
		AppendMessage(strMsg);
	}
	else
	{
		AppendMessage("设备打开成功，准备就绪.");
	}
	
}

BOOL CShadowSdtClientDlg::SetProtectSwitch(BOOL bProtectOn)
{
	BOOL bResult = FALSE ;
	DWORD dwIoCnt = 0 ;
	CString strMsg;
	DWORD dwIoCtrlCode = 0 ;
	
	if (bProtectOn)
	{
		dwIoCtrlCode = IOCTL_SHADOWSDTHOOK_ENABLEPROT;
	}
	else
	{
		dwIoCtrlCode = IOCTL_SHADOWSDTHOOK_DISABLEPROT;
	}
	
	
	bResult = DeviceIoControl(m_hDevice,dwIoCtrlCode,NULL,0,NULL,0,&dwIoCnt,NULL);
	if (!bResult)
	{
		strMsg.Format("设置保护开关时失败! err=%d",GetLastError());
		AppendMessage(strMsg);
	}
	else
	{
		bResult = TRUE ;
		if (bProtectOn)
		{
			AppendMessage("成功打开保护开关!");
		}
		else
		{
			AppendMessage("成功关闭保护开关!");
		}
	}
	
	return bResult;
}

void CShadowSdtClientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}

	CDialog::OnCancel();
}

