/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   MD5计算器源码
/*  (c) www.PEDIY.com  by 段钢 2002.9
/ *  Modified by cnbragon 2006
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"

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
	TCHAR szWaterMark[]="Modified by cnbragon belong to iNsidE PaNdoRa's BoX and Reverse Code Team";
	miracl* mip=mirsys(500,16);
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
    long dtLength,lsLength;
	int i;
	big n,e,c,m;

	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	lsLength=GetDlgItemText(hWnd,IDC_Serial,szSerial,sizeof(szSerial)/sizeof(TCHAR)+1);
	if (lsLength==0)
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
	
	for(i=0;szSerial[i]!=0;i++)      // 检查输入的序列号是否为16进制数，为cinstr(M,szSerial)使用做准备
	{
		if(isxdigit(szSerial[i])==0)
		{
			SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
			return FALSE;
		}
	}
    
	/* MIRACL大数运算库运算 
     * p=C75CB54BEDFA30AB
     * q=A554665CC62120D3
     * n=80C07AFC9D25404D6555B9ACF3567CF1
     * d=651A40B9739117EF505DBC33EB8F442D
     * e=10001
     * 128 bit
	 */
	mip->IOBASE=16;               // 设定16进制模式	                            
	n=mirvar(0);                  // 初始化变量 
	e=mirvar(0);     
	m=mirvar(0);                  // m 放明文：注册码
	c=mirvar(0);                  // c 放密文

	cinstr(m,szSerial);	                           // 将输入的序列号转换成大数 ，这里szSerial	
	cinstr(n,"80C07AFC9D25404D6555B9ACF3567CF1");  // 初始化模数n	 
	cinstr(e,"10001");  

	if(compare(m,n)==-1)                           // m < n ,才能对消息m加密
	{
		powmod(m,e,n,c);                               // 计算明文 c= m ^ e mod n

		big_to_bytes(0,c,szBuffer,FALSE);                 // 将 c 从大数转换成字节数组    
                              
		mirkill(n);
		mirkill(e);
		mirkill(m);
		mirkill(c);  
		mirexit();

		if(lstrcmp(szName, szBuffer)!=0)               // 比较姓名与序列号加密后数据的是否相等？
		{
			SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
			return FALSE;
		}
		else
		{
			SetDlgItemText(hWnd,IDC_Serial,"Success!");
			return TRUE;
		}
	}
	else
	{
		SetDlgItemText(hWnd,IDC_Serial,"Wrong Serial!");
		return FALSE;
	}
}
