/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   MD5计算器源码  code by kanxue 2002
/*  (c)  看雪软件安全网站 www.pediy.com
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "md5c.c"
#include "resource.h"

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/
HINSTANCE	hInst;
#define MAXINPUTLEN 200

/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    MD5Hash( HWND) ;

/*-------------------------------------------------------------*/
/* WinMain － 基于WIN32的程序的入口                            */
/*-------------------------------------------------------------*/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	hInst=hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, (DLGPROC)MainDlg,0);
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
		case  WM_LBUTTONDOWN:
			PostMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			return TRUE ;
        
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
BOOL CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	

	switch (message)
	{    
	case WM_CLOSE:
		EndDialog(hDlg,0);
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{ 
		

		case IDC_TXT0:		
			MD5Hash(hDlg);
			break;

		case IDC_ABOUT :
		case IDM_HELP_ABOUT :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	

		case IDC_EXIT:
			PostQuitMessage(0);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));  
		                                                             

		break;
	}
     return 0;
}


/*-------------------------------------------------------------*/
/* MD5Hash － MD5计算主函数                                    */
/*-------------------------------------------------------------*/
BOOL MD5Hash( HWND hWnd) 
{
	MD5_CTX context;
	long dtLength;
	int i;


	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};

	dtLength=GetDlgItemText(hWnd, IDC_TXT0, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (strlen(szName)==0)
	{
		SetDlgItemText(hWnd, IDC_TXT1, "");
		return FALSE;
	}
	MD5Init(&context);
	MD5Update(&context, szName, dtLength);
	MD5Final(szHash, &context);

			
	for(i=0; i < 16; i++)   // 将szHash[]中的16进制转换成字符形式显示
	{
		wsprintf(&szBuffer[i*2], "%02X", *(byte*)(szHash+i));
	}
	
	 SetDlgItemText(hWnd, IDC_TXT1,szBuffer);       

	return TRUE;
}
