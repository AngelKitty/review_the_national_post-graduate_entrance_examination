/*-------------------------------------------------------
/*  《加密与解密》第三版 第6章 加密算法   
/*   IDEA *KeyGenMe* 's KeyGen
/*  (c) www.PEDIY.com  by cnbragon 2007
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

/* IDEA Definition Begin */
#define maxim 	65537
#define fuyi	65536
#define one 	65535
#define round	8
	
/* multiplication using the Low-High algorithm */

unsigned mul(unsigned a,unsigned b) 
{
	long int p;
	long unsigned q;
		if(a==0) p=maxim-b;
		else
		if(b==0) p=maxim-a;
		else {
		q=(unsigned long)a*(unsigned long)b;
		p=(q & one) - (q>>16); 
		if(p<=0) p=p+maxim;
		}
	return (unsigned)(p&one);
}

/* encrypt algorithm */
void cip(unsigned short in[5],unsigned short out[5],unsigned short Z[7][10]) 
{
	unsigned r,x1,x2,x3,x4,kk,t1,t2,a;
	x1=in[1]; x2=in[2]; x3=in[3]; x4=in[4];
	for(r=1;r<=8;r++) 			/* the round function */
	{
			/* the group operation on 64-bits block */
	x1 = mul(x1,Z[1][r]);		x4 = mul(x4,Z[4][r]);
	x2 = (x2 + Z[2][r]) & one;	x3 = (x3 + Z[3][r]) & one;
			/* the function of the MA structure */
	kk = mul(Z[5][r],(x1^x3));
	t1 = mul(Z[6][r],(kk+(x2^x4)) & one);
	t2 = (kk+t1) & one;
			/* the involutary permutation PI */
	x1 = x1^t1;		x4=x4^t2;
	a  = x2^t2;		x2=x3^t1;	x3=a;
	}

		/* the output transformation */
	out[1] = mul(x1,Z[1][round+1]);
	out[4] = mul(x4,Z[4][round+1]);
	out[2] = (x3+Z[2][round+1]) & one;
	out[3] = (x2+Z[3][round+1]) & one;
}



/* generate encryption subkeys Z's */

void key(unsigned short uskey[9], unsigned short Z[7][10]) 
{
	unsigned short S[54];
	int i,j,r;
	for(i=1;i<9;i++) S[i-1]=uskey[i];
		/* shifts */
	for(i=8;i<54;i++)
		{
			if((i+2)%8 == 0)			/* for S[14],S[22],... */
				S[i] = ((S[i-7]<<9) ^ (S[i-14]>>7)) & one;
			else if((i+1)%8==0)			/* for S[15],S[23],... */
				S[i] = ((S[i-15]<<9) ^ (S[i-14]>>7)) & one;
			else
				S[i] = ((S[i-7]<<9) ^ (S[i-6]>>7)) & one;
		}

	/* get subkeys */

	for(r=1;r<=round+1;r++) 
	 for(j=1;j<7;j++)
		Z[j][r]=S[6*(r-1)+j-1];
}

/* compute inverse of xin by Euclidean gcd alg. */

unsigned inv(unsigned xin)
{
	long n1,n2,q,r,b1,b2,t;
	if(xin==0) b2=0;
	else
	{ n1=maxim; n2=xin; b2=1; b1=0;
		do { r = (n1 % n2); q = (n1-r)/n2;
			 if(r==0) { if(b2<0) b2=maxim+b2; }
			 else { n1=n2; n2=r; t=b2; b2=b1-q*b2; b1=t; }
		   } while (r!=0);
	}
	return (unsigned)b2;
}

/* compute decryption subkeys DK's */

void de_key(unsigned short Z[7][10],unsigned short DK[7][10])
{
	int j;
	for(j=1;j<=round+1;j++)
	{
		DK[1][round-j+2] = inv(Z[1][j]);
		DK[4][round-j+2] = inv(Z[4][j]);
	
		if (j==1 || j==round+1) {
			DK[2][round-j+2] = (fuyi-Z[2][j]) & one;
			DK[3][round-j+2] = (fuyi-Z[3][j]) & one;
		} else {
			DK[2][round-j+2] = (fuyi-Z[3][j]) & one;
			DK[3][round-j+2] = (fuyi-Z[2][j]) & one;
		}
	}

	for(j=1;j<=round+1;j++)
	{ DK[5][round+1-j]=Z[5][j];
	  DK[6][round+1-j]=Z[6][j];
	}
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

BOOL GenerateSerial(HWND hWnd) 
{
	sha sh;
	long dtLength;
	int i;
	TCHAR szName[MAXINPUTLEN]={0};
	TCHAR szHash[MAXINPUTLEN]={0};
	TCHAR szBuffer[MAXINPUTLEN]={0};
	TCHAR sztemp[MAXINPUTLEN]={0};
    WORD wiDeaKey[9]={0};
	WORD wSubKey[7][10]={0};
	WORD wDK[7][10]={0};
	WORD wSN[5]={0};
	WORD wCipherText[5]={0};
	DWORD dwVolumeSn=0;

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
	
	GetVolumeInformation("C:\\",NULL,0,&dwVolumeSn,NULL,NULL,NULL,0);
	memcpy((wiDeaKey+1),szHash,16);
	memcpy((wCipherText+1),(szHash+16),4);
	memcpy((wCipherText+3),&dwVolumeSn,4);
	key(wiDeaKey,wSubKey);
	de_key(wSubKey,wDK);
	cip(wCipherText,wSN,wDK);
	for (i=0;i<4;i++)
	{
		sprintf((sztemp+i*4),"%04X",(WORD)wSN[i+1]);
	}
    SetDlgItemText(hWnd, IDC_Serial,sztemp);       

	return TRUE;
}
