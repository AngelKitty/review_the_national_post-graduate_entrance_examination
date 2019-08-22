/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    MineWnd.cpp

Abstract:

       the mian Frame of the Mine-Game, we should do the most draw-work here,
	surely,the size and style of the window should set here too. 

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/
#include "stdafx.h"
#include "Mine.h"
#include "MineWnd.h"
#include "MineDefs.h"
#include "Mmsystem.h"
#include "DlgNewRecord.h"
#include "DlgHero.h"
#include "DlgCustom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMineApp theApp;
#define ID_TIMER_EVENT	WM_USER + 1
/////////////////////////////////////////////////////////////////////////////

CMineWnd::CMineWnd()
{
	m_pSndDead = NULL;
	m_pSndVictory = NULL;
	m_pSndClock = NULL;
	m_uTimer = 0;
	m_brsBG.CreateSolidBrush(COLOR_GRAY);
	
	LoadConfig();
	if (m_bSoundful) LoadWaveSrc();
	InitGame();
}

CMineWnd::~CMineWnd()
{
	FreeMines();
	FreeWaveSrc();
	SaveConfig();
}

BEGIN_MESSAGE_MAP(CMineWnd, CWnd)
//{{AFX_MSG_MAP(CMineWnd)
ON_WM_PAINT()
ON_WM_SHOWWINDOW()
ON_WM_TIMER()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_COMMAND(IDM_START, OnMemuStart)
ON_COMMAND(IDM_PRIMARY, OnMemuPrimary)
ON_COMMAND(IDM_SECOND, OnMemuSecond)
ON_COMMAND(IDM_ADVANCE, OnMemuAdvance)
ON_COMMAND(IDM_CUSTOM, OnMemuCustom)
ON_COMMAND(IDM_MARK, OnMemuMark)
ON_COMMAND(IDM_COLOR, OnMemuColor)
ON_COMMAND(IDM_SOUND, OnMemuSound)
ON_COMMAND(IDM_EXIT, OnMemuExit)
ON_COMMAND(IDM_HELP_LIST, OnMemuHelpList)
ON_COMMAND(IDM_HELP_FIND, OnMemuHelpFind)
ON_COMMAND(IDM_HELP_USE, OnMemuHelpUse)
ON_COMMAND(IDM_ABOUT, OnMemuAbout)
ON_WM_KEYDOWN()
ON_WM_INITMENU()
ON_WM_CLOSE()
ON_COMMAND(IDM_HERO, OnMemuHero)
ON_COMMAND(IDM_CHEAT, OnMemuCheat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////

//
//载入配置
//
void CMineWnd::LoadConfig()
{
	//----------获取本地目录-------------*/
	char exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = CString(exeFullPath);
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	//-----------------------------------*/	
	strPath += "\\config.ini";
	
		char strPrimary[80];
		char strSecond[80];
		char strAdvance[80];
	
	m_uXNum	= GetPrivateProfileInt("WINDOW","XNum", 10,strPath);
	m_uYNum	= GetPrivateProfileInt("WINDOW","YNum", 10,strPath);
	
	m_uMineNum = GetPrivateProfileInt("MINE","MineNum", 10,strPath);
	m_uLevel = GetPrivateProfileInt("MINE","Level", 0,strPath);
		
	m_uPrimary = GetPrivateProfileInt("SCORE","Primary", 999,strPath);
	m_uSecond  = GetPrivateProfileInt("SCORE","Second", 999,strPath);
	m_uAdvance = GetPrivateProfileInt("SCORE","Advance", 999,strPath);

	GetPrivateProfileString("HOLDER","PrimaryHolder", "匿名",strPrimary,80,strPath);
	GetPrivateProfileString("HOLDER","SecondHolder", "匿名",strSecond,80,strPath);
	GetPrivateProfileString("HOLDER","AdvanceHolder", "匿名",strAdvance,80,strPath);

	m_szPrimary = strPrimary;
	m_szSecond = strSecond;
	m_szAdvance  = strAdvance;

	m_bMarkful	= GetPrivateProfileInt("CONFIG","Markful", 0,strPath);
	m_bColorful = GetPrivateProfileInt("CONFIG","Colorful", 1,strPath);
	m_bSoundful = GetPrivateProfileInt("CONFIG","Soundful", 1,strPath);
}

//
// 保存
//
void CMineWnd::SaveConfig()
{

	//----------获取本地目录-------------*/
	char exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = CString(exeFullPath);
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	//-----------------------------------*/	
	strPath += "\\config.ini";
	
	
	//雷区行列数
	   CString strXNum,strYNum;
	   //雷数 选择级别
	   CString strMineNum,strLevel;
	   //记录保持分数
	   CString strPrimary,strSecond,strAdvance;
	   //记录保持者
	   CString strPrimaryHolder,strSecondHolder,strAdvanceHolder;
	   //其他配置
	   CString strMarkful,strColorful,strSoundful;
	   
	   
	   strXNum.Format("%u",m_uXNum);
	   strYNum.Format("%u",m_uYNum);
	   
	   strMineNum.Format("%u",m_uMineNum);
	   strLevel.Format("%u",m_uLevel);
	   
	   strPrimary.Format("%u",m_uPrimary);
	   strSecond.Format("%u",m_uSecond);
	   strAdvance.Format("%u",m_uAdvance);
	   
	   strPrimaryHolder.Format("%s",m_szPrimary);
	   strSecondHolder.Format("%s",m_szSecond);
	   strAdvanceHolder.Format("%s",m_szAdvance);
	   
	   strMarkful.Format("%d",m_bMarkful);
	   strColorful.Format("%d",m_bColorful);
	   strSoundful.Format("%d",m_bSoundful);
	   
	   
	   WritePrivateProfileString("WINDOW","XNum", strXNum,strPath);
	   WritePrivateProfileString("WINDOW","YNum", strYNum,strPath);
	   
	   WritePrivateProfileString("MINE","MineNum", strMineNum,strPath);
	   WritePrivateProfileString("MINE","Level", strLevel,strPath);
	   
	   WritePrivateProfileString("SCORE","Primary", strPrimary,strPath);
	   WritePrivateProfileString("SCORE","Second", strSecond,strPath);
	   WritePrivateProfileString("SCORE","Advance", strAdvance,strPath);
	   
	   WritePrivateProfileString("HOLDER","PrimaryHolder", strPrimaryHolder,strPath);
	   WritePrivateProfileString("HOLDER","SecondHolder", strSecondHolder,strPath);
	   WritePrivateProfileString("HOLDER","AdvanceHolder", strAdvanceHolder,strPath);
	   
	   WritePrivateProfileString("CONFIG","Markful", strMarkful,strPath);
	   WritePrivateProfileString("CONFIG","Colorful", strColorful,strPath);
	   WritePrivateProfileString("CONFIG","Soundful", strSoundful,strPath);
	   
}

//
//载入位图
//
void CMineWnd::LoadBitmap()
{
	if (m_bColorful)
	{
		m_clrDark = COLOR_DARK_GRAY;
		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);
		m_bmpNumber.DeleteObject();
		m_bmpNumber.LoadBitmap(IDB_NUM_COLOR);
		m_bmpButton.DeleteObject();
		m_bmpButton.LoadBitmap(IDB_BTN_COLOR);
	}
	else 
	{
		m_clrDark = COLOR_BLACK;
		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);
		m_bmpNumber.DeleteObject();
		m_bmpNumber.LoadBitmap(IDB_NUM_GRAY);
		m_bmpButton.DeleteObject();
		m_bmpButton.LoadBitmap(IDB_BTN_GRAY);
	}
}

