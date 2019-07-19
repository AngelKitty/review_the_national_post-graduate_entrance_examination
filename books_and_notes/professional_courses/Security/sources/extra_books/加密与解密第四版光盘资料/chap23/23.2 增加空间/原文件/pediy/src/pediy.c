/*-----------------------------------------------------------------------
第23章  代码的二次开发
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>
#include "resource.h"

#define ID_EDIT     1
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("pediy"); 

/*-------------------------------------------------------------*/
/* 优化                                                        */
/*-------------------------------------------------------------*/

//如果使用 VS.NET，可以这样设置：在“解决方案管理器”里选定项目，右击，选择“属性”->“链接器”->“高级”->“入口点” 
#pragma comment(linker, "/ENTRY:EntryPoint") 



//////////////////////////////////////////////////////////////////////////////////////////////////// 
//入口函数 
//使用我们自己的入口函数，而不用连接器默认提供的一大堆程序初始化操作的代码 
//为了在一个普通的 Win32SDK 程序里能使用这种方法，下面的函数将调用 WinMain() 函数，并给出相应的参数 
void EntryPoint() 
{ 
      ExitProcess(WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOWNORMAL)); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////// 


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HWND     hwnd ;
     MSG      msg ;
     WNDCLASS wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON));
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))                                // 按Unicode方案编译时,不能运行在Windows 9x系统
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("逆向分析技术"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          400, 300,
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
	 static HWND hwndEdit ;
     
     switch (message)
     {
   case WM_CREATE :
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

	 case WM_COMMAND:
 
          
          switch (LOWORD (wParam))
          {
         
               
        //  case IDM_APP_EXIT:
             //  SendMessage (hwnd, WM_CLOSE, 0, 0) ;
             //  return 0 ;
               
         
          case IDM_APP_ABOUT:
               MessageBox (hwnd, TEXT ("逆向分析技术\n")
                                 TEXT ("(c) 段钢, 2002"),
                           szAppName, MB_ICONINFORMATION | MB_OK) ;
               return 0 ;
          }
          break ;

               
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
