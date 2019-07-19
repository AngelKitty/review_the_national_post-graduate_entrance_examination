/*-----------------------------------------------------------------------
第22章  补丁技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

/*
004011F5   74 37         je      short 0040122E
这句改成：
004011F5      90         nop
004011F6      90         nop
*/
#define PATCH_ADDRESS   0x4011F5				//目标进程要补丁的地址
#define PATCH_SIZE      2					    //目标进程要补丁的字节数
#define SZFILENAME       ".\\TraceMe_asp.exe"   //目标文件名

#define BREAK_ENTRYPOINT 0x401000
#define BREAK_POINT1     0x40138F
#include <windows.h>

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{		
	STARTUPINFOA     	si;
	PROCESS_INFORMATION pi;
	
	BYTE      ReadBuffer[MAX_PATH]={0};
	BOOL      bContinueRun=TRUE;
	DWORD	  Oldpp;
	
	BYTE      TarGetData[]={0x74,0x37}; 			//补丁前的代码
	BYTE      WriteData[]={0x90,0x90}; 				//补丁后的代码
	
	
	
	ZeroMemory(&si, sizeof(STARTUPINFO)) ;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION)) ;

    if( !CreateProcess(SZFILENAME, 
        NULL, 
        NULL,  
        NULL,  
        FALSE,   
        CREATE_SUSPENDED, 
        NULL,          
        NULL,          
        &si,            
        &pi )        
		) 
	{
        MessageBox(NULL, "CreateProcess Failed.", "ERROR", MB_OK); 
        return FALSE; 
    } 
	
	
	while (bContinueRun) {
		
		ResumeThread(pi.hThread); 
		Sleep(10);
		SuspendThread(pi.hThread);
		ReadProcessMemory(pi.hProcess, (LPVOID)PATCH_ADDRESS, &ReadBuffer, PATCH_SIZE, NULL);
		//判断是不是完全解码出来了
		if( !memcmp(TarGetData,ReadBuffer, PATCH_SIZE) ){	
			VirtualProtectEx(pi.hProcess, (LPVOID)PATCH_ADDRESS, PATCH_SIZE, PAGE_EXECUTE_READWRITE, &Oldpp);
			WriteProcessMemory(pi.hProcess, (LPVOID)PATCH_ADDRESS,&WriteData, PATCH_SIZE,NULL);
			ResumeThread(pi.hThread); 
			bContinueRun=FALSE;
		}		
	}
    CloseHandle(pi.hProcess); 
    CloseHandle(pi.hThread); 

    return 0; 	
}
