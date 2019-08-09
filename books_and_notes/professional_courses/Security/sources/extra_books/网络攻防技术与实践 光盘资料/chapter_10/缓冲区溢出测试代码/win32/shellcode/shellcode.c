#include <windows.h>
#include <winbase.h>
typedef void (*MYPROC)(LPTSTR);
typedef void (*MYPROC2)(int);
int main()
{
        HINSTANCE LibHandle;
        MYPROC ProcAdd;
        MYPROC2 ProcAdd2;
        char dllbuf[11]  = "msvcrt.dll";
        char sysbuf[7] = "system";
        char cmdbuf[16] = "command.com";
        char sysbuf2[5] = "exit";
        LibHandle = LoadLibrary(dllbuf);
        ProcAdd = (MYPROC)GetProcAddress(
			LibHandle, sysbuf);
        (ProcAdd) (cmdbuf);

        ProcAdd2 = (MYPROC2) GetProcAddress(
			LibHandle, sysbuf2);
		(ProcAdd2)(0);
}

