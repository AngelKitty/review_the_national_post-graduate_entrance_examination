// DllLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "../SecurityMod/SecurityMod.h"

#ifdef _DEBUG
#pragma comment(lib,"../SecurityMod/Debug/SecurityMod.lib")
#else
#pragma comment(lib,"../SecurityMod/Release/SecurityMod.lib")
#endif



HANDLE CreateTrustThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes
	DWORD dwStackSize,                         // initial thread stack size
	LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function
	LPVOID lpParameter,                        // argument for new thread
	DWORD dwCreationFlags,                     // creation flags
	LPDWORD lpThreadId                         // pointer to receive thread ID
	);

DWORD WINAPI ThreadTrust(LPVOID lpParameters);

int main(int argc, char* argv[])
{
	printf("Dll Loader. ThreadId = %d\n",GetCurrentThreadId());
	CreateTrustThread(NULL,0,ThreadTrust,NULL,0,NULL);
	while (TRUE)
	{
		Sleep(100000);
	}
	return 0;
}

DWORD WINAPI ThreadTrust(LPVOID lpParameters)
{
	DWORD i = 0 ;
	printf("[ThreadTrust] Tid = %d\n",GetCurrentThreadId());
	while (TRUE)
	{
		printf("[%4d] ThreadTrust working...\n",i++);
		Sleep(5000);
	}
	return 0;
}

HANDLE CreateTrustThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes
	DWORD dwStackSize,                         // initial thread stack size
	LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function
	LPVOID lpParameter,                        // argument for new thread
	DWORD dwCreationFlags,                     // creation flags
	LPDWORD lpThreadId                         // pointer to receive thread ID
)
{
	DWORD dwThreadId = 0 ;
	DWORD dwNewFlag = dwCreationFlags | CREATE_SUSPENDED ;
	HANDLE hThread = CreateThread(
		lpThreadAttributes,
		dwStackSize,
		lpStartAddress,
		lpParameter,
		dwNewFlag,
		&dwThreadId
		);

	if (hThread != NULL)
	{
		RegisterThread(dwThreadId);
		if (lpThreadId != NULL)
		{
			*lpThreadId = dwThreadId;
		}

		if (!(dwCreationFlags & CREATE_SUSPENDED))
		{
			ResumeThread(hThread);
		}
	}

	return hThread;
}
