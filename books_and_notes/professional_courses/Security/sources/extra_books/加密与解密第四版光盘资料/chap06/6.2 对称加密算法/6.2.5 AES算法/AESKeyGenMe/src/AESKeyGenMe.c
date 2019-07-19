/*-------------------------------------------------------
/*  《加密与解密》第三版 第6章 加密算法   
/*   AES KeyGenMe
/*  (c) www.PEDIY.com  by cnbragon 2007
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "md5c.c"
#include "resource.h"
#include "miracl.h"
#include "mirdef.h"

#pragma comment( lib, "ms32.lib") 


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
	TCHAR szWaterMark[]="Coded by cnbragon belongs to iNsidE PaNdoRa's BoX and Reverse Code Team";
	MD5_CTX context;
	aes aes_test;
	long dtLength,lsLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	BYTE szKey[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	lsLength=GetDlgItemText(hWnd,IDC_Serial,szSerial,sizeof(szSerial)/sizeof(TCHAR)+1);
	if (lsLength!=32)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	for(i=0;szSerial[i]!=0;i++)
	{
		if(isxdigit(szSerial[i])==0)
		{
			SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
			return FALSE;
		}
	}

	for (i=0;i<16;i++)
	{
		sscanf((szSerial+i*2),"%02X",(szBuffer+i));
	}
	memset(&context,0,sizeof(context));
	MD5Init(&context);
	MD5Update(&context,szName,dtLength);
	MD5Final(szHash, &context);
	memset(&aes_test,0,sizeof(aes));	
	aes_init(&aes_test,MR_ECB,16,szKey,NULL);
    aes_encrypt(&aes_test,szBuffer);
	if (memcmp(szHash,szBuffer,16)==0)
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
