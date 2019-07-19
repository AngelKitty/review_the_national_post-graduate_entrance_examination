/*-------------------------------------------------------
/*  《加密与解密》第三版 第7章 加密算法   
/*   MD5 *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  by cnbragon 2006
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
TCHAR szConstName[]="pediy";
#define MAXINPUTLEN 200

/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    GenerateSerial( HWND) ;

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
		case IDC_Name:		
			GenerateSerial(hDlg);
			break;

		case IDC_About :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	

		case IDC_Exit:
			PostQuitMessage(0);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));
		SendDlgItemMessage(hDlg,IDC_Name,WM_SETTEXT,0,(LPARAM)szConstName);
		                                                             

		break;
	}
     return 0;
}


/*-------------------------------------------------------------*/
/* MD5Hash － MD5计算主函数                                    */
/*-------------------------------------------------------------*/
BOOL GenerateSerial(HWND hWnd) 
{
	MD5_CTX context;
	long dtLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szTeam[]="www.pediy.com";
	TCHAR szBase32[]="23456789ABCDEFGHJKLMNPQRSTUVWXYZ";
	TCHAR szSerial[20]={0};

	unsigned int tcTemp=0;

	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "please input name");
		return FALSE;
	}
	MD5Init(&context);
	MD5Update(&context, szName, dtLength);
	MD5Update(&context,szTeam,lstrlen(szTeam));
	MD5Final(szHash, &context);

	for(i=0;i<16;i++)
	{
		tcTemp=(unsigned int)szHash[i]%32;
		szBuffer[i]=szBase32[tcTemp];
	}

	memcpy(szSerial,szBuffer,4);
	szSerial[4]='-';
	memcpy((szSerial+5),(szBuffer+4),4);
	szSerial[9]='-';
	memcpy((szSerial+10),(szBuffer+8),4);
	szSerial[14]='-';
	memcpy((szSerial+15),(szBuffer+12),4);
	
    SetDlgItemText(hWnd, IDC_Serial,szSerial);       

	return TRUE;
}
