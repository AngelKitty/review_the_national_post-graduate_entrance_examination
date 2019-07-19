/*******************************************************
《加密与解密》第四版配套实例
*程序:ReverseMe.cpp                                   *
*用途:静态分析实例                                     *
*  (c)  看雪学院 www.kanxue.com 2000-2018                  *
********************************************************/

#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("chap231") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
          return 0 ;
        
     hwnd = CreateWindow (szAppName,                  
                          TEXT ("静态分析技术实例") ,  
                          WS_OVERLAPPEDWINDOW,         
                          CW_USEDEFAULT,               
                          CW_USEDEFAULT,               
                          CW_USEDEFAULT,               
                          CW_USEDEFAULT,               
                          NULL,                       
                          NULL,                       
                          hInstance,                  
                          NULL) ;                      
     
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
	 int input;    
     switch (message)
     {         
	case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;

    case WM_CLOSE:
		input=MessageBox(hwnd,TEXT ("你真的决定退出吗?"),TEXT ("退出"),
						 MB_YESNO|MB_ICONQUESTION);
		switch(input)
			{
			case IDYES:
				break;
			case IDNO:
				return 0;
			}
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}