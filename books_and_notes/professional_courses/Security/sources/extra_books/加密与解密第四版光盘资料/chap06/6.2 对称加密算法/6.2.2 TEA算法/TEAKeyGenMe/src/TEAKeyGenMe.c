/*-------------------------------------------------------
/*  《加密与解密》 第三版 第6章 加密算法   
/*   TEA KeyGenMe
/*  (c) www.PEDIY.com 
 *    Code by cnbragon 2006
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
void TEA_Encrypt(DWORD* v,DWORD* k)
{
	DWORD y=v[0],z=v[1],sum=0,delta=0x9e3779b9,n=32;
	while (n-->0)
	{
		sum+=delta;
		y+=((z<<4)+k[0])^(z+sum)^((z>>5)+k[1]);
		z+=((y<<4)+k[2])^(y+sum)^((y>>5)+k[3]);
	}
	v[0]=y;
	v[1]=z;
}
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
	TCHAR szWaterMark[]="Coded by cnbragon belong to iNsidE PaNdoRa's BoX and Reverse Code Team";
	MD5_CTX context;
	long dtLength,lsLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	DWORD TEA_Key[4]={0};
	DWORD dwMessage[2]={0};
	
	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	lsLength=GetDlgItemText(hWnd,IDC_Serial,szSerial,sizeof(szSerial)/sizeof(TCHAR)+1);
	if (lsLength!=16)
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
	for (i=0;i<8;i++)
	{
		sscanf((szSerial+i*2),"%02X",(szBuffer+i));
	}

    memcpy(dwMessage,szBuffer,8);

	MD5Init(&context);
	MD5Update(&context,szName,dtLength);
	MD5Final(szHash, &context);
		
	memcpy(TEA_Key,szHash,16);
	TEA_Encrypt(dwMessage,TEA_Key);
	memcpy(szBuffer,dwMessage,8);
	for (i=0;i<8;i++)
	{
		szBuffer[i]^=szHash[i];
	}
	if (lstrcmp((szHash+8),szBuffer)==0)
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
