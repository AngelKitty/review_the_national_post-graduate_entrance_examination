/*-------------------------------------------------------
/*  《加密与解密》 第三版 第6章 加密算法   
/*   Blowfish KeyGenMe
/*  (c) www.PEDIY.com  by cnbragon 2007
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"
#include "blowfish.c"

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


BOOL CheckSerial(HWND hWnd) 
{
	TCHAR szWaterMark[]="coded by cnbragon belong to iNsidE PaNdoRa's BoX and Reverse Code Team";
	BLOWFISH_CTX context;
	long lsLength;
	int i;
	TCHAR szCipher[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	BYTE bf_key[MAXINPUTLEN]={0};
    DWORD xL,xR,dwVolumeSn,dwXor;

	xL=xR=dwVolumeSn=dwXor=0;
	    
	lsLength=GetDlgItemText(hWnd,IDC_Serial,szSerial,sizeof(szSerial)/sizeof(TCHAR)+1);
	if (lsLength!=32)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	
	for (i=0;i<16;i++)
	{
		sscanf((szSerial+i*2),"%02X",(szBuffer+i));
	}
	
	memcpy(bf_key,szBuffer,8);
	Blowfish_Init(&context,bf_key,8);
	xL=*(DWORD*)(&szBuffer[8]);
	xR=*(DWORD*)(&szBuffer[12]);
	Blowfish_Decrypt(&context,&xL,&xR);
	dwXor=xL^xR;
	memset(&context,0,sizeof(BLOWFISH_CTX));
	    
	GetVolumeInformation("C:\\",NULL,0,&dwVolumeSn,NULL,NULL,NULL,0);
		
	if (dwXor==dwVolumeSn)
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
