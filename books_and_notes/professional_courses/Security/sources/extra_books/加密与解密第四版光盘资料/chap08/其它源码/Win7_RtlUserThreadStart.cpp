VOID _RtlUserThreadStart(
	LPTHREAD_START_ROUTINE lpThreadStartAddr,
	LPVOID lpThreadParm
	)
{
	EXCEPTION_REGISTRATION_RECORD FinalSEH;
	RtlInitializeExceptionChain(&FinalSEH);
	__RtlUserThreadStart(lpThreadStartAddr,lpThreadParm);
	__debugbreak();
}

DWORD WINAPI __RtlUserThreadStart(
	LPTHREAD_START_ROUTINE lpThreadStartAddr,
	LPVOID lpThreadParm
	)
{
	DWORD dwThreadExitCode ;
	PTOP_LEVEL_EXCEPTION_FILTER FilterFunc = NULL ;
    __try 
    {
		if (Kernel32ThreadInitThunkFunction != NULL)
		{
			return Kernel32ThreadInitThunkFunction(FALSE,lpThreadStartAddr,lpThreadParm);
		}
		else
		{
			dwThreadExitCode = lpWin32StartAddr(lpThreadParm);
			RtlExitUserThread(dwThreadExitCode);
		}
    } 
    __except(FilterFunc = RtlDecodePointer(RtlpUnhandledExceptionFilter),
		FilterFunc = ( FilterFunc == NULL ) ? RtlUnhandledExceptionFilter : FilterFunc ,
		FilterFunc(GetExceptionInformation())) 
    {
		ZwTerminateProcess(NtCurrentProcess(),GetExceptionCode());
    }
}
