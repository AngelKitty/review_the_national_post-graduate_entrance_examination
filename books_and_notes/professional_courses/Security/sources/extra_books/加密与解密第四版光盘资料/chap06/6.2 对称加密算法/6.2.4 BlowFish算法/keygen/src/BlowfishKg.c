/*-------------------------------------------------------
/*  《加密与解密》第三版 第6章 加密算法   
/*   Blowfish *KeyGenMe* 's KeyGen
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
		case IDC_About :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	

		case IDC_Generate:
			GenerateSerial(hDlg);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));
		GenerateSerial(hDlg);
		break;
	}
     return 0;
}

BOOL GenerateSerial(HWND hWnd) 
{
	BLOWFISH_CTX context;
	TCHAR szCipher[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	BYTE bf_key[MAXINPUTLEN]={0};
    DWORD xL,xR,dwVolumeSn,dwXor;
	int i;

	xL=xR=dwVolumeSn=dwXor=0;
	srand(GetTickCount());
	for (i=0;i<12;i++)
	{
		szBuffer[i]=rand()%0x100;
	}
	GetVolumeInformation("C:\\",NULL,0,&dwVolumeSn,NULL,NULL,NULL,0);
	memcpy(bf_key,szBuffer,8);
	Blowfish_Init(&context,bf_key,8);
	xL=*(DWORD*)(&szBuffer[8]);
	xR=xL^dwVolumeSn;
	Blowfish_Encrypt(&context,&xL,&xR);
    *(DWORD*)(&szBuffer[8])=xL;
	*(DWORD*)(&szBuffer[12])=xR;
	memset(&context,0,sizeof(BLOWFISH_CTX));
	
	for(i=0;i<16;i++)
	{
		sprintf((szSerial+i*2),"%02X",(BYTE)szBuffer[i]);
	}

	SetDlgItemText(hWnd, IDC_Serial,szSerial);       

	return TRUE;
}