//
//  游戏的初始化
//
void CMineWnd::InitGame()
{
	//位图的初始化
	LoadBitmap();
	m_nLeaveNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = BUTTON_NORMAL;
	m_uGameState = GS_WAIT;
	
	if (m_uTimer) 
	{
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}
	m_pNewMine = NULL;
	m_pOldMine = NULL;
	
	FreeMines();
	//初始化地图
	for (UINT i = 0; i<m_uYNum; i++) 
	{
		for (UINT j = 0; j<m_uXNum; j++)
		{
			m_pMines[i][j].uRow = i;
			m_pMines[i][j].uCol = j;
			m_pMines[i][j].uState = STATE_NORMAL;
			m_pMines[i][j].uAttrib = ATTRIB_EMPTY;
			m_pMines[i][j].uOldState = STATE_NORMAL;
		}
	}
}

//
// 布雷
//
void CMineWnd::LayMines(UINT row, UINT col)
{
//埋下随机种子
	srand( (unsigned)time( NULL ) );
	UINT i, j;
	
	for(UINT index = 0; index < m_uMineNum;) 
	{	
		//取随即数
		i = rand() % m_uYNum;
		j = rand() % m_uXNum;

		if (i == row && j == col) continue;
		if(m_pMines[i][j].uAttrib != ATTRIB_MINE) 
		{
			m_pMines[i][j].uAttrib = ATTRIB_MINE;//修改属性为雷

			index++;
		}
	}	
}

//
//雷方块拓展(对于周围无雷的空白区域)
//
void CMineWnd::ExpandMines(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	UINT around = GetAroundNum(row, col);

	
	m_pMines[row][col].uState = 15 - around;
	m_pMines[row][col].uOldState = 15 - around;

	// “打开”该区域，重绘
	DrawSpecialMine(row, col);

	//对周围一个雷都没有的空白区域
	if (around == 0)
	{
		for (i = minRow; i < maxRow; i++) 
		{
			for (j = minCol; j < maxCol; j++) 
			{//对于周围可以拓展的区域进行的规拓展			
				if (!(i == row && j == col) && 
					m_pMines[i][j].uState == STATE_NORMAL
					&& m_pMines[i][j].uAttrib != ATTRIB_MINE) 
				{

					if (!IsInMineArea(i, j)) continue;
					ExpandMines(i, j);
				}
			}
		}
	}
}


//
//  获取某个小方块区域相邻8个区域的雷个数
//
UINT CMineWnd::GetAroundNum(UINT row, UINT col)
{
	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;

	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uAttrib == ATTRIB_MINE) around++;
		}
	}
	return around;
}

//
//  获取某个小方块区域相邻8个区域的已标志状态数
//
UINT CMineWnd::GetAroundFlags(UINT row, UINT col)
{
	UINT i, j;
	UINT flags = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG) flags++;
		}
	}
	return flags;
}
//
//判断是否为雷
//
BOOL CMineWnd::IsMine(UINT row, UINT col)
{
	return (m_pMines[row][col].uAttrib == ATTRIB_MINE);
}

//判断是否在雷区域
BOOL CMineWnd::IsInMineArea(UINT row, UINT col)
{
	return (row >= 0 && row < m_uYNum && col >= 0 && col < m_uXNum);
}

