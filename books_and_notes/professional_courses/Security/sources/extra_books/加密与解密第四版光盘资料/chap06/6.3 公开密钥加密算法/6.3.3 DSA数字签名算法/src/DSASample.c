/*-------------------------------------------------------
/*  《加密与解密》第三版 第6章 加密算法   
/*   DSASample
/*  (c) www.PEDIY.com 
 *  Coded by cnbragon 2007
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"
#if _DEBUG
  #pragma comment(linker,"/NODEFAULTLIB:LIBC")
#endif


#include "miracl.h"
#include "mirdef.h"

#pragma comment( lib, "ms32.lib") 

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
BOOL    GenerateSig( HWND) ;
BOOL    VerifySig(HWND);

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
		case IDB_Very:		
			VerifySig(hDlg);
			break;

		case IDB_About :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	

		case IDB_Sig:
			GenerateSig(hDlg);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));
		SendDlgItemMessage(hDlg,IDE_Name,EM_LIMITTEXT,50,0);
		SendDlgItemMessage(hDlg,IDE_Name,WM_SETTEXT,0,(LPARAM)szConstName);
		                                                             

		break;
	}
     return 0;
}

BOOL GenerateSig(HWND hWnd) 
{
	TCHAR szWaterMark[]="Coded by cnbragon belong to iNsidE PaNdoRa's BoX and Reverse Code Team";
	miracl* mip=mirsys(800,16);
	sha psh; 
	int dtLength,i;
	big big_m,big_p,big_r,big_s,big_x,big_temp,big_g,big_k,big_q;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szSig[MAXINPUTLEN]={0};
	
	dtLength=GetDlgItemText(hWnd, IDE_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);
	if (strlen(szName)==0)
	{
		SetDlgItemText(hWnd, IDE_Sig, "please input name");
		return FALSE;
	}

	shs_init(&psh);
	for (i=0;i<dtLength;i++)
	{
		shs_process(&psh,(int)szName[i]);
	}
	shs_hash(&psh,szHash);
	
	mip->IOBASE=16;
	big_m=mirvar(0);
	big_p=mirvar(0);
	big_x=mirvar(0);
	big_r=mirvar(0);
	big_s=mirvar(0);
	big_temp=mirvar(0);	
	big_g=mirvar(0);
	big_k=mirvar(0);
	big_q=mirvar(0);
	
	irand(GetTickCount());
	bytes_to_big(20,szHash,big_m);
	
    cinstr(big_p,"8df2a494492276aa3d25759bb06869cbeac0d83afb8d0cf7cbb8324f0d7882e5d0762fc5b7210eafc2e9adac32ab7aac49693dfbf83724c2ec0736ee31c80291");
	cinstr(big_x,"2070b3223dba372fde1c0ffc7b2e3b498b260614");
	cinstr(big_g,"626d027839ea0a13413163a55b4cb500299d5522956cefcb3bff10f399ce2c2e71cb9de5fa24babf58e5b79521925c9cc42e9f6f464b088cc572af53e6d78802");
	cinstr(big_q,"c773218c737ec8ee993b4f2ded30f48edace915f");

	while(1)
	{
		bigrand(big_q,big_k);
		if (compare(big_k,big_temp)==0) // 随机数k不能为0
		{
			continue;
		}
		else
		{
			powmod(big_g,big_k,big_p,big_r);
			divide(big_r,big_q,big_q);
			if (compare(big_r,big_temp)==0) // 签名r不能为0
			{
				continue;
			} 
			else
			{
				xgcd(big_k,big_q,big_k,big_k,big_k); // 求出k的逆元
				mad(big_x,big_r,big_m,big_q,big_q,big_temp); // temp=H(m)+xr mod q
				mad(big_k,big_temp,big_temp,big_q,big_q,big_s); // s=k^-1.temp mod q
				zero(big_temp);
				if (compare(big_s,big_temp)==0) // 签名s不能为0
				{
					continue;
				} 
				else
				{
					break;
				}
			}
			break;
		}
	}
			    
	cotstr(big_r,szSig);
	SetDlgItemText(hWnd, IDE_Sig, szSig);
	memset(szSig,0,MAXINPUTLEN);
	cotstr(big_s,szSig);
    
	SetDlgItemText(hWnd, IDE_S, szSig);

	mirkill(big_m);
	mirkill(big_p);
	mirkill(big_r);
    mirkill(big_s);
	mirkill(big_x);
	mirkill(big_temp);
	mirkill(big_g);
	mirkill(big_k);
	mirkill(big_q);
	mirexit();	

    return TRUE;		
}

BOOL VerifySig(HWND hWnd)
{
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szSig[MAXINPUTLEN]={0};
	int iLen,i;
	sha psh;
    miracl* mip=mirsys(800,16);
	big big_w,big_s,big_q,big_u1,big_u2,big_g,big_y,big_p,big_v,big_r,big_m;

	iLen=GetDlgItemText(hWnd,IDE_Name,szName,sizeof(szName)/sizeof(TCHAR)+1);
	if (strlen(szName)==0)
	{
		MessageBox(hWnd,"No Message input!","Verify failded",MB_OK);
		return FALSE;
	}

    iLen=GetDlgItemText(hWnd,IDE_Sig,szSig,sizeof(szSig)/sizeof(TCHAR)+1);
	if(strlen(szSig)==0)
	{
		MessageBox(hWnd,"No Signature!","Verify failded",MB_OK);
		return FALSE;
	}
        
	shs_init(&psh);
	for (i=0;i<lstrlen(szName);i++)
	{
		shs_process(&psh,(int)szName[i]);
	}
	shs_hash(&psh,szHash);
    
	mip->IOBASE=16;
	big_w=mirvar(0);
	big_s=mirvar(0);
	big_q=mirvar(0);
	big_u1=mirvar(0);
	big_u2=mirvar(0);
	big_g=mirvar(0);
	big_y=mirvar(0);
	big_p=mirvar(0);
	big_v=mirvar(0);
	big_r=mirvar(0);
	big_m=mirvar(0);
    
	cinstr(big_p,"8df2a494492276aa3d25759bb06869cbeac0d83afb8d0cf7cbb8324f0d7882e5d0762fc5b7210eafc2e9adac32ab7aac49693dfbf83724c2ec0736ee31c80291");
	cinstr(big_q,"c773218c737ec8ee993b4f2ded30f48edace915f");
	cinstr(big_g,"626d027839ea0a13413163a55b4cb500299d5522956cefcb3bff10f399ce2c2e71cb9de5fa24babf58e5b79521925c9cc42e9f6f464b088cc572af53e6d78802");
	cinstr(big_y,"19131871d75b1612a819f29d78d1b0d7346f7aa77bb62a859bfd6c5675da9d212d3a36ef1672ef660b8c7c255cc0ec74858fba33f44c06699630a76b030ee333");
		
	cinstr(big_r,szSig);
	if (compare(big_r,big_q)>=0) // 若r大于等于q,则签名无效
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		return FALSE;
	}
	memset(szSig,0,MAXINPUTLEN);
	iLen=GetDlgItemText(hWnd,IDE_S,szSig,sizeof(szSig)/sizeof(TCHAR)+1);
	if(strlen(szSig)==0)
	{
		MessageBox(hWnd,"No Signature!","Verify failded",MB_OK);
		return FALSE;
	}
    
	cinstr(big_s,szSig);
    if (compare(big_s,big_q)>=0) //若s大于等于q，则签名无效
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		return FALSE;
	}
	bytes_to_big(20,szHash,big_m);
	
	xgcd(big_s,big_q,big_s,big_s,big_s); // 求出s的逆元
	copy(big_s,big_w);
	mad(big_m,big_w,big_w,big_q,big_q,big_u1); // u1=H(m)w mod q
	mad(big_r,big_w,big_w,big_q,big_q,big_u2); // u2=rw mod q
	powmod2(big_g,big_u1,big_y,big_u2,big_p,big_v); // v=g^u1.y^u2 mod p
	divide(big_v,big_q,big_q);  // v = v mod q
	if (compare(big_v,big_r)==0)
	{
		MessageBox(hWnd,"Valid Signature!","Verify Success...",MB_OK);
		return TRUE;
	} 
	else
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		return FALSE;
	}
	mirkill(big_w);
	mirkill(big_s);
	mirkill(big_q);
	mirkill(big_u1);
	mirkill(big_u2);
	mirkill(big_g);
	mirkill(big_y);
	mirkill(big_p);
	mirkill(big_v);
	mirkill(big_r);
	mirkill(big_m);
	mirexit();
	return TRUE;
}