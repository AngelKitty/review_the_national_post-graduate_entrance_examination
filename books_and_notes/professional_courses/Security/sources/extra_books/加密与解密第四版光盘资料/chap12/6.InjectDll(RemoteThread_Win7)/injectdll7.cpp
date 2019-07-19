/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "advapi32.lib")

DWORD GetDebugPrivilege()
{
	HANDLE hToken;
	DWORD Ret=1;
	TOKEN_PRIVILEGES TP;

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		printf("[-] Error in GetDebugPrivilege OpenProcessToken: %u\n", GetLastError());
		Ret=0;
		goto bye;	
	} 
	
	if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TP.Privileges[0].Luid))
	{
		printf("[-] Error in GetDebugPrivilege LookupPrivilegeValue: %u\n", GetLastError());
		Ret=0;
		goto bye;	
		
	}
	
	TP.PrivilegeCount=1;
	TP.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	
	if(!AdjustTokenPrivileges(hToken,
							FALSE,
							&TP,
							0,
							NULL,
							NULL))
	{
		printf("[-] Error in GetDebugPrivilege with  AdjustTokenPrivileges: %u\n", GetLastError());
		Ret=0;
		goto bye;
	}
	
bye:
	CloseHandle(hToken);
	
	return Ret;	
}


/*
kernelbase.dll
7597BD24    6A 0C           PUSH 0C
7597BD26    68 01000100     PUSH 10001
7597BD2B    53              PUSH EBX
7597BD2C    8D85 F0FDFFFF   LEA EAX, DWORD PTR SS:[EBP-210]
7597BD32    50              PUSH EAX
7597BD33    FF15 00129775   CALL NEAR DWORD PTR DS:[<&ntdll.CsrClientCallServer>]      ; ntdll.CsrClientCallServer
7597BD39    8B85 10FEFFFF   MOV EAX, DWORD PTR SS:[EBP-1F0]
7597BD3F    8985 E8FDFFFF   MOV DWORD PTR SS:[EBP-218], EAX
7597BD45    399D E8FDFFFF   CMP DWORD PTR SS:[EBP-218], EBX
7597BD4B    0F8C 13D80100   JL KERNELBA.75999564
*/

DWORD __stdcall MyCsrClientCallServer(PVOID Arg1, PVOID Arg2, DWORD Arg3, DWORD Arg4)
{
	*(PDWORD)((PBYTE)Arg1+0x20)=0;
	return 0;	
}


DWORD ScanIatForImportAddress(HANDLE hModule, PCHAR Import)
{
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS NtHeader;
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor;
	PIMAGE_THUNK_DATA OriginalThunk;
	PDWORD FirstThunk;
	PIMAGE_IMPORT_BY_NAME ImportByName;
	PCHAR Name;
	DWORD i;
	
	if(hModule==NULL || Import==NULL)
		return 0;
	
	DosHeader=(PIMAGE_DOS_HEADER)hModule;
	NtHeader=(PIMAGE_NT_HEADERS)((PBYTE)hModule+DosHeader->e_lfanew);
	ImportDescriptor=(PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)hModule+NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	while(*(PDWORD)ImportDescriptor!=0)
	{
		//printf("[*] Module : %s\n", ((PBYTE)hModule+ImportDescriptor->Name));
		OriginalThunk=(PIMAGE_THUNK_DATA)((PBYTE)hModule+ImportDescriptor->OriginalFirstThunk);
		FirstThunk=(PDWORD)((PBYTE)hModule+ImportDescriptor->FirstThunk);
		i=0;
		while(*(PDWORD)OriginalThunk!=0)
		{
			ImportByName=(PIMAGE_IMPORT_BY_NAME)((PBYTE)hModule+OriginalThunk->u1.AddressOfData);
			Name=(PCHAR)((PBYTE)ImportByName+sizeof(WORD));
			//printf("[*] ImportName(%u): %s\n", ImportByName->Hint, Name);
			
			if(_stricmp(Name, Import)==0)
				return (DWORD)&FirstThunk[i++];
				
			OriginalThunk++;
			i++;
		}
		ImportDescriptor++;
	}
	
	return 0;	
}