//
//  失败处理
//
void CMineWnd::Dead(UINT row, UINT col)
{
	//按钮所在的区域
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//雷区所在的区域
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, 
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	UINT i, j;
	
	if (m_pMines[row][col].uAttrib == ATTRIB_MINE) 
	{//失败－－点中雷
		for (i = 0; i < m_uYNum; i++)
		{
			for (j = 0; j < m_uXNum; j++)
			{
				m_pMines[row][col].uState = STATE_BLAST;
				m_pMines[row][col].uOldState = STATE_BLAST;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG) 
				{
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	else {//失败－－错误雷
		for (i = 0; i < m_uYNum; i++)
		{
			for (j = 0; j < m_uXNum; j++) 
			{
				m_pMines[row][col].uState = STATE_ERROR;
				m_pMines[row][col].uOldState = STATE_ERROR;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG)
				{
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	//失败处理
	InvalidateRect(rcMineArea);
	m_uBtnState = BUTTON_DEAD;
	InvalidateRect(rcBtn);	
	m_uGameState = GS_DEAD;
	
	if (m_uTimer != 0) 
	{//将定时器去激活
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	if (m_bSoundful) 
	{//失败声音播放
		sndPlaySound((LPCTSTR)LockResource(m_pSndDead), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
}

//
//  胜利判断并处理
//
BOOL CMineWnd::Victory()
{
	UINT i, j;
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	
	// 检测是否胜利
	for (i = 0; i < m_uYNum; i++)
	{
		for (j = 0; j < m_uXNum; j++) 
		{
			if (m_pMines[i][j].uState == STATE_NORMAL) return FALSE;
			if (m_pMines[i][j].uState == STATE_DICEY) return FALSE;
		}
	}
	
	//胜利则作胜利处理
	m_uBtnState = BUTTON_VICTORY;
	m_uGameState = GS_VICTORY;
	Invalidate();
	if (m_uTimer != 0) 
	{//将定时器去激活
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}
	//胜利声音
	if (m_bSoundful)
	{
		sndPlaySound((LPCTSTR)LockResource(m_pSndVictory), 
			SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
	
	//英雄榜
	UINT uRecord;
	if (m_uLevel == LEVEL_PRIMARY) uRecord = m_uPrimary;
	else if (m_uLevel == LEVEL_SECONDRY) uRecord = m_uSecond;
	else if (m_uLevel == LEVEL_ADVANCE) uRecord = m_uAdvance;
	if (uRecord > m_uSpendTime) 
	{
		CDlgNewRecord dlg;
		dlg.SetLevel(m_uLevel);
		dlg.DoModal();
		switch(m_uLevel)
		{
		case LEVEL_PRIMARY:
			m_uPrimary = m_uSpendTime;
			m_szPrimary = dlg.GetName();
			break;
		case LEVEL_SECONDRY:
			m_uSecond = m_uSpendTime;
			m_szSecond = dlg.GetName();
			break;
		case LEVEL_ADVANCE:
			m_uAdvance = m_uSpendTime;
			m_szAdvance = dlg.GetName();
			break;
		case LEVEL_CUSTOM:
			return TRUE;
		default: 
			break;
		}
		OnMemuHero();
	}
	
	return TRUE;
}
//
// 鼠标右键，根据原先不同的标记作状态循环修改
// 以便用户可以修改其原先标志
//
void CMineWnd::OnLRBtnDown(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++) 
		{
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL) 
			{
				m_pMines[i][j].uState = STATE_EMPTY;
			}
			else if (m_pMines[i][j].uState == STATE_DICEY)
			{
				m_pMines[i][j].uState = STATE_DICEY_DOWN;
			}
		}
	}
}

void CMineWnd::OnLRBtnUp(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++)
		{
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uOldState == STATE_NORMAL) 
			{
				m_pMines[i][j].uState = STATE_NORMAL;
			}
			else if (m_pMines[i][j].uOldState == STATE_DICEY) 
			{
				m_pMines[i][j].uState = STATE_DICEY;
			}
		}
	}
	//	Invalidate();
}


//展开拓展周围8个方向
void CMineWnd::OpenAround(UINT row, UINT col)
{
	//如果周围相邻的标志雷数 != 周围相邻的雷数 则返回
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return;

	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++) 
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL) 
			{//如果该区域为正常区域

				//拓展该雷区
				ExpandMines(i, j);
				around = GetAroundNum(i, j);
				m_pMines[i][j].uState = 15 - around;
				m_pMines[i][j].uOldState = 15 - around;
			}
		}
	}

	// 判断是否胜利，是则将地图中所有雷标识出来
	if (Victory())
	{
		for (i = 0; i < m_uYNum; i++) 
		{
			for (j = 0; j < m_uXNum; j++)
			{
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE)
				{
					m_pMines[i][j].uState = STATE_FLAG;
					m_pMines[i][j].uOldState = STATE_FLAG;
				}
			}
		}
		m_nLeaveNum = 0;
		Invalidate();
	}
}

