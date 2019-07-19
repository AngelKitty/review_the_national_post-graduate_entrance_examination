/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   Elgamal KeyGenMe
/*  (c) www.PEDIY.com  by 段钢 2002.9
/*  Modified by cnbragon 2006
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"
#include "global.h"
#include "md5c.c"
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
	MD5_CTX context;
	int i,dtLength,iPos;
	char* pstr;
	big M,p,y,a,b,g,result1,result2,result3,result4,Buffer;
    TCHAR szConst[]="pediy";
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szSerial[MAXINPUTLEN]={0}; // 为了方便阅读，多定义了几个数组
	TCHAR szSerial1[MAXINPUTLEN]={0};
	TCHAR szSerial2[MAXINPUTLEN]={0};
    
	
	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  //取用户名
	if (dtLength==0)//检查是否输入了用户名
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}

	dtLength=GetDlgItemText(hWnd, IDC_Serial, szSerial, sizeof(szSerial)/sizeof(TCHAR)+1); // 取序列号
	if(dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	else if(strchr(szSerial,'-')==NULL)
	{
		SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		return FALSE;
	}
	else
	{
		pstr=strchr(szSerial,'-');
		iPos=pstr-szSerial;
	}

	for(i=0;szSerial[i]!=0;i++)  // 检查输入的序列号是否为16进制数，为cinstr(M,szSerial)使用做准备
	{
        if(i==iPos)
		{
			continue;
		}
		if(isxdigit(szSerial[i])==0)
		{
			SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
		    return FALSE;
		}
	}
	
	strncpy(szSerial1,szSerial,iPos);  // 将szSerial字符成分成szSerial1和szSerial12两组
	strncpy(szSerial2,(szSerial+iPos+1),(dtLength-iPos-1));

	MD5Init(&context);
	MD5Update(&context,szConst,3);
	MD5Update(&context,szName,lstrlen(szName));
	MD5Final(szHash, &context);//szHash中就是用户名的MD5散列值


// MIRACL大数运算库运算 
//===================================================================================	
		mip->IOBASE=16;
		M=mirvar(0);
		p=mirvar(0);
		y=mirvar(0);
		a=mirvar(0);
		b=mirvar(0);
		g=mirvar(0);
		Buffer=mirvar(0);;
		result1=mirvar(0);  // 为了让代码可读性好些，特增加了几个中间变量放结果
		result2=mirvar(0);
		result3=mirvar(0);
		result4=mirvar(0);		

		bytes_to_big(16,szHash,M);
		
		cinstr(a,szSerial1); // 转换成大数
		cinstr(b,szSerial2);
		cinstr(p,"CE892335578D3F");
		cinstr(g,"473FE7D24CB6A6");
		cinstr(y,"A3CCD85BBD896");

		powmod(y,a,p,result1); // result1=y ^ a mod p
		powmod(a,b,p,result2); // result2=a ^ b mod p
        mad(result1,result2,result1,p,p,result3) ; //result3=result1*result2 mod p，即result3=(y^a)*(a^b)(mod p)
		// MANUAL.DOC对mad的解释：
		// Function:	void mad(x,y,z,w,q,r)
		// Six big numbers x,y,z,w,q and r. On exit q=(x.y+z)/w and r contains the remainder. 
		// If w and q are not distinct variables then only the remainder is returned; 
		// if q and r are not distinct then only the quotient is returned. 
		// The addition of z is not done if x and z (or y and z) are the same.
    
		powmod(g,M,p,result4); // result4 = g^M(mode)p	

		if(compare(result3,result4)==0)  // 验证签名(y^a) *( a^b )( mod p ) = g^M ( mod p )
		{
			SetDlgItemText(hWnd,IDC_Serial,"Success!");
			return TRUE;
		}
		else
		{
			SetDlgItemText(hWnd, IDC_Serial, "Wrong Serial!");
			return FALSE;
		}
		 
		mirkill(M);
		mirkill(p);
		mirkill(y);
		mirkill(a);
		mirkill(b);
		mirkill(g);
		mirkill(Buffer);
		mirkill(result1);
		mirkill(result2);
		mirkill(result3);
		mirkill(result4);		
		mirexit();		
		return TRUE;
}
