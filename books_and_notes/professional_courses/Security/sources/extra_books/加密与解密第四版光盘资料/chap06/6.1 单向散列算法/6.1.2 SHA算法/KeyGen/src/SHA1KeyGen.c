/*-------------------------------------------------------
/*  《加密与解密》 第6章 加密算法   
/*   SHA1 *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  by cnbragon 2006
-------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <windows.h>
#include "resource.h"

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/
HINSTANCE	hInst;
TCHAR szConstName[]="pediy";
#define MAXINPUTLEN 200

typedef struct {
unsigned int length[2];
unsigned int h[8];
unsigned int w[80];
} sha;

#define H0 0x67452301L
#define H1 0xefcdab89L
#define H2 0x98badcfeL
#define H3 0x10325476L
#define H4 0xc3d2e1f0L

#define K0 0x5a827999L
#define K1 0x6ed9eba1L
#define K2 0x8f1bbcdcL
#define K3 0xca62c1d6L

#define PAD  0x80
#define ZERO 0

/* functions */

#define S(n,x) (((x)<<n) | ((x)>>(32-n)))

#define F0(x,y,z) ((x&y)|((~x)&z))
#define F1(x,y,z) (x^y^z)
#define F2(x,y,z) ((x&y) | (x&z)|(y&z)) 
#define F3(x,y,z) (x^y^z)

static void sha1_transform(sha *sh)
{ /* basic transformation step */
    unsigned int a,b,c,d,e,temp;
    int t;

    for (t=16;t<80;t++) sh->w[t]=S(1,sh->w[t-3]^sh->w[t-8]^sh->w[t-14]^sh->w[t-16]);
    a=sh->h[0]; b=sh->h[1]; c=sh->h[2]; d=sh->h[3]; e=sh->h[4];
    for (t=0;t<20;t++)
    { /* 20 times - mush it up */
        temp=K0+F0(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=20;t<40;t++)
    { /* 20 more times - mush it up */
        temp=K1+F1(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=40;t<60;t++)
    { /* 20 more times - mush it up */
        temp=K2+F2(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=60;t<80;t++)
    { /* 20 more times - mush it up */
        temp=K3+F3(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    sh->h[0]+=a; sh->h[1]+=b; sh->h[2]+=c;
    sh->h[3]+=d; sh->h[4]+=e;
} 

void sha1_init(sha *sh)
{ /* re-initialise */
    int i;
	
    for (i=0;i<80;i++) sh->w[i]=0L;
    sh->length[0]=sh->length[1]=0L;
	
    sh->h[0]=H0;
    sh->h[1]=H1;
    sh->h[2]=H2;
    sh->h[3]=H3;
    sh->h[4]=H4;
}

void sha1_process(sha *sh,int byte)
{ /* process the next message byte */
    int cnt;
    
    cnt=(int)((sh->length[0]/32)%16);
    
    sh->w[cnt]<<=8;
    sh->w[cnt]|=(unsigned int)(byte&0xFF);

    sh->length[0]+=8;
    if (sh->length[0]==0L) { sh->length[1]++; sh->length[0]=0L; }
    if ((sh->length[0]%512)==0) sha1_transform(sh);
}

void sha1_hash(sha *sh,char hash[20])
{ /* pad message and finish - supply digest */
    int i;
    unsigned int len0,len1;
    len0=sh->length[0];
    len1=sh->length[1];
    sha1_process(sh,PAD);
    while ((sh->length[0]%512)!=448) sha1_process(sh,ZERO);
    sh->w[14]=len1;
    sh->w[15]=len0;    
    sha1_transform(sh);
    for (i=0;i<20;i++)
    { /* convert to bytes */
        hash[i]=((sh->h[i/4]>>(8*(3-i%4))) & 0xffL);
    }
    sha1_init(sh);
}

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
	sha sh;
	long dtLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR szTeam1[]="PEDIY Forum";
	TCHAR szTeam2[]="pediy.com";
	TCHAR szSerial[24]={0};

	dtLength=GetDlgItemText(hWnd, IDC_Name, szName, sizeof(szName)/sizeof(TCHAR)+1);  
	if (dtLength==0)
	{
		SetDlgItemText(hWnd, IDC_Serial, "please input name");
		return FALSE;
	}
	
	sha1_init(&sh);
	for (i=0;i<dtLength;i++)
	{
		sha1_process(&sh,szName[i]);
	}
	sha1_hash(&sh,szHash);
	for (i=0;i<17;i++)
	{
		szBuffer[i]=szHash[i]^szTeam1[i];
	}
	
	for (;i<20;i++)
	{
		szBuffer[i]=szHash[i]^szTeam2[i-17];
	}

	for (i=0;i<10;i++)
	{
		szBuffer[i]^=szBuffer[i+0x0A];
		wsprintf((szSerial+i*2),"%02X",(unsigned char)szBuffer[i]);
	}
	
    SetDlgItemText(hWnd, IDC_Serial,szSerial);       

	return TRUE;
}