BOOL CMineWnd::ErrorAroundFlag(UINT row, UINT col)
{
		//如果周围相邻的标志雷数 != 周围相邻的雷数 则返回
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return FALSE;

	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) 
	{
		for (j = minCol; j < maxCol; j++) 
		{
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG)
			{
				if (m_pMines[i][j].uAttrib != ATTRIB_MINE)
				{
					Dead(i, j);
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

void CMineWnd::OpenByCheat()
{
	UINT i, j;
	for (i = 0; i < m_uYNum; i++)
	{
		for (j = 0; j < m_uXNum; j++)
		{
			if (m_pMines[i][j].uState == STATE_FLAG) continue;
			OpenAround(i, j);
		}
	}
}

void CMineWnd::FreeMines()
{
	//	if (m_pMines) {
	//		delete[] m_pMines;
	//		m_pMines = NULL;
	//	}
	//	for (UINT row = 0; row<m_uYNum; row++) {
	//		if (m_pMines[row]) {
	//			delete []m_pMines[m_uXNum];
	//			delete m_pMines[row];
	//		}
	//	}
}

void CMineWnd::SizeWindow( void )
{
	UINT uWidth = DEFAULT_FRAME_X + m_uXNum * MINE_WIDTH +
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 + SIDE_WIDTH_1; 
	UINT uHeight = DEFAULT_FRAME_Y + m_uYNum * MINE_HEIGHT + 
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 * 2 + SIDE_WIDTH_1 + SHELL_S_H;	
	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, 
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);	// SWP_NOCOPYBITS does not do function？？？
	
	GetClientRect(&m_rcClient);
	// Button Position
	m_uBtnRect[0] = m_rcClient.right / 2 - 12;			// the left side of the start(state) button without 3d border
	m_uBtnRect[1] = m_rcClient.right / 2 - 13;			// the left side of the start(state) button with 3d border
	m_uBtnRect[2] = m_rcClient.right / 2 + 12;			// the right side of the start(state) button 
	
	// Number Position
	m_uNumRect[0] = m_rcClient.right - 55;				// the left side of the spend time rect	with 3d border
	m_uNumRect[1] = m_rcClient.right - 15;				// the right side of the spend time rect
	m_uNumRect[2] = m_rcClient.right - 54;				// the left side of the spend time rect without 3d border
	//	m_uNumRect[3] = m_rcClient.right - 41;
	//	m_uNumRect[4] = m_rcClient.right - 28;	
	
	// Shell Position
	m_uShellRcX[0] = m_rcClient.right;					//
	m_uShellRcX[1] = m_rcClient.right - 14;				// the width of the small(big) shell
	
	m_uShellRcY[0] = m_rcClient.bottom;					// 
	m_uShellRcY[1] = m_rcClient.bottom - SHELL_L_START_Y - 5;		// the height of the big shell
}

//
// 绘制笑脸按钮图
//
void CMineWnd::DrawButton(CPaintDC &dc)
{
	CDC cdc;	
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpButton);	
	dc.StretchBlt(m_uBtnRect[0], 16, 24, 24, &cdc, 0, 24 * m_uBtnState, 24, 24, SRCCOPY);
	
	dc.Draw3dRect(m_uBtnRect[1], 15, 26, 26, m_clrDark, m_clrDark);
	
}

//
// 绘制数字
//
void CMineWnd::DrawNumber(CPaintDC &dc)
{
	CDC dcMemory;	
	dcMemory.CreateCompatibleDC(&dc);
	dcMemory.SelectObject(m_bmpNumber);
	
	dc.Draw3dRect(16, 15, 41, 25, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(m_uNumRect[0], 15, 41, 25, m_clrDark, COLOR_WHITE);
	int num;
	
	// draw remaining mine numbers
	num = (m_nLeaveNum<0) ? 11 : m_nLeaveNum/100;
	dc.StretchBlt(17, 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
	num = (m_nLeaveNum < 0) ? -(m_nLeaveNum-num*100)/10 : (m_nLeaveNum-num*100)/10;
	dc.StretchBlt(30, 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
	num = (m_nLeaveNum<0) ? -m_nLeaveNum%10 : m_nLeaveNum%10;
	dc.StretchBlt(43, 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
	
	// draw take seconds
	num = m_uSpendTime / 100;
	dc.StretchBlt(m_uNumRect[0], 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
	num = (m_uSpendTime-num*100)/10;
	dc.StretchBlt(m_uNumRect[0] + 13, 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
	num = m_uSpendTime%10;
	dc.StretchBlt(m_uNumRect[0]+26, 16, 13, 23, &dcMemory, 0, 276-23*(num+1), 13, 23, SRCCOPY);
}

//
// 绘制外壳
//
void CMineWnd::DrawShell(CPaintDC &dc)
{
	// 绘画2条白条
	dc.FillSolidRect(0, 0, m_uShellRcX[0], LINE_WIDTH_0, COLOR_WHITE);
	dc.FillSolidRect(0, 0, LINE_WIDTH_0, m_uShellRcY[0], COLOR_WHITE);
	
	// 画小的外壳
	dc.Draw3dRect(SHELL_S_START_X, SHELL_S_START_Y, 
		m_uShellRcX[1], SHELL_S_H, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_S_START_X + 1, SHELL_S_START_Y + 1, 
		m_uShellRcX[1] - 2, SHELL_S_H - 2, m_clrDark, COLOR_WHITE);
	
	// 画大的外壳
	dc.Draw3dRect(SHELL_L_START_X, SHELL_L_START_Y,
		m_uShellRcX[1], m_uShellRcY[1], m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_L_START_X + 1, SHELL_L_START_Y + 1,
		m_uShellRcX[1] - 2, m_uShellRcY[1] - 2, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_L_START_X + 2, SHELL_L_START_Y + 2, 
		m_uShellRcX[1] - 4, m_uShellRcY[1] - 4, m_clrDark, COLOR_WHITE);
}

//
// 绘制雷区
//
void CMineWnd::DrawMineArea(CPaintDC &dc)
{
	CDC dcMemory; //用作内存设备
	dcMemory.CreateCompatibleDC(&dc); //使得这个设备与dc兼容
	dcMemory.SelectObject(m_bmpMine); //将内存设备与位图资源关联
	
	for (UINT i = 0; i<m_uYNum; i++) 
	{
		for (UINT j = 0; j<m_uXNum; j++) 
		{//根据[i][j]区域的雷方块状态拷贝相应的图像到[i][j]雷区的特定区域
			dc.StretchBlt(MINEAREA_FRAME_X+16*j, MINEAREA_FRAME_Y+16*i, 
				16, 16, &dcMemory, 0, 16*m_pMines[i][j].uState, 16, 16, SRCCOPY);
		}
	}
}

//
// 绘制按下扫雷后的数字
//
void CMineWnd::DrawDownNum(MINEWND* mine, UINT num)
{
	mine->uState = 15 - num;
	mine->uOldState = 15 - num;
	CRect rcMine(mine->uCol * 16, mine->uRow * 16, (mine->uCol+1) *16, (mine->uRow+1) * 16);
	InvalidateRect(rcMine);
}

//重新绘制雷区域的某个方块
void CMineWnd::DrawSpecialMine(UINT row, UINT col)
{
	CRect rcMine(col * 16, row * 16, (col+1) * 16, (row+1) * 16);
	InvalidateRect(rcMine);
}

//
//  获取体图中(x,y)区域的雷信息
//
MINEWND* CMineWnd::GetMine(long x, long y)
{
	//保证参数合格
	if (x < MINEAREA_FRAME_X || y < MINEAREA_FRAME_Y) 
	{
		return NULL;
	}

	//根据坐标值算出该小方块所在地图的行和列
	UINT uCol = (UINT)(x-MINEAREA_FRAME_X) / 16;
	UINT uRow = (UINT)(y-MINEAREA_FRAME_Y) / 16;
	//返回该区域的雷信息
	return &m_pMines[uRow][uCol];
}

void CMineWnd::SetCheckedLevel()
{
	if (m_pSubMenu)
	{
		m_pSubMenu->CheckMenuItem(IDM_PRIMARY, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_SECOND, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_ADVANCE, MF_UNCHECKED | MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_CUSTOM, MF_UNCHECKED | MF_BYCOMMAND);
		switch(m_uLevel)
		{
		case LEVEL_PRIMARY:
			m_pSubMenu->CheckMenuItem(IDM_PRIMARY, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_SECONDRY:
			m_pSubMenu->CheckMenuItem(IDM_SECOND, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_ADVANCE:
			m_pSubMenu->CheckMenuItem(IDM_ADVANCE, MF_CHECKED | MF_BYCOMMAND);
			break;
		case LEVEL_CUSTOM:
			m_pSubMenu->CheckMenuItem(IDM_CUSTOM, MF_CHECKED | MF_BYCOMMAND);
			break;
		default: 
			break;
		}
	}
}

void CMineWnd::SetCheckedMark()
{
	if (m_pSubMenu) {
		if (m_bMarkful) {
			m_pSubMenu->CheckMenuItem(IDM_MARK, MF_CHECKED | MF_BYCOMMAND);
		}
		else {
			m_pSubMenu->CheckMenuItem(IDM_MARK, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedColor()
{
	if (m_pSubMenu)
	{
		if (m_bColorful)
		{
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_CHECKED | MF_BYCOMMAND);
		}
		else 
		{
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedSound()
{
	if (m_pSubMenu)
	{
		if (m_bSoundful) 
		{
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_CHECKED | MF_BYCOMMAND);
		}
		else 
		{
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_UNCHECKED | MF_BYCOMMAND);
		}
	}
}

void CMineWnd::SetCheckedCheat()
{
	if (m_pSubMenu)
	{
//		if (m_bCheat) 
//		{
//			m_pSubMenu->CheckMenuItem(IDM_CHEAT, MF_CHECKED | MF_BYCOMMAND);
//		}
	//	else 
//		{
			m_pSubMenu->CheckMenuItem(IDM_CHEAT, MF_UNCHECKED | MF_BYCOMMAND);
//		}
	}
}

void CMineWnd::LoadWaveSrc()
{
	HMODULE hMdl;
	hMdl = AfxGetResourceHandle();
	HRSRC hSrc;
	
	hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_DEAD), _T("WAVE"));
	m_pSndDead = LoadResource(hMdl, hSrc);
	hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_VICTORY), _T("WAVE"));
	m_pSndVictory = LoadResource(hMdl, hSrc);
	hSrc = FindResource(hMdl, MAKEINTRESOURCE(IDR_WAVE_CLOCK), _T("WAVE"));
	m_pSndClock = LoadResource(hMdl, hSrc);
}

void CMineWnd::FreeWaveSrc()
{
	if (m_pSndDead)
	{
		FreeResource(m_pSndDead);
		m_pSndDead = NULL;
	}
	if (m_pSndVictory) 
	{
		FreeResource(m_pSndVictory);
		m_pSndVictory = NULL;
	}
	if (m_pSndClock)
	{
		FreeResource(m_pSndClock);
		m_pSndClock = NULL;
	}
}

void CMineWnd::ResetRecord()
{
	m_uPrimary = DEFAULT_RECORD;
	m_szPrimary = DEFAULT_HOLDER;
	m_uSecond= DEFAULT_RECORD;
	m_szSecond = DEFAULT_HOLDER;
	m_uAdvance = DEFAULT_RECORD;
	m_szAdvance = DEFAULT_HOLDER;
}

void CMineWnd::SetCustom(UINT xNum, UINT yNum, UINT mNum)
{
	m_uXNum = xNum;
	m_uYNum = yNum;
	m_uMineNum = mNum;
}

void CMineWnd::OnPaint() 
{

	CPaintDC dc(this);    // 用以屏幕显示的设备
	CDC dcMemory;  // 内存设备

	CBitmap bitmap;

	if (!dc.IsPrinting()) 
	{

		// 与dc设备兼容
		if (dcMemory.CreateCompatibleDC(&dc))
		{	
			// 使得bitmap与实际显示的设备兼容
			if (bitmap.CreateCompatibleBitmap(&dc, m_rcClient.right, m_rcClient.bottom))
			{
				// 内存设备选择物件－位图
				dcMemory.SelectObject(&bitmap);
				//绘制背景框
				dcMemory.FillRect(&m_rcClient, &m_brsBG);
				
				DrawButton((CPaintDC&) dcMemory);//笑脸按钮绘图
				DrawNumber((CPaintDC&) dcMemory);//文字绘图(计时器文字和剩余雷数文字)
				DrawShell((CPaintDC&) dcMemory);//3D效果外壳绘图
				DrawMineArea((CPaintDC&) dcMemory);//雷区绘图


				// 将内存设备的内容拷贝到实际屏幕显示的设备
				dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, &dcMemory, 0, 0, SRCCOPY);
				bitmap.DeleteObject();
			}
		}
	}
}

void CMineWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	SizeWindow();
	CWnd::OnShowWindow(bShow, nStatus);
}

void CMineWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_EVENT)
	{
		if (m_bSoundful) 
		{
			sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		}
		m_uSpendTime++;
		CRect rcNum(m_uNumRect[0], 15, m_uNumRect[1], 39);
	//	InvalidateRect(rcNum);
		Invalidate();
		if (m_uSpendTime >= DEFAULT_RECORD) 
		{
			KillTimer(ID_TIMER_EVENT);
			m_uTimer = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CMineWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//按钮所在的区域
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//雷区所在的区域
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	SetCapture();			// capture the mouse cursor
	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;

	if (rcBtn.PtInRect(point)) 
	{// click in the button area
		m_bClickBtn = TRUE;
		m_uBtnState = BUTTON_DOWN;
		InvalidateRect(rcBtn);
	}
	else if (rcMineArea.PtInRect(point))
	{// click in the mine area
		// change mine state by gamestate
		switch(m_uGameState) 
		{						
		case GS_WAIT: case GS_RUN:
			m_pNewMine = GetMine(point.x, point.y);
			if (!m_pNewMine) return;
			if (m_pNewMine->uState == STATE_NORMAL)
			{
				m_pNewMine->uState = STATE_EMPTY;
			}
			if (m_pNewMine->uState == STATE_DICEY)
			{
				m_pNewMine->uState = STATE_DICEY_DOWN;
			}
			m_pOldMine = m_pNewMine;
			break;
		case GS_DEAD: case GS_VICTORY:
			return;
			break;
		default :
			break;
		}
		m_uBtnState = BUTTON_CLICK;
		InvalidateRect(rcBtn);
		// both of the left button and the right button are pushing down 
		if (nFlags == (MK_LBUTTON | MK_RBUTTON)) 
		{
			m_bLRBtnDown = TRUE;
			OnLRBtnDown(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		InvalidateRect(rcMineArea);
	}
	else 
	{											// click in other area
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
		{
			m_uBtnState = BUTTON_CLICK;
			InvalidateRect(rcBtn);
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMineWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
		//笑脸图按钮所在的区域
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//雷区所在的区域
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, 
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	if (rcBtn.PtInRect(point)) 
	{// 点击笑脸图

			Invalidate();
			InitGame();
	}
	else if (rcMineArea.PtInRect(point)) 
	{//点击雷区域
		CString value;		
		UINT around = 0;

		//根据不同的游戏状态作处理
		switch(m_uGameState) 
		{
			//游戏进行状态
		case GS_WAIT: case GS_RUN:
			// first get the MINEWND which if pushing down
			m_pOldMine = GetMine(point.x, point.y);
			if (!m_pOldMine) 
			{
				ReleaseCapture();
				return;
			}
			// do normal process
			// judge whether the lr button are both pushed down
			//检测判断当前状态是否为左右鼠标同时按下
			if (m_bLRBtnDown) 
			{
				m_bLRBtnDown = FALSE;
				OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
				if (m_uGameState == GS_WAIT)
				{
					m_uBtnState = BUTTON_NORMAL;
					Invalidate();
					ReleaseCapture();
					return;
				}
				// if the around flags number equal to the around mines number, expand.
				//假若周围已经标识的雷＝周围真正的雷数，拓展
				if (m_pOldMine->uState != STATE_FLAG)
				{
					OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
				}
				// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
				if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol))
				{

					Dead(m_pOldMine->uRow, m_pOldMine->uCol);
					ReleaseCapture();
					return;
				}
			}
			else 
			{
				// start the game, init the mines area
				//如果游戏尚未开始，点击左键启动游戏
				if (m_uGameState == GS_WAIT) 
				{
					if (m_uTimer)
					{
						KillTimer(ID_TIMER_EVENT);
						m_uTimer = 0;
					}
					// the following five lines refresh the remining mine num rect immediately 
					// when click in the mine area at the first time
					m_uSpendTime = 1;
					Invalidate();
					if (m_bSoundful) 
					{
						sndPlaySound((LPCTSTR)LockResource(m_pSndClock), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
					}
					//启动定时器
					m_uTimer = SetTimer(ID_TIMER_EVENT, 1000, NULL);
					//布雷
					LayMines(m_pOldMine->uRow, m_pOldMine->uCol);		// lay all the mines down 
					//改变游戏状态为"运行/GS_RUN"
					m_uGameState = GS_RUN;

				}

				if (m_pOldMine->uOldState == STATE_NORMAL)
				{//当该雷区域为正常未作标记才打开
					// first judge if the special MINEWND is a mine
					//如果该区域为雷，则死亡
					if (IsMine(m_pOldMine->uRow, m_pOldMine->uCol)) 
					{
						Dead(m_pOldMine->uRow, m_pOldMine->uCol);
						ReleaseCapture();
						return;
					}
					// the special MINEWND is not a mine 
					//不是雷的时候，获取其周围的雷数目
					around = GetAroundNum(m_pOldMine->uRow, m_pOldMine->uCol);
				// 如果为空白区域，拓展，否则打开该区域（显示周围有多少雷数）
					if (around == 0) ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
					else DrawDownNum(m_pOldMine, around);
				}
				else if (m_pOldMine->uOldState == STATE_DICEY)
				{//标志为“？”问号的时候
					m_pOldMine->uState = STATE_DICEY;
				}

				//判断是否为胜利
				if (Victory())
				{
					Invalidate();
					ReleaseCapture();
					return;
				}
			}
			break;
		case GS_VICTORY:
		case GS_DEAD:
			ReleaseCapture();		// release the cursor
			return;
		default :
			break;
		}
		m_uBtnState = BUTTON_NORMAL;
		Invalidate();
	}
	else 
	{//点击非雷区域
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
		{
			m_uBtnState = BUTTON_NORMAL;
			InvalidateRect(rcBtn);
		}
	}
	
	ReleaseCapture();		// release the cursor
	CWnd::OnLButtonUp(nFlags, point);
}

void CMineWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
		//笑脸图按钮所在的区域
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	//雷区所在的区域
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, 
		MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	m_bLRBtnDown = FALSE;
		
	if (rcMineArea.PtInRect(point)) 
	{//点击雷区域			
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN) 
		{
			m_pNewMine = GetMine(point.x, point.y);
			if (!m_pNewMine) return;
			// both of the left button and the right button are pushing down 
			if (nFlags == (MK_LBUTTON | MK_RBUTTON))
			{
				m_bLRBtnDown = TRUE;
				OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
			}
			else
			{
				switch(m_pNewMine->uState) 
				{
				case STATE_NORMAL:
					m_pNewMine->uState = STATE_FLAG;
					m_pNewMine->uOldState = STATE_FLAG;
					m_nLeaveNum--;
					break;
				case STATE_FLAG:
					m_pNewMine->uState = STATE_DICEY;
					m_pNewMine->uOldState = STATE_DICEY;
					m_nLeaveNum++;
					break;
				case STATE_DICEY:
					m_pNewMine->uState = STATE_NORMAL;
					m_pNewMine->uOldState = STATE_NORMAL;
					break;
				default: 
					break;
				}
			}
			Invalidate();

		}
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMineWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	m_pOldMine = GetMine(point.x, point.y);
	if (!m_pOldMine) return;
	// judge whether the lr button are both pushed down
	if (m_bLRBtnDown) 
	{
		m_bLRBtnDown = FALSE;
		OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
		if (m_uGameState == GS_WAIT) 
		{
			m_uBtnState = BUTTON_NORMAL;
			Invalidate();
			return;
		}
		// if the around flags number equal to the around mines number, expand.
		if (m_pOldMine->uState != STATE_FLAG)
		{
			OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
		if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol))
		{
			//			Dead(m_pOldMine->uRow, m_pOldMine->uCol);
			//			ReleaseCapture();
			return;
		}
	}
	else
	{
		Victory();
	}
				
	CWnd::OnRButtonUp(nFlags, point);
}

void CMineWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_RBUTTON))
	{
		CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
		CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
			MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
		
		if (rcBtn.PtInRect(point)) 
		{				// point in button area
			switch(m_uGameState)
			{
			case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BUTTON_DOWN : BUTTON_CLICK;
				break;
			case GS_DEAD: case GS_VICTORY:
				if (m_bClickBtn) m_uBtnState = BUTTON_DOWN;
				break;
			default: 
				break;
			}
			InvalidateRect(rcBtn);
		}
		else if (rcMineArea.PtInRect(point)) 
		{		// point in mine area
			switch(m_uGameState) 
			{
			case GS_RUN:
				m_pNewMine = GetMine(point.x, point.y);
				if (!m_pNewMine || !m_pOldMine) return;
				if (m_pNewMine->uCol != m_pOldMine->uCol ||
					m_pNewMine->uRow != m_pOldMine->uRow) 
				{
					// change the new mine rect state
					switch(m_pNewMine->uState)
					{
					case STATE_NORMAL:
						m_pNewMine->uState = STATE_EMPTY;
						break;
					case STATE_DICEY:
						m_pNewMine->uState = STATE_DICEY_DOWN;
						break;
					}
					// resume the old mine rect state
					switch(m_pOldMine->uOldState) 
					{
					case STATE_NORMAL:
						m_pOldMine->uState = STATE_NORMAL;
						break;
					case STATE_DICEY:
						m_pOldMine->uState = STATE_DICEY;
						break;
					default :
						break;
					}
					// judge whether the lr button are pushed down
					if (m_bLRBtnDown) 
					{
						OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
						OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
					}
					m_pOldMine = m_pNewMine;
				}
				InvalidateRect(rcMineArea);
				break;
			case GS_VICTORY: case GS_DEAD:
				return;
			default: 
				break;
			}
		}
		else
		{										// point in other area
			switch(m_uGameState) 
			{
			 case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BUTTON_NORMAL : BUTTON_CLICK;
				if (m_pNewMine) 
				{
					if (m_pNewMine->uOldState == STATE_NORMAL)
					{
						m_pNewMine->uState = STATE_NORMAL;
					}
					else if (m_pNewMine->uOldState == STATE_DICEY)
					{
						m_pNewMine->uState = STATE_DICEY;
					}
				}
				break;
			case GS_DEAD: 
				m_uBtnState = BUTTON_DEAD;
				break;
			case GS_VICTORY:
				m_uBtnState = BUTTON_VICTORY;
				break;
			default: 
				break;
			}
			Invalidate();
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CMineWnd::OnMemuStart() 
{	
	InitGame();
	Invalidate();
}

void CMineWnd::OnMemuPrimary() 
{
	m_uLevel = LEVEL_PRIMARY;
	m_uXNum = PRIMARY_XNUM;
	m_uYNum = PRIMARY_YNUM;
	m_uMineNum = PRIMARY_MINENUM;
	
	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuSecond() 
{
	m_uLevel = LEVEL_SECONDRY;
	m_uXNum = SECONDRY_XNUM;
	m_uYNum = SECONDRY_YNUM;
	m_uMineNum = SECONDRY_MINENUM;
	
	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuAdvance() 
{
	m_uLevel = LEVEL_ADVANCE;
	m_uXNum = ADVANCE_XNUM;
	m_uYNum = ADVANCE_YNUM;
	m_uMineNum = ADVANCE_MINENUM;
	
	SetCheckedLevel();
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuCustom() 
{
	m_uLevel = LEVEL_CUSTOM;
	SetCheckedLevel();
	CDlgCustom dlg;
	dlg.InitData(m_uXNum, m_uYNum, m_uMineNum);
	dlg.DoModal();
	
	InitGame();
	Invalidate();
	SizeWindow();
}

void CMineWnd::OnMemuCheat() 
{
//	m_bCheat = !m_bCheat;
//	SetCheckedCheat();
//	Invalidate();
}

void CMineWnd::OnMemuMark() 
{
	m_bMarkful = !m_bMarkful;
	SetCheckedMark();
	Invalidate();
}

void CMineWnd::OnMemuColor() 
{	
	m_bColorful = !m_bColorful;
	LoadBitmap();
	SetCheckedColor();
	Invalidate();
}

void CMineWnd::OnMemuSound() 
{	
	m_bSoundful = !m_bSoundful;
	SetCheckedSound();
	if (m_bSoundful) 
	{
		LoadWaveSrc();
	}
	else
	{
		FreeWaveSrc();
	}
}

void CMineWnd::OnMemuHero() 
{	
	CDlgHero dlg;
	dlg.SetBRecord(m_uPrimary);
	dlg.SetBHolder(m_szPrimary);
	dlg.SetIRecord(m_uSecond);
	dlg.SetIHolder(m_szSecond);
	dlg.SetERecord(m_uAdvance);
	dlg.SetEHolder(m_szAdvance);
	dlg.DoModal();
}

void CMineWnd::OnMemuExit() 
{	
	PostQuitMessage(0);
}

void CMineWnd::OnMemuHelpList() 
{	
	::WinExec("HH	WINMINE.CHM", SW_SHOW);
}

void CMineWnd::OnMemuHelpFind() 
{	
	::WinExec("HH	WINMINE.CHM", SW_SHOW);
}

void CMineWnd::OnMemuHelpUse() 
{	
	::WinExec("HH	NTHelp.CHM", SW_SHOW);
}

void CMineWnd::OnMemuAbout() 
{	
	ShellAbout(this->m_hWnd, "扫雷", "skybluehacker@yahoo.com.cn",NULL);
}

void CMineWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	switch(nChar)
	{
	case VK_F1:
		OnMemuHelpList();
		break;
	case VK_F2:
		OnMemuStart();
		break;
	default: 
		break;
	}
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMineWnd::OnInitMenu(CMenu* pMenu) 
{
	CWnd::OnInitMenu(pMenu);
	if ((m_pSubMenu = pMenu->GetSubMenu(0)) == 0) 
	{
		AfxMessageBox("初始化菜单失败！");
		PostQuitMessage(0);
	}
	else
	{
		SetCheckedLevel();
		SetCheckedMark();
		SetCheckedColor();
		SetCheckedSound();
	//	SetCheckedCheat();
	}
}

void CMineWnd::OnMemuClose() 
{
	SaveConfig();
	KillTimer(ID_TIMER_EVENT);
	
	CWnd::OnClose();
}



