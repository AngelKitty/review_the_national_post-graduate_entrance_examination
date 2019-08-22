/*++

Copyright (c) 2004-2005  Micro-soft

Module Name:

    MineWnd.h

Abstract:

       the mian Frame of the Mine-Game, we should do the most draw-work here,
	surely,the size and style of the window should set here too. 

Author:
	Microsoft's Engineer - Unknown Name

    improved by Weijian Luo (Arthur Luo)   15-Jun-2005

	E-mail: skybluehacker@yahoo.com.cn

Revision History:      1.0

--*/

#if !defined(AFX_MAINWND_H__FB2594FD_B845_48A9_A9D0_FADC86BCE950__INCLUDED_)
#define AFX_MAINWND_H__FB2594FD_B845_48A9_A9D0_FADC86BCE950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct 
{
	UINT uRow;         //所在雷区二维数组的行
	UINT uCol;         //所在雷区二位数组的列
	UINT uState;       //当前状态
	UINT uAttrib;      //方块属性
	UINT uOldState;    //历史状态
} MINEWND;	// 雷方块结构体

/////////////////////////////////////////////////////////////////////////////
class CMineWnd : public CWnd
{
public:
	CMineWnd();
	virtual ~CMineWnd();
	void ResetRecord();
	void SetCustom(UINT xNum, UINT yNum, UINT mNum);
	//{{AFX_VIRTUAL(CMineWnd)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CMineWnd)
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMemuStart();
	afx_msg void OnMemuPrimary();
	afx_msg void OnMemuSecond();
	afx_msg void OnMemuAdvance();
	afx_msg void OnMemuCustom();
	afx_msg void OnMemuMark();
	afx_msg void OnMemuColor();
	afx_msg void OnMemuSound();
	afx_msg void OnMemuExit();
	afx_msg void OnMemuHelpList();
	afx_msg void OnMemuHelpFind();
	afx_msg void OnMemuHelpUse();
	afx_msg void OnMemuAbout();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnMemuClose();
	afx_msg void OnMemuHero();
	afx_msg void OnMemuCheat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// init and save functions
	void LoadConfig();
	void SaveConfig();
	void LoadWaveSrc();
	void FreeWaveSrc();
	void InitGame();
	void LoadBitmap();
	void FreeMines();
	void SizeWindow();
	void LayMines(UINT row, UINT col);
	// draw functions
	void DrawShell(CPaintDC &dc);
	void DrawButton(CPaintDC &dc);
	void DrawNumber(CPaintDC &dc);
	void DrawMineArea(CPaintDC &dc);
	void DrawDownNum(MINEWND* mine, UINT num);
	void DrawSpecialMine(UINT row, UINT col);
	// change menu check state funtions
	void SetCheckedSound();
	void SetCheckedColor();
	void SetCheckedMark();
	void SetCheckedLevel();
	void SetCheckedCheat();
	// other functions
	void ExpandMines(UINT row, UINT col);
	BOOL IsInMineArea(UINT row, UINT col);
	BOOL IsMine(UINT row, UINT col);
	UINT GetAroundNum(UINT row, UINT col);
	UINT GetAroundFlags(UINT row, UINT col);
	void Dead(UINT row, UINT col);
	BOOL Victory();
	// both button click fuctions
	void OnLRBtnDown(UINT row, UINT col);
	void OnLRBtnUp(UINT row, UINT col);
	void OpenAround(UINT row, UINT col);
	BOOL ErrorAroundFlag(UINT row, UINT col/*, UINT &errorRow, UINT &errorCol*/);
	void OpenByCheat();

	MINEWND* GetMine(long x, long y);
protected :
	UINT		m_uXNum;				// X方向小方块个数
	UINT		m_uYNum;				// Y方向小方块个数
	UINT		m_uMineNum;				// 总的雷个数
	int 		m_nLeaveNum;			// 剩余的雷个数
	UINT		m_uSpendTime;			// 游戏开始击到目前所花费的时间
	UINT		m_uGameState;			// 游戏状态
	UINT		m_uTimer;				// 定时器标识
	UINT		m_uNewState;			// 当前选中的小方块的状态

	UINT		m_uLevel;				// 当前游戏等级
	UINT		m_uPrimary;				// 初级记录
	UINT		m_uSecond;				// 中级记录
	UINT		m_uAdvance;				// 高级记录
	CString		m_szPrimary;			// 初级记录保持者
	CString		m_szSecond;				// 中级记录保持者
	CString		m_szAdvance;			// 高级记录保持者

	BOOL		m_bLRBtnDown;			// 是否为左右键同时按下
	BOOL		m_bClickBtn;			// 左键按下的时候鼠标是否位于按钮区域内
	BOOL		m_bMarkful;				// 是否能显示标记
	BOOL		m_bColorful;			// 是否彩色显示
	BOOL		m_bSoundful;			// 是否有声音
	CMenu*		m_pSubMenu;				// 子菜单
	CBitmap		m_bmpMine;				// 雷区背景图像
	CBitmap		m_bmpNumber;			// 数字背景图像
	CBitmap		m_bmpButton;			// 笑脸按钮背景图像
	CBrush		m_brsBG;				// 背景画刷对象
	COLORREF	m_clrDark;				// 各按钮的深色调
	RECT		m_rcClient;				// 客户区域
	UINT		m_uBtnRect[3];			// 按钮框区域坐标数组
	UINT		m_uBtnState;			// 按钮状态
	UINT		m_uNumRect[3];			// 数字框区域坐标数组(包括时间和雷个数)
	UINT		m_uShellRcX[2];			// 内框以及边界的坐标X方向
	UINT		m_uShellRcY[2];			// 内框以及边界的坐标Y方向
	
	MINEWND		m_pMines[100][100];		// 表示雷区内的所有小方块的二维数组
	MINEWND*	m_pNewMine;				// 当前选中的小方块
	MINEWND*	m_pOldMine;				// 上次选中的小方块
	void*		m_pSndDead;				// 失败提示音
	void*		m_pSndVictory;			// 胜利提示音
	void*		m_pSndClock;			// 时钟提示音
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_MAINWND_H__FB2594FD_B845_48A9_A9D0_FADC86BCE950__INCLUDED_)
