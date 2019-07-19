/*-----------------------------------------------------------------------
TraceMe.cpp -- 动态调试技术练习实例 
2.1 OllyDbg调试器
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "resource.h"
#include <windows.h> 

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/

#define MAXINPUTLEN 80

INT_PTR    CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;
INT_PTR    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
INT_PTR    CALLBACK CheckDlgProc  (HWND, UINT, WPARAM, LPARAM) ;

BOOL GenRegCode( TCHAR  *rCode, TCHAR  *name ,int len) ;
TCHAR szBuffer[30];
unsigned char Table[8] = {0xC,0xA,0x13,0x9,0xC,0xB,0xA,0x8};    //计算序列号要用的数据表，全局变量

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
/* AboutDlgProc － 关于窗口                                    */
/*-------------------------------------------------------------*/

INT_PTR CALLBACK AboutDlgProc (HWND hDlg, UINT message,
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
/* CheckDlgProc － 提示信息窗口                            */
/*-------------------------------------------------------------*/

INT_PTR CALLBACK CheckDlgProc (HWND hDlg, UINT message,
                            WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
        
		SendMessage(GetDlgItem(hDlg, IDC_STATIC1), WM_SETTEXT, 0, (LPARAM)szBuffer);//初始化提示信息
        return TRUE;
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

INT_PTR CALLBACK MainDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{			

	TCHAR cName[MAXINPUTLEN]={0};
	TCHAR cCode[100]={0};
	int len;

	TCHAR szEnchar[] = TEXT ("你输入字符要大于四个！") ;
	TCHAR szSucc[] = TEXT ("恭喜你！成功！") ;
	TCHAR szFail[] = TEXT ("序列号错误，再来一次！") ;


	switch (message) 
	{
	case WM_INITDIALOG:
		SendMessage(hDlg,WM_SETICON,ICON_BIG,LPARAM(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON)))); //设置图标
		SendDlgItemMessage(hDlg, IDC_TXT0, EM_LIMITTEXT, MAXINPUTLEN, 0);   //初始化edit控件IDC_TXT0字符长度
		break; 
		
	case WM_CLOSE:
		DestroyWindow(hDlg); 		                       
		break;

	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
	
		case IDC_ABOUT :
		case IDM_HELP_ABOUT :
			DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
			break;
			
		case IDC_OK:		

			len=GetDlgItemText(hDlg,IDC_TXT0,cName,sizeof(cName)/sizeof(TCHAR)+1);
			GetDlgItemText(hDlg,IDC_TXT1,cCode,sizeof(cCode)/sizeof(TCHAR)+1);
			if (cName[0] == 0||len<5) 
			{
				lstrcpy(szBuffer,szEnchar);
				SetFocus (GetDlgItem(hDlg,IDC_TXT0));
			}

			else
			{

				
				if(GenRegCode(cCode, cName ,len))	//此处调用序列号计算的子程序
				{	lstrcpy(szBuffer,szSucc);
			    	EnableWindow(GetDlgItem(hDlg,IDC_TXT0),FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_TXT1),FALSE);
				}

				else
					lstrcpy(szBuffer,szFail);
		   	SetFocus (GetDlgItem(hDlg,IDC_TXT1));
			}
			MessageBeep (MB_OK);
			DialogBox (hInst, MAKEINTRESOURCE (IDD_CHECK), hDlg, CheckDlgProc ) ;	
			


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
BOOL GenRegCode( TCHAR  *rCode, TCHAR  *name ,int len) 
{ 
int i,j;
unsigned long code=0; 

	for(i=3,j=0;i<len;i++,j++) 
		 {if(j>7) j=0; 
		   code+=((BYTE)name[i])*Table[j]; 											 														
		  } 

	wsprintf(name,TEXT("%ld"),code);
	if(lstrcmp(rCode, name)==0)      //比较真假序列号，这里为了省事，直接比较了
		return TRUE;
	else
		return FALSE;

}


