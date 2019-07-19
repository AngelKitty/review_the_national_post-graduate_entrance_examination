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
#define MAXINPUTLEN 200

/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CheckSerial( HWND) ;

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
		case IDC_Check:		
			CheckSerial(hDlg);
			break;	

		case IDC_Exit:
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
BOOL CheckSerial(HWND hWnd) 
{
	MD5_CTX context;
	long dtLength,lsLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	TCHAR szTeam[]="www.pediy.com";
	TCHAR szSNtemp[20]={0};
	TCHAR szBase32[]="23456789ABCDEFGHJKLMNPQRSTUVWXYZ";
	unsigned int tcTemp=0;

	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	lsLength=GetDlgItemText(hWnd,IDC_Serial,szSerial,sizeof(szSerial)/sizeof(TCHAR)+1);
	if (lsLength!=19)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	if ((szSerial[4]!='-')||(szSerial[9]!='-')||(szSerial[14]!='-'))
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	memcpy(szSNtemp,szSerial,4);
	memcpy((szSNtemp+4),(szSerial+5),4);
	memcpy((szSNtemp+8),(szSerial+10),4);
	memcpy((szSNtemp+12),(szSerial+15),4);
	MD5Init(&context);
	MD5Update(&context,szName,dtLength);
	MD5Update(&context,szTeam,lstrlen(szTeam));
	MD5Final(szHash, &context);
		
	for(i=0;i<16;i++)
	{
		tcTemp=(unsigned int)szHash[i]%32;
		szBuffer[i]=szBase32[tcTemp];
	}
	if (lstrcmp(szSNtemp,szBuffer)==0)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Success!");
		return TRUE;
	} 
	else
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	return TRUE;
}
