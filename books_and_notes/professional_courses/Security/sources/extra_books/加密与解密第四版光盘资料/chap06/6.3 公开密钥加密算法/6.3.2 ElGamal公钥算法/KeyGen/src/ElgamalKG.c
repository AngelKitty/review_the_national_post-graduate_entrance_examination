/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   Elgamal *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  段钢 2002.12
 *  Modified by cnbragon 2006
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"
#include "md5c.c"
#include "global.h"
#include "miracl.h"
#include "mirdef.h"

#if _DEBUG
  #pragma comment(linker,"/NODEFAULTLIB:LIBC")
#endif

#pragma comment(lib,"ms32.lib")
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
			//GenerateSerial(hDlg);
			break;

		case IDC_About :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	

		case IDC_Gen:
			GenerateSerial(hDlg);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));
		SendDlgItemMessage(hDlg,IDC_Name,EM_LIMITTEXT,50,0);
		SendDlgItemMessage(hDlg,IDC_Name,WM_SETTEXT,0,(LPARAM)szConstName);
		                                                             

		break;
	}
     return 0;
}

BOOL GenerateSerial(HWND hWnd) 
{
	miracl* mip=mirsys(500,16);
	MD5_CTX context;
	int dtLength;
	big M,p,y,a,b,x,temp,g,bigK,bigcd;
    TCHAR szConst[]="pediy";
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	
	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  //取用户名
	if (strlen(szName)==0)//检查是否输入了用户名
	{
		SetDlgItemText(hWnd, IDC_Serial, "please input name");
		return FALSE;
	}

	MD5Init(&context);
	MD5Update(&context,szConst,3);
	MD5Update(&context,szName,dtLength);
	MD5Final(szHash,&context);


// MIRACL大数运算库运算 
//===================================================================================	
	mip->IOBASE=16;
	M=mirvar(0);
	p=mirvar(0);
	y=mirvar(0);
	x=mirvar(0);
	a=mirvar(0);
	b=mirvar(0);
	temp=mirvar(0);	
	g=mirvar(0);
	bigK=mirvar(0);
	bigcd=mirvar(0);
	
	irand(GetTickCount());
	bytes_to_big(16,szHash,M);
	
    cinstr(p,"CE892335578D3F");
	cinstr(x,"264D8D82C7AAB8");
	cinstr(g,"473FE7D24CB6A6");
	
	zero(temp);
	decr(p,1,p);
	while(1)
	{
		bigrand(p,bigK);
		if(compare(bigK,temp)==0)
		{
			continue;
		}
		else
		if(egcd(bigK,p,bigcd)==1)
		{
			break;
		}
	}
	incr(p,1,p);
	powmod(g,bigK,p,a);
	decr(p,1,p);
	multiply(x,a,temp);
	subtract(M,temp,temp);
	divide(temp,p,p);
	xgcd(bigK,p,bigK,bigK,bigK);
	multiply(bigK,temp,b);
	divide(b,p,p);
	zero(temp);
	if(compare(b,temp)==0)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Signature b is zero,please regenerate!");
		return FALSE;
	}
	    
	cotstr(a,szSerial);
	dtLength=lstrlen(szSerial);
	szSerial[dtLength++]='-';
	cotstr(b,(szSerial+dtLength));
    
	
	SetDlgItemText(hWnd, IDC_Serial, szSerial);

	mirkill(M);
	mirkill(p);
	mirkill(y);
	mirkill(x);
	mirkill(a);
    mirkill(b);
	mirkill(temp);
	mirexit();	

    return TRUE;		
}
