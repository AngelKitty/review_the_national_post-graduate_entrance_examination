
/*-----------------------------------------------------------------------
第17章  软件保护技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include "resource.h"

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/
#define ID_EDIT     1
#define MAXINPUTLEN 8

LRESULT CALLBACK WndProc  (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK RegisterDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK MenuOpen(HWND) ;
void    Decrypt (DWORD* ,DWORD,DWORD );

BOOL    bSucceed=FALSE;
TCHAR   cCode[30]={0}; 

/*-------------------------------------------------------------*/
/* WinMain － 基于WIN32的程序的入口                            */
/*-------------------------------------------------------------*/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("PEDIY") ;
     MSG          msg ;
     HWND         hwnd ;
     WNDCLASS     wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON)) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("About Box Demo Program"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ; 
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static HINSTANCE hInstance ;
	 static HWND hwndEdit ;
     
     switch (message)
     {
	 case WM_CREATE :
		  hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
          hwndEdit = CreateWindow (TEXT ("edit"), NULL,
                         WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                                   WS_BORDER | ES_LEFT | ES_MULTILINE |
                                   ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                         0, 0, 0, 0, hwnd, (HMENU) ID_EDIT,
                         ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;
           return 0 ;
          
     case WM_SETFOCUS :
          SetFocus (hwndEdit) ;	  
          return 0 ;
          
     case WM_SIZE : 
          MoveWindow (hwndEdit, 0, 0, LOWORD (lParam), HIWORD (lParam), TRUE) ;
          return 0 ;

     case WM_COMMAND :
          switch (LOWORD (wParam))
          {
		  case IDM_APP_OPEN:
			   MenuOpen(hwndEdit);
	           break ;
		  case IDM_APP_EXIT:
			   SendMessage (hwnd, WM_CLOSE, 0, 0) ;
               break ;
		  case IDM_APP_REGISTER:
			   DialogBox (hInstance, TEXT ("REGISTERBOX"), hwnd, RegisterDlgProc) ;
			   break ;

          case IDM_APP_ABOUT :
			   MessageBoxA (NULL, TEXT ("代码与数据结合技术     (c) 段钢, 2003"), TEXT ("《软件加密技术内幕》"), 0) ;          
               break ;
          }
          return 0 ;
          
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

/*-------------------------------------------------------------*/
/* RegisterDlgProc － 注册对话窗口                            */
/*-------------------------------------------------------------*/
BOOL CALLBACK RegisterDlgProc (HWND hDlg, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{

     switch (message)
     {
     case WM_INITDIALOG :
		  //初始化edit控件IDC_TXT0字符长度
		  SendDlgItemMessage(hDlg, IDC_TXT0, EM_LIMITTEXT, MAXINPUTLEN, 0);
          return TRUE ;
          
     case WM_COMMAND :
          switch (LOWORD (wParam))
          {
          case IDOK :
			   GetDlgItemText(hDlg,IDC_TXT0,cCode,sizeof(cCode)/sizeof(TCHAR)+1);
			   MessageBoxA (NULL, TEXT ("感谢你注册，如注册码正确则打开功能可用!"), 
				          TEXT ("(c) 段钢, 2002"), 0) ;    
          case IDCANCEL :
               EndDialog (hDlg, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

/*-------------------------------------------------------------*/
/* MenuOpen－ 菜单功能，打开文件                               */
/*-------------------------------------------------------------*/
BOOL CALLBACK MenuOpen(HWND hWnd)  
{
	
	HANDLE hFile;	
	DWORD szTemp,address1,address2,Size,k;
	DWORD* ptr;
	long FileSize;
	static OPENFILENAME ofn ;
	TCHAR  *pBuffer = new TCHAR [0x10000];     // 申请64K内存
	TCHAR szFileName[MAX_PATH ], szTitleName[MAX_PATH ] ;
	static TCHAR szFilter[] =TEXT ("TXT Files (*.TXT)\0*.txt\0") \
				TEXT ("All Files (*.*)\0*.*\0\0") ;

	szFileName[0] = '\0';                         
	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hwndOwner         = hWnd ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ;         
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = szTitleName ;          
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;

	
	_asm mov address1,offset begindecrypt   // 取待加密代码首地址
    _asm mov address2,offset enddecrypt     // 取待加密代码末地址

	//对输入的注册码进行一定的变换，得到密钥k ，k = F（注册码）
	k=1;
	for (unsigned int i=0;i<strlen(cCode);i++) 
	{
		k = k*6 + cCode[i];
	}

	Size=address2-address1;
	ptr=(DWORD*)address1;

	if(!bSucceed)
	Decrypt (ptr,Size,k); //执行解密函数

	//如Decrypt()函数没解出正确的代码，则会异常
	try
	{
	 // 在十六进制工具中用下面两行代码定位加密代码起始处
      _asm inc eax // 在十六进制工具中对应0x40  
      _asm dec eax // 在十六进制工具中对应0x48

/***************************************************************/
/* 需要加密的代码起始标签，即address1地址                     */
/***************************************************************/
begindecrypt: 

	if(GetOpenFileName (&ofn))
	{

		     hFile = CreateFile( szFileName,	
					 GENERIC_READ ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL ,
					 NULL);
		     if( hFile != INVALID_HANDLE_VALUE )
		     {
			     FileSize=GetFileSize(hFile,&szTemp);  // 获取文件大小
			     if(FileSize<0x10000)                  // 如文件小于64K则读取
			     {
				     if(ReadFile(hFile, pBuffer, FileSize, &szTemp, NULL)!=NULL) // 读取数据
				     {

					     SetWindowText(hWnd,pBuffer); // 将内存中的数据显示在文本编辑框中
					     CloseHandle(hFile);
					     delete pBuffer;  // 释放内存
						 bSucceed=true;   //设置标志，意味代码解密正确，以后再调用“打开”功能时，不必再调用Decrypt()函数
					     return TRUE;    // 退出函数
				     }
			     }
			     CloseHandle(hFile);
		 
		}
		MessageBoxA (NULL, TEXT ("可能是文件大于64K等其它原因!"), TEXT ("打开文件失败！"), 0) ;
	}

	delete pBuffer; 
	bSucceed=true;
/*****************************************************************/
/* 需要加密的代码结束标签 ，即address2地址                       */
/*****************************************************************/
enddecrypt: 

	 // 在十六进制工具中用下面两行代码定位加密代码结束处
	  _asm inc eax // 在十六进制工具中对应0x40  
      _asm dec eax // 在十六进制工具中对应0x48
	  return TRUE;

	}
	//如果地址address1和address2之间数据解密不成功，则执行这些垃圾代码时必定异常，异常后就会跳到如下代码处：
	catch (...)
	{
		MessageBoxA (NULL, TEXT ("请注册，以获得完整的功能 !"), TEXT ("提示"), 0) ;  
		Decrypt (ptr,Size,k);//如注册码不正确，则必须得再次调用Decrypt()将address1和address2之间数据还原
		return FALSE;
	}

}

/*-------------------------------------------------------------*/
/* Decrypt － 解密子程序                                      */
/*-------------------------------------------------------------*/
// 对地址address1和address2之间数据解密
// 解密数据 = 原始数据 XOR value
//
// 注意：如不用VirtualQuery、VirtualProtect函数改变面属性，则必须用PE工具将.text区块设为读、写模式，效果一样。

void Decrypt (DWORD* pData,DWORD Size,DWORD value)
{
    //首先要做的是改变这一块虚拟内存的内存保护状态，以便可以自由存取代码
	MEMORY_BASIC_INFORMATION mbi_thunk; 
    //查询页信息
	VirtualQuery(pData, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION)); 
	//改变页保护属性为读写。
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_READWRITE, &mbi_thunk.Protect); 

	Size=Size/0x4; //对数据共需要异或的次数
	//解密begindecrypt与enddecrypt标签处的数据
	while(Size--)
	{
		*pData=(*pData)^value;
		pData++;
	}
	
	//恢复页的原保护属性。
	DWORD dwOldProtect; 
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect); 

}


