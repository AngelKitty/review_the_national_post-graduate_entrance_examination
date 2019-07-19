
/*******************************************************
《加密与解密》第三版配套实例 2007.10 code by kanxue

  5.1.4  注册机制作
(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

#include <windows.h>
#include <stdio.h>
#include "resource.h"


/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/

HINSTANCE hInst;
#define MAXINPUTLEN 10

TCHAR cName[MAXINPUTLEN]={0};
/*-------------------------------------------------------------*/
/*  函数声明                                                   */
/*-------------------------------------------------------------*/
BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;

BOOL GenRegCode( HWND) ;


/*-------------------------------------------------------------*/
/* WinMain － 基于WIN32的程序的入口                            */
/*-------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;
	DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, MainDlg, NULL);
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

BOOL CALLBACK MainDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{			
	switch (message) 
	{
	case WM_INITDIALOG:
		int DlgHeight,DlgWidth,x,y;
		RECT DlgRect;
		RECT DesktopRect;
		HWND hwindow;
		SendMessage(hDlg,WM_SETICON,ICON_BIG,LPARAM(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON)))); //设置图标
		SendDlgItemMessage(hDlg, IDC_TXT0, EM_LIMITTEXT, MAXINPUTLEN, 0);      //初始化edit控件IDC_TXT0字符长度
		// 将窗口定位屏幕中央
		GetWindowRect(hDlg,&DlgRect); 
		hwindow = GetDesktopWindow(); 
		GetWindowRect(hwindow,&DesktopRect);
		
		DlgHeight = DlgRect.bottom - DlgRect.top;
		DlgWidth = DlgRect.right - DlgRect.left;
		x=(DesktopRect.right+DesktopRect.left-DlgWidth)/2;
		y =(DesktopRect.bottom+DesktopRect.top-DlgHeight)/2;
		MoveWindow(hDlg,x,y,DlgWidth,DlgHeight,FALSE);
		
		
		return TRUE;	 
		break; 
		
	case WM_CLOSE:
		EndDialog(hDlg,0);
		
		return TRUE;
		break;
		
	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
			
		case IDC_ABOUT :
		case IDM_HELP_ABOUT :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;	
		case IDC_OK:
			if(!GenRegCode(hDlg)) 
				MessageBox(hDlg,"输入字符中不能有数字！","Error",MB_ICONEXCLAMATION);
			SetFocus (GetDlgItem(hDlg,IDC_TXT0));
			
			break;
		case IDC_EXIT: 
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		
		return TRUE;
		break;
	}
	return FALSE;
}

/*-------------------------------------------------------------*/
/* GenRegCode － 注册算法主函数                                */
/*-------------------------------------------------------------*/
BOOL GenRegCode( HWND hwnd) 
{
	TCHAR cCode[100]={0};
	
	GetDlgItemText(hwnd,IDC_TXT0,cName,sizeof(cName)/sizeof(TCHAR)+1);
	if (strlen(cName)==0)
	{
		
		MessageBox(hwnd,"请输入姓名！","Error",MB_ICONEXCLAMATION);
		return TRUE;
	}
	int k1=0,k2=0;
	BOOL bIsnum=FALSE;
		
	__asm
	{
     	mov     esi,OFFSET  cName //注意：cName[]必须为全局变量数组
     	push    esi
     L002:
       	mov     al, byte ptr [esi]
       	test    al, al
       	je L014
       	cmp     al, 0x41
       	jb L019
       	cmp     al, 0x5A
       	jnb L011
       	inc     esi
       	jmp L002
     L011:
      	call L035
      	inc     esi
       	jmp L002
     L014:
       	pop     esi
       	call L026
      	xor     edi, 0x5678
        mov     eax, edi
       	jmp L025
     L019:
       	pop     esi
		mov   bIsnum,1
     L025:
       	jmp LEND
     L026:
       	xor     edi, edi
       	xor     ebx, ebx
     L028:
       	mov     bl, byte ptr [esi]
       	test    bl, bl
       	je L034
       	add     edi, ebx
       	inc     esi
       	jmp L028
     L034:
       	retn
     L035:
       	sub     al, 0x20
       	mov     byte ptr [esi], al
       	retn
     LEND:
       	mov k1,eax
	}
	if(bIsnum)
		return FALSE;
	k2=k1^0x1234;  
	wsprintf(cCode,TEXT("%ld"),k2);
	SetDlgItemText(hwnd,IDC_TXT1,cCode);
	return TRUE;	
	
}



