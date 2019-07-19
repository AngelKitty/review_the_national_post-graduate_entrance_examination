/*-----------------------------------------------------------------------
第22章  补丁技术
《加密与解密（第四版）》
Code by 罗  翼
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>

#define BREAK_ENTRYPOINT 0x401000          //程序入口点
#define BREAK_POINT1     0x040138D          //需要中断的地址
#define SZFILENAME       ".\\TraceMe_asp.exe"  //目标文件名

	
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
	DWORD			dwState, dwBpCnt, dwSSCnt, dwAddrProc ;
	
	dwBpCnt = dwSSCnt = 0 ;

	Regs.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS ;
	
	while (WhileDoFlag) {
		WaitForDebugEvent (&DBEvent, INFINITE);
		dwState = DBG_EXCEPTION_NOT_HANDLED ;
		switch (DBEvent.dwDebugEventCode)
		{
		case	EXCEPTION_DEBUG_EVENT:
			switch (DBEvent.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case	EXCEPTION_BREAKPOINT:
				{
					++dwBpCnt ;
					if (dwBpCnt == 1)
					{
						//通过dwBPCnt计数器判断出接收到的是第一个调试信息。
						//得到ntdll.ntContinue的地址，并且设置Dr0 的值等于该地址
						GetThreadContext(pi.hThread, &Regs) ;
						Regs.Dr0 = (DWORD)(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtContinue") );
						Regs.Dr7 = 0x101 ;
						SetThreadContext(pi.hThread, &Regs) ;
						dwState = DBG_CONTINUE ;
					}
					break ;
				}
			case	EXCEPTION_SINGLE_STEP :
				{
					++dwSSCnt ;
					if (dwSSCnt == 1)
					{
						//当收到第一个EXCEPTION_SINGLE_STEP异常信号，表示中断在ntdll.ntContinue函数
						//把Dr0设置成程序的入口地址
						
						GetThreadContext(pi.hThread, &Regs) ;
						Regs.Dr0 = Regs.Dr7 = 0 ;
						SetThreadContext(pi.hThread, &Regs) ;
						
						ReadProcessMemory(pi.hProcess, (LPCVOID)(Regs.Esp+4), &dwAddrProc, sizeof(DWORD), NULL) ;
						ReadProcessMemory(pi.hProcess, (LPCVOID)dwAddrProc, &Regs, sizeof(CONTEXT), NULL) ;
						
						Regs.Dr0 = BREAK_ENTRYPOINT ;
						Regs.Dr7 = 0x101 ;
						
						WriteProcessMemory(pi.hProcess, (LPVOID)dwAddrProc, &Regs, sizeof(CONTEXT), NULL) ;
						
						dwState = DBG_CONTINUE ;
					}
					else if (dwSSCnt == 2)
					{   //第二次中断在起先设置的入口点，在BREAK_POINT1处设置硬件断点
						
						GetThreadContext(pi.hThread, &Regs) ;
						Regs.Dr0 = BREAK_POINT1;
						Regs.Dr7 = 0x101 ;
						SetThreadContext(pi.hThread, &Regs) ;
						dwState = DBG_CONTINUE ;
					}
					else if (dwSSCnt == 3)
					{
						//第三次中断，己到指定的地址，读取EBP寄存器指向的内存数据
						GetThreadContext(pi.hThread, &Regs) ;
						Regs.Dr0 = Regs.Dr7 = 0 ;
						//WriteProcessMemory(pi.hProcess, (LPVOID)Regs.Eip,&WriteData, 2,NULL);	//打补丁
						ReadProcessMemory(pi.hProcess, (LPCVOID)(Regs.Ebp), &ReadBuffer, sizeof(ReadBuffer), NULL) ;
						MessageBox (0, (char *)ReadBuffer, "test", MB_OK);						
						SetThreadContext(pi.hThread, &Regs) ;
						dwState = DBG_CONTINUE ;
					}
					break ;
				}				 
			}
			break ;

		case	EXIT_PROCESS_DEBUG_EVENT :
				WhileDoFlag=FALSE;
				break ;
		}
		
		ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, dwState) ;
	} //.end while
	
	CloseHandle(pi.hProcess) ;
	CloseHandle(pi.hThread)  ;
	return TRUE;
}
