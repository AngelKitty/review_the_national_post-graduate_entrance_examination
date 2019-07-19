/*-------------------------------------------------------
/*  RSA 1024 演示  
/* 《加密与解密》第三版
/* http://www.pediy.com
/*  (c) code by kanxue 2005.9
-------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* 本程序调用了MIRACL 大数运算库     */
/*-----------------------------------------------------------*/

////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "resource.h"
#include <ctype.h>



#include "miracl.h"
#include "mirdef.h"

#pragma comment( lib, "ms32.lib") 

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/
HINSTANCE	hInst;
#define MAXINPUTLEN 1200

/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    Decrypt( HWND) ;

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
		case IDC_OK:
			SetFocus (GetDlgItem(hDlg,IDC_TXT0));
			Decrypt(hDlg);
			break;
		case IDC_EXIT:
			PostQuitMessage(0);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,(WPARAM) 1,(LPARAM) LoadIconA(hInst,MAKEINTRESOURCE(IDI_ICON)));
		break;
	}
     return 0;
}

BOOL Decrypt( HWND hWnd) 
{
  int  len,i;  
  big n,d,c,m;
  miracl *mip=mirsys(100,0);

  TCHAR szData[MAXINPUTLEN]={0};
  TCHAR szDecryptText[MAXINPUTLEN]={0};
  TCHAR szBuffer[MAXINPUTLEN]={"0"};

  len=GetDlgItemText(hWnd, IDC_TXT0, szData, sizeof(szData)/sizeof(TCHAR)+1); 
  if (strlen(szData)==0)
  {
    SetDlgItemText(hWnd, IDC_TXT1, "请输入字符");
    return FALSE;
  }
  for(i=0;i<len;i++)
  {
	  if(isxdigit(szData[i])==0)
	  {
		  SetDlgItemText(hWnd, IDC_TXT1, "请输入16进制字符");
		  return FALSE;
	  }
  }

  


// MIRACL大数运算库运算 
//===================================================================================
// p=0xCA8A8D7BA04B5D799CFCDFAA687F85ED1D988E2DB3DA02383C160F3722EADAFB7967C272AAF8A66A92DF6EEE79C2A9E4ADC9E0E0591000F7F89838E5D94F97B1
// q=0xDB016D3633816DA63EB9ABEA98CAC6735C3CAAEEB9C6B7FE7B207ACD7870471DA51EE7AEBEE772C7D058A1009B2883ED6AB895B102460F0D04F4F7AD7D08A7FD
// n=0xAD45A7FB2FD39557BF690F3E230ECDB52AA90BBFAD7F21AD490C9B432DBB08FF6D900D9B2F50BEA69D9D5FA364FCF537998CF07ECEFA35F83EE3CB60BB5914ABCAFFB114605B6EF3EF2110C2AC5DCBBEB3417AE3D791CBABF32064B39BCD0318E19538FD4DF41B9AB143CDAB57F6AACC3BD6B7EDD12579FDFCB21D15DA2560ED
// d=0x65ECBD2A585E57D9436AF9E850DB881037364325751D9B56DFAD10096631C90EB8EB534C39F342F898F32932F01C5402968F240E5BA23DDD522BA4CF7D437591433501DF9DF98D8BCF2F99A264A20EAFE58AF9845C6FAFEA9BB6BCDF69AC663C1862EA9F771AD43D5B15F72372BB76B141F5539FCCD45C7459D97C1089C3F571
// e=0x11
// 1024 bit RSA
       
  mip->IOBASE=16;                                // 16进制模式
  c=mirvar(0);                                   // MIRACL的大数类型
  n=mirvar(0);
  d=mirvar(0);
  m=mirvar(0);

  cinstr(c,szData);
  cinstr(n,"AD45A7FB2FD39557BF690F3E230ECDB52AA90BBFAD7F21AD490C9B432DBB08FF6D900D9B2F50BEA69D9D5FA364FCF537998CF07ECEFA35F83EE3CB60BB5914ABCAFFB114605B6EF3EF2110C2AC5DCBBEB3417AE3D791CBABF32064B39BCD0318E19538FD4DF41B9AB143CDAB57F6AACC3BD6B7EDD12579FDFCB21D15DA2560ED");  // 初始化模数n   
  cinstr(d,"65ECBD2A585E57D9436AF9E850DB881037364325751D9B56DFAD10096631C90EB8EB534C39F342F898F32932F01C5402968F240E5BA23DDD522BA4CF7D437591433501DF9DF98D8BCF2F99A264A20EAFE58AF9845C6FAFEA9BB6BCDF69AC663C1862EA9F771AD43D5B15F72372BB76B141F5539FCCD45C7459D97C1089C3F571");  // 初始化私钥d
  powmod(c,d,n,m);                            // 计算m = (c ^ d) mod n
  big_to_bytes(0,m,szDecryptText,0); 

  mirkill(c);
  mirkill(n);
  mirkill(d);
  mirkill(m);
  mirexit();
   SetDlgItemText(hWnd, IDC_TXT1,szDecryptText);       // 显示

  return TRUE;
}
