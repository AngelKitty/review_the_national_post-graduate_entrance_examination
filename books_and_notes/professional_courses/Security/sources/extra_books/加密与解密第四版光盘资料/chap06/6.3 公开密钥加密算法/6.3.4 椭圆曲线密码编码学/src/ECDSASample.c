/*-------------------------------------------------------
/*  《加密与解密》第三版 第7章 加密算法   
/*   ECDSASample
/*  (c) www.PEDIY.com 
 *  Coded by cnbragon 2007
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
	big big_e,big_a,big_b,big_n,big_q,big_px,big_py,big_d,big_r,big_s,big_temp,big_k,big_zero;
	epoint *pt_kP, *pt_P;
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
	big_e=mirvar(0);
	big_a=mirvar(-3);
	big_b=mirvar(0);
	big_r=mirvar(0);
	big_s=mirvar(0);
	big_temp=mirvar(0);	
	big_px=mirvar(0);
	big_py=mirvar(0);
	big_q=mirvar(0);
	big_d=mirvar(0);
	big_k=mirvar(0);
	big_n=mirvar(0);
	big_zero=mirvar(0);
	pt_P=epoint_init();
	pt_kP=epoint_init();
	
	irand(GetTickCount());
	bytes_to_big(20,szHash,big_e);
	
    cinstr(big_b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1");
	cinstr(big_q,"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF");
	cinstr(big_px,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012");
	cinstr(big_py,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811");
	cinstr(big_n,"FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831");
	cinstr(big_d,"97B27EA7BB8A6639601888965DC22BA3287E31D9");

	ecurve_init(big_a,big_b,big_q,MR_PROJECTIVE);
	while(1)
	{
		decr(big_n,1,big_n);
		bigrand(big_n,big_k);
		incr(big_n,1,big_n);
		if (compare(big_k,big_zero)==0) // 随机数k不能为0
		{
			continue;
		}
		else
		{
			epoint_set(big_px,big_py,1,pt_P);
			ecurve_mult(big_k,pt_P,pt_kP);
			epoint_get(pt_kP,big_r,big_r);
			divide(big_r,big_n,big_n);
			if (compare(big_r,big_zero)==0) // 签名r不能为0
			{
				continue;
			} 
			else
			{
				xgcd(big_k,big_n,big_k,big_k,big_k); // 求出k的逆元
				mad(big_d,big_r,big_e,big_n,big_n,big_temp); // temp=e+dr mod n
				mad(big_k,big_temp,big_temp,big_n,big_n,big_s); // s=k^-1.temp mod n
				if (compare(big_s,big_zero)==0) // 签名s不能为0
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

	epoint_free(pt_P);
	epoint_free(pt_kP);
	mirkill(big_e);
	mirkill(big_a);
	mirkill(big_b);
	mirkill(big_q);
	mirkill(big_r);
    mirkill(big_s);
	mirkill(big_d);
	mirkill(big_temp);
	mirkill(big_n);
	mirkill(big_k);
	mirkill(big_px);
    mirkill(big_py);
	mirkill(big_zero);
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
	big big_a,big_b,big_q,big_n,big_px,big_py,big_qx,big_qy,big_e;
	big big_w,big_s,big_u1,big_u2,big_r,big_v;
	epoint *pt_P, *pt_Q, *pt_X;

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
	big_s=mirvar(0);
	big_r=mirvar(0);
    big_n=mirvar(0);

	cinstr(big_n,"FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831");
		
	cinstr(big_r,szSig);
	if (compare(big_r,big_n)>=0) // 若r大于等于n,则签名无效
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		mirkill(big_r);
		mirkill(big_n);
		mirexit();
		return FALSE;
	}
	memset(szSig,0,MAXINPUTLEN);
	iLen=GetDlgItemText(hWnd,IDE_S,szSig,sizeof(szSig)/sizeof(TCHAR)+1);
	if(strlen(szSig)==0)
	{
		MessageBox(hWnd,"No Signature!","Verify failded",MB_OK);
		return FALSE;
	}
    big_s=mirvar(0);
	cinstr(big_s,szSig);
    if (compare(big_s,big_n)>=0) //若s大于等于n，则签名无效
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		mirkill(big_s);
		mirkill(big_r);
		mirkill(big_n);
		mirexit();
		return FALSE;
	}

	big_a=mirvar(-3);
	big_b=mirvar(0);
	big_q=mirvar(0);
	big_px=mirvar(0);
	big_py=mirvar(0);
	big_qx=mirvar(0);
	big_qy=mirvar(0);
	big_e=mirvar(0);
	big_w=mirvar(0);
	big_u1=mirvar(0);
	big_u2=mirvar(0);
	big_v=mirvar(0);

	bytes_to_big(20,szHash,big_e);
	cinstr(big_b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1");
	cinstr(big_q,"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF");
	cinstr(big_px,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012");
	cinstr(big_py,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811");
	cinstr(big_qx,"9DD9DB8B71E342CA10652144B4FA3BFAFF854987CFBED260");
	cinstr(big_qy,"E5F30E201B0FF7F644BD6BA0313EF793A8CFA7D86CBD3DBF");
	
	xgcd(big_s,big_n,big_s,big_s,big_s); // 求出s的逆元
	copy(big_s,big_w);
	mad(big_e,big_w,big_w,big_n,big_n,big_u1); // u1=ew mod n
	mad(big_r,big_w,big_w,big_n,big_n,big_u2); // u2=rw mod n
	pt_P=epoint_init();
	pt_Q=epoint_init();
	pt_X=epoint_init();
	ecurve_init(big_a,big_b,big_q,MR_PROJECTIVE);
	epoint_set(big_px,big_py,1,pt_P);
	epoint_set(big_qx,big_qy,1,pt_Q);
	ecurve_mult2(big_u1,pt_P,big_u2,pt_Q,pt_X);
	if (point_at_infinity(pt_X)==TRUE)
	{
		MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
	} 
	else
	{
		epoint_get(pt_X,big_v,big_v);
		divide(big_v,big_n,big_v);
		if (compare(big_v,big_r)==FALSE)
		{
			MessageBox(hWnd,"Not Valid Signature!","Verify failded",MB_OK);
		} 
		else
		{
			MessageBox(hWnd,"Valid Signature!","Verify Success",MB_OK);
		}
	}
	
	epoint_free(pt_P);
	epoint_free(pt_Q);
	epoint_free(pt_X);
	mirkill(big_a);
	mirkill(big_b);
	mirkill(big_q);
	mirkill(big_n);
	mirkill(big_px);
	mirkill(big_py);
	mirkill(big_qx);
	mirkill(big_qy);
	mirkill(big_e);
	mirkill(big_w);
	mirkill(big_s);
	mirkill(big_u1);
	mirkill(big_u2);
	mirkill(big_r);
	mirkill(big_v);
	mirexit();
	return TRUE;
}