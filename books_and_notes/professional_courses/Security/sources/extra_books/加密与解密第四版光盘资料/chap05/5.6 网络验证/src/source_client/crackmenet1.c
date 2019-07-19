/*******************************************************
《加密与解密》第三版配套实例
5.6 网络验证
code by riijj
(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/


#include <windows.h>
#include <time.h>
#include "resource.h"

const char g_szClassName[] = "myWindowClass";

BYTE bufResultEncrypt[0x5a]={0};


HWND hwnd;
HWND hwnddlg;

int performCheck();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
			WSACleanup();
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


BOOL CALLBACK DialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
  ){

	if( uMsg == WM_INITDIALOG)
	{
		SetFocus( GetDlgItem( hwndDlg, IDC_EDIT1));
	}
	else if( uMsg == WM_COMMAND)
	{
		int id = LOWORD(wParam);
		HWND ctl = (HWND)lParam;

		if( id == IDC_BUTTON1)
		{
			EnableWindow( ctl, FALSE);
			if(performCheck())
			{
				if( bufResultEncrypt[0x48] != 0x77 )
					ExitProcess(0) ;
			}
			EnableWindow( ctl, TRUE);
		}
	}

	return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
	int sx, sy;
    MSG Msg;
	WSADATA wsa;

	WSAStartup( MAKEWORD( 2, 2 ), &wsa);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.lpszMenuName  = 0;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassEx(&wc);

	sx = GetSystemMetrics(SM_CXSCREEN);
	sy = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(
        0,
        g_szClassName,
        "Pediy - Crackme (net) 20061204",
        WS_OVERLAPPED | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU,
        sx/2-300/2, sy/2-200/2, 300, 200,
        0, 0, hInstance, 0);

	hwnddlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), hwnd, DialogProc);

	SetWindowPos( hwnddlg, hwnd, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
   
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, 0, 0, 0))
    {
		if( ! IsDialogMessage( hwnddlg, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
    }
    return Msg.wParam;
}


int performCheck()
{
	int err;
	char bufname[51]={0};
	char bufkey[51]={0};
	BYTE bufRecv[500]={0};
	char bufEncrypt[200]={0};
	int nameLength, keyLength, totalLength;
	BYTE ran_K;
	int numRecv;
	int i;
	struct   sockaddr_in name;

	SOCKET s = socket(AF_INET,SOCK_STREAM,0);
	
	name.sin_family = AF_INET;
	name.sin_port = htons(8877);
	name.sin_addr.s_addr = inet_addr("127.0.0.1");

	err = connect( s, (struct sockaddr*)&name, sizeof(name));

	if( err == SOCKET_ERROR)
	{
		MessageBox( hwnd, "Error: Connection failed.", "Crackme net-2", 0);
		return 0;
	}

	GetDlgItemText( hwnddlg, IDC_EDIT1, bufname, 50);
	GetDlgItemText( hwnddlg, IDC_EDIT2, bufkey, 50);

	nameLength = strlen( bufname);
	keyLength = strlen( bufkey);

	srand(time(0));
	ran_K = rand() % 256;

	bufEncrypt[0] = (BYTE)nameLength;
	bufEncrypt[1] = (BYTE)keyLength;	
	bufEncrypt[2] = ran_K;
	
	memcpy( bufEncrypt+3, bufname, nameLength);
	memcpy( bufEncrypt+3+nameLength, bufkey, keyLength);

	totalLength = 3 + nameLength + keyLength;

	for( i=0; i< totalLength; i++)
	{
		bufEncrypt[i] = bufEncrypt[i] ^ 0xa6;  //  XOR encrypt
	}
	
	send( s, bufEncrypt, totalLength, 0);

	Sleep(500);

	numRecv = recv( s, bufRecv, 500, 0);

	for( i=0; i<numRecv; i++)
	{
		bufResultEncrypt[i] = bufRecv[i] ^ 0x6e;					
	}

	if( bufResultEncrypt[0x4] == 0xD5 &&
		bufResultEncrypt[0xe] == ran_K &&
		bufResultEncrypt[0xa]+3 == 0x16 &&
		bufResultEncrypt[0x23] == 0xBD &&
		bufResultEncrypt[0x3c] == 0x43 &&
		bufResultEncrypt[0x42] == 0xc6  )
	{
		MessageBox( hwnd, "Registration successful !", "Crackne net-2", 0);
		return 1;
	}
	return 0;

}


// End of code

