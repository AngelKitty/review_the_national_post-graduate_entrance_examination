#include "stdafx.h"
#include "Mine.h"
#include "MineDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMineApp, CWinApp)
	//{{AFX_MSG_MAP(CMineApp)
	//}}AFX_MSG
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
CMineApp::CMineApp()
{
}
/////////////////////////////////////////////////////////////////////////////
CMineApp theApp;

const CString className = _T("MineClass");
/////////////////////////////////////////////////////////////////////////////
BOOL CMineApp::InitInstance()
{
	SetRegistryKey("CSER_513_2");
	m_uXMineNum = GetProfileInt(GAME_SETTING, CUSTOM_XNUM, PRIMARY_XNUM);
	m_uYMineNum = GetProfileInt(GAME_SETTING, CUSTOM_YNUM, PRIMARY_YNUM);

	//定义并注册MineWnd窗口
	m_pMineWnd = new CMineWnd();
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(IDC_ARROW);
	wc.hIcon = LoadIcon(IDR_MAINFRAME);
	wc.lpszClassName = className;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	// register the minewnd class
	//注册窗口
	if (!AfxRegisterClass(&wc)) 
	{
		AfxMessageBox("注册类失败！");
		return FALSE;
	}

	// get the minewnd coordinates, width and height
	UINT uWidth = DEFAULT_FRAME_X + m_uXMineNum * MINE_WIDTH +
				LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 + SIDE_WIDTH_1;
	UINT uHeight = DEFAULT_FRAME_Y + m_uYMineNum * MINE_HEIGHT + 
				LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 * 2 + SIDE_WIDTH_1 + SHELL_S_H;
	UINT uXPos = GetSystemMetrics(SM_CXSCREEN) / 2 - uWidth / 2;
	UINT uYPos = GetSystemMetrics(SM_CYSCREEN) / 2 - uHeight / 2;
	//创建窗口
	if ( !m_pMineWnd->CreateEx( NULL, className, _T("扫雷"), 
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
		uXPos, uYPos, uWidth, uHeight,
		NULL, NULL) ) 
	{
		AfxMessageBox("创建主窗口失败！");
		return FALSE;
	}
	// 绑定该窗口为主窗口
	m_pMainWnd = m_pMineWnd;
	//显示窗口
	m_pMineWnd->ShowWindow(SW_NORMAL);
	return TRUE;
}

int CMineApp::ExitInstance() 
{
	delete m_pMineWnd;
	
	return CWinApp::ExitInstance();
}


