/*-------------------------------------------------------
/*  《加密与解密》 第三版 第6章 加密算法   
/*   TEA *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  by cnbragon 2007
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
TCHAR szConstName[]="pediy.com";
#define MAXINPUTLEN 200

/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    GenerateSerial( HWND) ;
void TEA_Decrypt(DWORD* v,DWORD* k)
{
	DWORD y=v[0],z=v[1],sum,delta=0x9e3779b9,n=32;
	sum=delta<<5;
	while (n-->0)
	{
		z-=((y<<4)+k[2])^(y+sum)^((y>>5)+k[3]);
		y-=((z<<4)+k[0])^(z+sum)^((z>>5)+k[1]);
		sum-=delta;		
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

BOOL GenerateSerial(HWND hWnd) 
{
	TCHAR szWaterMark[]="Coded by cnbragon belong to iNsidE PaNdoRa's BoX and Reverse Code Team";
	MD5_CTX context;
	long dtLength;
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
		SetDlgItemText(hWnd, IDC_Serial, "please input name");
		return FALSE;
	}
	MD5Init(&context);
	MD5Update(&context, szName, dtLength);
	MD5Final(szHash, &context);
    
	memcpy(TEA_Key,szHash,16);
	memcpy(szBuffer,szHash,8);
	for (i=0;i<8;i++)
	{
		szBuffer[i]^=szHash[i+8];
	}

	memcpy(dwMessage,szBuffer,8);
	
	TEA_Decrypt(dwMessage,TEA_Key);
    memcpy(szBuffer,dwMessage,8);
    for (i=0;i<8;i++)
    {
		sprintf((szSerial+i*2),"%02X",(BYTE)szBuffer[i]);
    }
    		
    SetDlgItemText(hWnd, IDC_Serial,szSerial);       

	return TRUE;
}
