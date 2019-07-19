/*******************************************************
/*《加密与解密（第四版）》
/* 第22章  补丁技术
/*  Code by 罗  翼 
/*  Thanks Zeal 
/*  
/*(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************//*******************************************************
/*《加密与解密（第四版）》
/* 第22章  补丁技术
/*  Code by 罗  翼 
/*  Thanks Zeal 
/*  
/*(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/

#include <windows.h>

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
	DWORD			 dwSSCnt ;
	
	dwSSCnt = 0 ;

	Regs.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS ;

	//使进程在Single Step模式下运行,每执行一条指令就会给调试进程发送EXCEPTION_SINGLE_STEP
	//需要注意的是，收到这个消息后，如果还想继续让程序Single Step下去，是需要重新设置一次SF位的
	GetThreadContext(pi.hThread,&Regs);
	Regs.EFlags|=0x100;
	SetThreadContext(pi.hThread,&Regs);

	ResumeThread(pi.hThread);

	
	while (WhileDoFlag) {
		WaitForDebugEvent (&DBEvent, INFINITE);
		switch (DBEvent.dwDebugEventCode)
		{
		case	EXCEPTION_DEBUG_EVENT:

			switch (DBEvent.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case	EXCEPTION_SINGLE_STEP :
				{
					++dwSSCnt ;
					if (dwSSCnt == 1)
					{   
						//当收到第一个EXCEPTION_SINGLE_STEP异常信号，表示中断在程序的第一条指令，即入口点
						//把Dr0设置成程序的入口地址
						
						GetThreadContext(pi.hThread,&Regs);
						
						Regs.Dr0=Regs.Eax;
						Regs.Dr7=0x101;
						
						SetThreadContext(pi.hThread,&Regs);
						
					}
					else if (dwSSCnt == 2)
					{
						//第二次中断在起先设置的入口点，在BREAK_POINT1处设置硬件断点

						GetThreadContext(pi.hThread, &Regs) ;
						Regs.Dr0 = BREAK_POINT1;
						Regs.Dr7 = 0x101 ;
						SetThreadContext(pi.hThread, &Regs) ;
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
					}
					break ;
				}				 
			}
			break ;

		case	EXIT_PROCESS_DEBUG_EVENT :
				WhileDoFlag=FALSE;
				break ;
		}
		
		ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, DBG_CONTINUE) ;
	} //.end while
	
	CloseHandle(pi.hProcess) ;
	CloseHandle(pi.hThread)  ;
	return TRUE;
}