HANDLE WINAPI MyCreateRemoteThread(
  __in   HANDLE hProcess,
  __in   LPSECURITY_ATTRIBUTES lpThreadAttributes,
  __in   SIZE_T dwStackSize,
  __in   LPTHREAD_START_ROUTINE lpStartAddress,
  __in   LPVOID lpParameter,
  __in   DWORD dwCreationFlags,
  __out  LPDWORD lpThreadId
)
{
	HANDLE hThread;
	DWORD ImportAddress, OriginalCsrClientCallServer, OldProtect;

	ImportAddress=ScanIatForImportAddress(GetModuleHandle("kernelbase.dll"), "CsrClientCallServer");
	if(ImportAddress==0)
	{
		printf("[-] Error in MyCreateRemoteThread with ScanIatForThunk : Cannot find thunk address\n");
		return NULL;
	}
	printf("[*] CsrClientCallServer import address at : 0x%x\n", ImportAddress);
		
	VirtualProtect((PVOID)ImportAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &OldProtect);
	OriginalCsrClientCallServer=*(PDWORD)ImportAddress;
	*(PDWORD)ImportAddress=(DWORD)MyCsrClientCallServer;
	
	hThread=CreateRemoteThread(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	if(hThread==NULL)
	{
		printf("[-] Error in MyCreateRemoteThread with CreateRemoteThread : %u\n", GetLastError());
		return NULL;
	}
	*(PDWORD)ImportAddress=OriginalCsrClientCallServer;
	VirtualProtect((PVOID)ImportAddress, sizeof(DWORD), OldProtect, &OldProtect);

	return hThread;
}


BOOLEAN InjectDll(DWORD Pid, LPTSTR DllName)
{
	DWORD ThreadID;
	HANDLE hThread;
	HANDLE hProcess;
	DWORD InjectSize;
	LPVOID Arg;
	LPTHREAD_START_ROUTINE Injector;
	CHAR FullDllPath[MAX_PATH];
	
	RtlZeroMemory(FullDllPath, sizeof(FullDllPath));
	GetCurrentDirectory(sizeof(FullDllPath)-sizeof(CHAR), FullDllPath);
	StringCbCat(FullDllPath, sizeof(FullDllPath)-strlen(FullDllPath)-sizeof(CHAR), "\\");
	StringCbCat(FullDllPath, sizeof(FullDllPath)-strlen(FullDllPath)-sizeof(CHAR), DllName);
	
	printf("[*] FullDllPath : %s\n", FullDllPath);

	if(!GetDebugPrivilege())
	{
		printf("[-] Error in InjectDll with GetDebugPrivilege : Cannot grant SeDebugPrivilege\n");
		return FALSE;
	}

	hProcess=OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ|PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION, FALSE, Pid);
	if(hProcess==NULL)
	{
		printf("[-] Error in InjectDll with  OpenProcess : %u\n", GetLastError());
		return FALSE;
	}
	
	InjectSize=strlen(FullDllPath)+1;

	Arg=VirtualAllocEx(hProcess, NULL, InjectSize, MEM_COMMIT, PAGE_READWRITE);
	if(Arg==NULL)
	{
		printf("[-] Error in InjectDll with VirtualAllocEx : %u\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}

	if(WriteProcessMemory(hProcess, Arg, FullDllPath, InjectSize, 0)==FALSE)
	{
		printf("[-] Error in InjectDll with  WriteProcessMemory : %u\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}

	Injector=(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	hThread=MyCreateRemoteThread(hProcess, NULL, 0, Injector , Arg, 0, &ThreadID);
	if(hThread==NULL)
	{
		printf("[-] Error in InjectDll with MyCreateRemoteThread : %u\n", GetLastError());
		CloseHandle(hProcess);
		return FALSE;
	}

	if(WaitForSingleObject(hThread, INFINITE)==WAIT_FAILED)
	{
		printf("[-] Error in InjectDll with WaitForSingleObject : %u\n", GetLastError());

		CloseHandle(hProcess);
		CloseHandle(hThread);
		return FALSE;
	}

	if(VirtualFreeEx(hProcess, Arg, 0, MEM_DECOMMIT)==FALSE)
	{
		printf("[-] Error in InjectDll with VirtualFreeEx : %u\n", GetLastError());

		CloseHandle(hProcess);
		CloseHandle(hThread);
		return FALSE;
	}

	CloseHandle(hProcess);
	CloseHandle(hThread);

	return TRUE;
}


int __cdecl main(int argc, char * argv[])
{
	DWORD Pid;
	
	printf("Custom CreateRemoteThread() which bypass subsystem control\nBy Ivanlef0u\nBE M4D !\n\n");
	
	if(argc<3)
	{
		printf("Usage is : %s <pid> <dll name>\n");
		return 0;
	}
	
	Pid=strtoul(argv[1], NULL, 10);
	if(Pid==0 || Pid==4)
		return 0;
	
	InjectDll(Pid, argv[2]);	
	
	printf("[+] Dll successfully injected !\n");
	
	return 0;
}