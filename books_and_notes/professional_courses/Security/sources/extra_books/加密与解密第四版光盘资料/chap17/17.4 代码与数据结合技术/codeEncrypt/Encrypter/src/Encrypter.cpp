/*-----------------------------------------------------------------------
第17章  软件保护技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h> 
#include "resource.h"
#include<stdlib.h>

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/

#define MAXINPUTLEN 8

BOOL    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
BOOL    PopFileOpenDlg(void);
int     encrypt(HWND);
TCHAR   szFileName[MAX_PATH ] ;
HINSTANCE hInst;

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
		EnableWindow(GetDlgItem(hDlg,IDC_ENCRYPT),FALSE);


		// 将窗口定位屏幕中央
		GetWindowRect(hDlg,&DlgRect); 
		hwindow = GetDesktopWindow(); 
		GetWindowRect(hwindow,&DesktopRect);

		DlgHeight = DlgRect.bottom - DlgRect.top;
		DlgWidth = DlgRect.right - DlgRect.left;
		x=(DesktopRect.right+DesktopRect.left-DlgWidth)/2;
		y =(DesktopRect.bottom+DesktopRect.top-DlgHeight)/2;
		MoveWindow(hDlg,x,y,DlgWidth,DlgHeight,FALSE);	 
		break;  
		
	case WM_CLOSE:
		DestroyWindow(hDlg); 		                       
		break;

	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
		case IDC_OPENFILE:
			if(PopFileOpenDlg())
			{
			SendMessage(GetDlgItem(hDlg,IDC_TXTFILE), WM_SETTEXT, 0, (LPARAM)szFileName);
			EnableWindow(GetDlgItem(hDlg,IDC_ENCRYPT),TRUE);
			}
			break;
			
		case IDC_ENCRYPT:
			encrypt(hDlg);
			break;

		case IDC_EXIT1: 
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}

		return TRUE;
		break;
	}
	return FALSE;
}
/*-------------------------------------------------------------*/
/*  PopFileOpenDlg － 打开文件                                 */
/*-------------------------------------------------------------*/
BOOL PopFileOpenDlg()
{

	static OPENFILENAME ofn ;
	static TCHAR szFilter[] =TEXT ("EXE Files (*.exe)\0*.exe\0") \
	                         TEXT ("All Files (*.*)\0*.*\0\0") ;

	szFileName[0] = '\0';                         
	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ;         
	ofn.nMaxFile          = MAX_PATH ;   
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ; 
	return GetOpenFileName (&ofn) ;
}

/*-------------------------------------------------------------*/
/*  encrypt － 对address1和address2之间的代码进行加密处理      */
/*-------------------------------------------------------------*/
int encrypt(HWND hWnd)
{

	TCHAR szBuffer[30]={0}; 
    DWORD address1,address2,offset,Size,k,nbWritten,szTemp;;
	HANDLE hFile;	
	DWORD* ptr;
	         
	hFile = CreateFile(
			szFileName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ, 
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	
	if ( hFile == INVALID_HANDLE_VALUE )
	 {
		MessageBox(NULL,"I can't access this file !","Open error",MB_ICONEXCLAMATION);
		return 1;
	}


	GetDlgItemText(hWnd,IDC_TXT1,szBuffer,sizeof(szBuffer)/sizeof(TCHAR)+1);
	 // 检查输入的是否为16进制数
	for(DWORD i=0;szBuffer[i]!=0;i++)                  
	{
		if(isxdigit(szBuffer[i])==0)
		{
			MessageBox(NULL,"请输入十六进制数!","aborted",MB_ICONEXCLAMATION);
			return 0;
		}
	}
	address1 = (DWORD)strtoul( szBuffer, NULL, 16 ); 

	GetDlgItemText(hWnd,IDC_TXT2,szBuffer,sizeof(szBuffer)/sizeof(TCHAR)+1);
	 // 检查输入的是否为16进制数
	for(i=0;szBuffer[i]!=0;i++)                 
	{
		if(isxdigit(szBuffer[i])==0)
		{
			MessageBox(NULL,"请输入十六进制数!","aborted",MB_ICONEXCLAMATION);
			return 0;
		}
	}
	address2 = (DWORD)strtoul( szBuffer, NULL, 16 ); 

	//对输入的注册码进行一定的变换，得到密钥k ，k = F（注册码）
	k=1;
	GetDlgItemText(hWnd,IDC_TXT0,szBuffer,sizeof(szBuffer)/sizeof(TCHAR)+1);
	for ( i=0;i<strlen(szBuffer);i++) 
	{
		k = k*6 + szBuffer[i];
	}

	Size=address2-address1;
	Size=Size/0x4; 	//加密时，每次异或 DWORD数据，Size是为最终需要异或的次数
	offset=address1;
 	for (i=0;i<Size;i++)
	{
		SetFilePointer(hFile,offset,NULL,FILE_BEGIN); 
		ReadFile(hFile,szBuffer, 4, &szTemp, NULL);//读取DWORD字节的文件内容
		ptr=(DWORD*)szBuffer;
		*ptr=(*ptr)^k;
		SetFilePointer(hFile,offset,NULL,FILE_BEGIN); 

		if(!WriteFile(hFile,ptr,4,&nbWritten,NULL))// 写入文件
		{
			MessageBox(NULL,"Error while patching !","Patch aborted",MB_ICONEXCLAMATION);
			CloseHandle(hFile);
		
			return 1;
		}
		offset=offset+4;
	}

	CloseHandle(hFile);
	MessageBox(NULL,"Patch successfull !","Patch",MB_ICONINFORMATION);
	return 1;

}
