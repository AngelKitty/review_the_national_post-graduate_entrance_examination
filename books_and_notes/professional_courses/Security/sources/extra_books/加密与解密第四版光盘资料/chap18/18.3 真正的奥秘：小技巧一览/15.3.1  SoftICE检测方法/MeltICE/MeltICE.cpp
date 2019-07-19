/*--------------------------------------------------------------
   MeltICE.cpp -- MeltICE子类型检测SoftICE
                           (c) www.pediy.com  段钢, 2003
  --------------------------------------------------------------*/

#include <windows.h>


BOOL IsSoftIceNTLoaded();
BOOL IsSoftIce9xLoaded();
BOOL SoftICELoadedbySIWDEBUG();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if( IsSoftIce9xLoaded() || SoftICELoadedbySIWDEBUG())
		MessageBox(NULL,TEXT ("SoftICE for Windows 9x is active!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else if( IsSoftIceNTLoaded() )
		MessageBox(NULL,TEXT ("SoftICE 4.05 for Win2K  is Running!\n"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,TEXT ("Can't find SoftICE with this method!"),TEXT ("Error"),MB_ICONEXCLAMATION);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//
// 检测Windows 9x上的SoftICE
//////////////////////////////////////////////////////////////////////
BOOL IsSoftIce9xLoaded()
{
	HANDLE hFile;  
 
	hFile = CreateFile( "\\\\.\\SICE",	
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(hFile);
		return TRUE;
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////
//
// 利用\.\\SIWDEBUG检测Windows 9x上的SoftICE
//////////////////////////////////////////////////////////////////////

BOOL SoftICELoadedbySIWDEBUG()
{
	
	HANDLE hFile;  
	hFile = CreateFile( "\\\\.\\SIWDEBUG",	
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if( GetLastError()==0x32 )  // 如果SoftICE激活GetLastError()返回32h; 否则返回02
		return TRUE;
	else 
		return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// 检测Windows 2000系统上的SofICE 4.05
//////////////////////////////////////////////////////////////////////
BOOL IsSoftIceNTLoaded()
{
	HANDLE hFile;  
 
	hFile = CreateFile( "\\\\.\\NTICE",
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(hFile);
		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////