/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   RSA *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  段钢 2002.12
 *  Modified by cnbragon 2006
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"

#include "miracl.h"
#include "mirdef.h"

#pragma comment( lib, "ms32.lib") 


#if _DEBUG
  #pragma comment(linker,"/NODEFAULTLIB:LIBC")
#endif

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
		SendDlgItemMessage(hDlg,IDC_Name,EM_LIMITTEXT,16,0);
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
	miracl* mip=mirsys(500,16);
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
    long dtLength;
	big n,d,c,m;

	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "please input name");
		return FALSE;
	}
	
	/* MIRACL大数运算库运算 
     * p=C75CB54BEDFA30AB
     * q=A554665CC62120D3
     * n=80C07AFC9D25404D6555B9ACF3567CF1
     * d=651A40B9739117EF505DBC33EB8F442D
     * e=10001
     * 128 bit
	 */
	mip->IOBASE=16;                                // 16进制模式
	c=mirvar(0);                                   // MIRACL的大数类型
	n=mirvar(0);
	d=mirvar(0);
	m=mirvar(0);

	bytes_to_big(dtLength,szName,c);                    // 将姓名转换成大数
	cinstr(n,"80C07AFC9D25404D6555B9ACF3567CF1");  // 初始化模数n	 
	cinstr(d,"651A40B9739117EF505DBC33EB8F442D");  // 初始化私钥d
	powmod(c,d,n,m);	         	               // 计算m = (c ^ d) mod n
	cotstr(m,szSerial);                            // 将m的16进制串表示写入szSerial中，即为注册码
    
	SetDlgItemText(hWnd, IDC_Serial,szSerial);       
    
	mirkill(c);
	mirkill(n);
	mirkill(d);
	mirkill(m);
	mirexit();
	return TRUE;
}
