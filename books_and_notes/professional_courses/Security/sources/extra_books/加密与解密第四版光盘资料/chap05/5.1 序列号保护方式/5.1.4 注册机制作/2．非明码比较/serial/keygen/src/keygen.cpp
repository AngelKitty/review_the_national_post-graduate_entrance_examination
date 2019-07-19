/*******************************************************
《加密与解密》第三版配套实例
                                      5.1.4 注册机制作
(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

#include <windows.h>
#include <stdio.h>

#include "resource.h"
 

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/

HINSTANCE hInst;
#define MAXINPUTLEN 10


/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;

BOOL GenRegCode( HWND) ;


/*-------------------------------------------------------------*/
/* WinMain － 基于WIN32的程序的入口                            */
/*-------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;
	DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, MainDlg, NULL);
	return 0;	
}

/*-------------------------------------------------------------*/
/* AboutDlgProc － 关于窗口                                    */
/*-------------------------------------------------------------*/

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
        
	case WM_COMMAND :
		switch (LOWORD (wParam))  		            
		{
		case IDOK :
		case IDCANCEL :
			EndDialog (hDlg, 0) ;
			return TRUE ;
		 }
		  break ;
	 }
     return FALSE ;
}

/*-------------------------------------------------------------*/
/*  MainDlg － 主对话窗口                                      */
/*-------------------------------------------------------------*/

BOOL CALLBACK MainDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{			
	switch (message) 
	{
	case WM_INITDIALOG:
		int DlgHeight,DlgWidth,x,y;
		RECT DlgRect;
		RECT DesktopRect;
		HWND hwindow;
		SendMessage(hDlg,WM_SETICON,ICON_BIG,LPARAM(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON)))); //设置图标
		SendDlgItemMessage(hDlg, IDC_TXT0, EM_LIMITTEXT, MAXINPUTLEN, 0);      //初始化edit控件IDC_TXT0字符长度
		// 将窗口定位屏幕中央
		GetWindowRect(hDlg,&DlgRect); 
		hwindow = GetDesktopWindow(); 
		GetWindowRect(hwindow,&DesktopRect);

		DlgHeight = DlgRect.bottom - DlgRect.top;
		DlgWidth = DlgRect.right - DlgRect.left;
		x=(DesktopRect.right+DesktopRect.left-DlgWidth)/2;
		y =(DesktopRect.bottom+DesktopRect.top-DlgHeight)/2;
		MoveWindow(hDlg,x,y,DlgWidth,DlgHeight,FALSE);
		
		
		return TRUE;	 
		break; 
		
	case WM_CLOSE:
		EndDialog(hDlg,0);
		
		return TRUE;
		break;

	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
	
		case IDC_ABOUT :
		case IDM_HELP_ABOUT :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	
		case IDC_OK:
			if(!GenRegCode(hDlg)) 
				MessageBox(hDlg,"输入字符中不能有数字！","Error",MB_ICONEXCLAMATION);
			SetFocus (GetDlgItem(hDlg,IDC_TXT0));

			break;
		case IDC_EXIT: 
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}

		return TRUE;
		break;
	}
	return FALSE;
}
	
/*-------------------------------------------------------------*/
/* GenRegCode － 注册算法主函数                                */
/*-------------------------------------------------------------*/
BOOL GenRegCode( HWND hwnd) 
{
	TCHAR cName[MAXINPUTLEN]={0};
	TCHAR cCode[100]={0};
	
	
	GetDlgItemText(hwnd,IDC_TXT0,cName,sizeof(cName)/sizeof(TCHAR)+1);
	if (strlen(cName)==0)
	{
		
		MessageBox(hwnd,"请输入姓名！","Error",MB_ICONEXCLAMATION);
		return TRUE;
	}


	int i,k1=0,k2=0;
	char ch;
	for(i=0; cName[i]!=0&&i<=9;i++)


	{
		ch=(BYTE)cName[i];
		if(ch<'A')  return FALSE; 

		k1+=(ch>'Z')?(ch-32):ch;	
	}                         
	k2=k1^0x5678^0x1234;  
	wsprintf(cCode,TEXT("%ld"),k2);
	SetDlgItemText(hwnd,IDC_TXT1,cCode);
	return TRUE;	

}



