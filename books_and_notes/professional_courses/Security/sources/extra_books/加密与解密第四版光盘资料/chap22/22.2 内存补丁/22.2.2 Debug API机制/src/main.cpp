/*-----------------------------------------------------------------------
第22章  补丁技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>

#define BREAK_POINT1     0x040138D            //需要中断的地址
#define SZFILENAME       ".\\TraceMe.exe"  //目标文件名

	
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{		
	
	STARTUPINFO				si ;
	PROCESS_INFORMATION		pi ;
	ZeroMemory(&si, sizeof(STARTUPINFO)) ;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION)) ;
	si.cb = sizeof(STARTUPINFO) ;
	
	BOOL	WhileDoFlag=TRUE;
	BYTE    ReadBuffer[MAX_PATH]={0};
	BYTE    dwINT3code[1]={0xCC};
	BYTE    dwOldbyte[1]={0};

	if( !CreateProcess(SZFILENAME, 
        NULL, 
        NULL,  
        NULL,  
        FALSE,   
        DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS, 
        NULL,          
        NULL,          
        &si,            
        &pi )        
		) 
	{
        MessageBox(NULL, "CreateProcess Failed.", "ERROR", MB_OK); 
        return FALSE; 
    } 
	
	DEBUG_EVENT		DBEvent ;
	CONTEXT			Regs ;
	DWORD			dwState,Oldpp;


	Regs.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS ;
	
	while (WhileDoFlag) {
		WaitForDebugEvent (&DBEvent, INFINITE);
		dwState = DBG_EXCEPTION_NOT_HANDLED ;
		switch (DBEvent.dwDebugEventCode)
		{
			case CREATE_PROCESS_DEBUG_EVENT:
				//如果进程开始，则将断点地址的代码改为INT3中断,同时备份原机器码
				ReadProcessMemory(pi.hProcess, (LPCVOID)(BREAK_POINT1), &dwOldbyte, sizeof(dwOldbyte), NULL) ;
				VirtualProtectEx(pi.hProcess, (LPVOID)BREAK_POINT1, 1, PAGE_EXECUTE_READWRITE, &Oldpp);
				WriteProcessMemory(pi.hProcess, (LPVOID)BREAK_POINT1,&dwINT3code, 1,NULL);	//打补丁
				dwState = DBG_CONTINUE ;
				break;			
				
			case	EXIT_PROCESS_DEBUG_EVENT :
				WhileDoFlag=FALSE;
				break ;
			
			case	EXCEPTION_DEBUG_EVENT:
				switch (DBEvent.u.Exception.ExceptionRecord.ExceptionCode)
				{
					case	EXCEPTION_BREAKPOINT:
					{
						GetThreadContext(pi.hThread, &Regs) ;
						if(Regs.Eip==BREAK_POINT1+1){
							//中断触发异常事件，恢复原机器码，并读出数据
							Regs.Eip--;
							WriteProcessMemory(pi.hProcess, (LPVOID)BREAK_POINT1,&dwOldbyte, 1,NULL);
							ReadProcessMemory(pi.hProcess, (LPCVOID)(Regs.Ebp), &ReadBuffer, sizeof(ReadBuffer), NULL) ;
							MessageBox (0, (char *)ReadBuffer, "内存注册机演示", MB_OK);						
							SetThreadContext(pi.hThread, &Regs) ;
						}
						dwState = DBG_CONTINUE ;
						break;
					}
				}
				break;
		}
		
		ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, dwState) ;
	} //.end while
	
	CloseHandle(pi.hProcess) ;
	CloseHandle(pi.hThread)  ;
	return TRUE;
}
