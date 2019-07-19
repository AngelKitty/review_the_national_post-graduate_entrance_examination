/*--------------------------------------------------------------
   NticeService.cpp -- 利用NticeService检测SoftICE
                           (c) www.pediy.com  段钢, 2003
  --------------------------------------------------------------*/

#include <windows.h>
#include <wchar.h>

BOOL SoftICELoaded();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if( SoftICELoaded() )
		MessageBox(NULL,TEXT ("SoftICE is active!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,TEXT ("Can't find SoftICE with this method!"),TEXT ("Error"),MB_ICONEXCLAMATION);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//


BOOL SoftICELoaded()
{

	SERVICE_STATUS ssStatus; 
	// 打开服务控制管理器
	SC_HANDLE shServiceManager = OpenSCManager( NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
	if(shServiceManager==NULL) 
		return FALSE;
	// 打开NTice服务
	SC_HANDLE shMyService =OpenService(shServiceManager,"NTice",SERVICE_ALL_ACCESS);

	if ( (QueryServiceStatus( shMyService, &ssStatus)) == 0 ) //查询并返回当前Service的状态
	{
		CloseServiceHandle(shMyService);
		return FALSE;
	}

	if (ssStatus.dwCurrentState == SERVICE_RUNNING ) //NTice服务正在运行？
	{
		CloseServiceHandle(shMyService);		
			return TRUE;
	}

	return FALSE;

}

