/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// CreateProcContextInject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <Psapi.h>
#include "Image.h"

#pragma comment(linker,"/Base:0x20000000")

//#define FILED_OFFSET(EndAddr,StartAddr) ((ULONG)((ULONG_PTR)EndAddr - (ULONG_PTR)StartAddr))

#pragma comment(lib,"psapi.lib")

typedef LONG NTSTATUS;

#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef NTSTATUS
(WINAPI *PFN_LdrLoadDll)(
	IN PWCHAR PathToFile,
	IN PULONG DllCharacteristics,
	IN PUNICODE_STRING ModuleFileName,
	OUT PHANDLE ModuleHandle
);

//x86
typedef struct _INJECT_DATA 
{
	BYTE ShellCode[0xC0];
	/*Off=0xC0*/HANDLE ModuleHandle; //Dll句柄
	/*Off=0xC4*/PUNICODE_STRING pDllPath;//PUNICODE_STRING DllPath
	/*Off=0xC8*/ULONG DllCharacteristics;
	/*Off=0xCC*/ULONG_PTR AddrOfLdrLoadDll;//LdrLoadDll地址
	/*Off=0xD0*/ULONG_PTR ProtectBase; //用于ZwProtectVirtualMemory
	/*Off=0xD4*/ULONG OldProtect; //用于ZwProtectVirtualMemory
	/*Off=0xD8*/SIZE_T ProtectSize;
	/*Off=0xDC*/ULONG_PTR ExeEntry;//Exe入口点的地址
	/*Off=0xE0*/ULONG_PTR AddrOfZwProtectVirtualMemory;
	/*Off=0xE4*/BYTE  SavedEntryCode[16];//保存exe入口点的前8字节
	/*Off=0xE8*/UNICODE_STRING usDllPath;//Dll路径
	/*Off=0xEC*/WCHAR wDllPath[256];//Dll路径，也就是usDllPath中的Buffer
}INJECT_DATA;

typedef struct _INJECT_DATA64 
{
	BYTE ShellCode[0xC0];
	/*Off=0x0C0*/HANDLE ModuleHandle; //Dll句柄
	/*Off=0x0C8*/PUNICODE_STRING pDllPath;//PUNICODE_STRING DllPath
	/*Off=0x0D0*/ULONG DllCharacteristics;
	/*Off=0x0D8*/ULONG_PTR AddrOfLdrLoadDll;//LdrLoadDll地址
	/*Off=0x0E0*/ULONG_PTR ProtectBase; //用于VirtualMemory
	/*Off=0x0E8*/ULONG OldProtect; //用于VirtualMemory
	/*Off=0x0F0*/SIZE_T ProtectSize;
	/*Off=0x0F8*/ULONG_PTR ExeEntry;//Exe入口点的地址
	/*Off=0x100*/ULONG_PTR AddrOfZwProtectVirtualMemory;
	/*Off=0x108*/BYTE  SavedEntryCode[16];//保存exe入口点的前16字节
	/*Off=0x118*/UNICODE_STRING usDllPath;//Dll路径
	/*Off=0x128*/WCHAR wDllPath[256];//Dll路径，也就是usDllPath中的Buffer
}INJECT_DATA64;

#ifdef _WIN64
EXTERN_C VOID ShellCodeFun64(VOID);
#else
VOID ShellCodeFun(VOID);
#endif

ULONG_PTR FindImageBase(HANDLE hProc,LPSTR lpModuleName);
BOOL IsWow64();

typedef
NTSTATUS
(NTAPI
*PFN_ZwProtectVirtualMemory)(
	IN HANDLE ProcessHandle,
	IN PVOID *BaseAddress,
	IN PSIZE_T RegionSize,
	IN ULONG NewProtect,
	OUT PULONG OldProtect
	);

BOOL WINAPI CreateProcessWithDll(
	LPCTSTR lpApplicationName,
	LPTSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCTSTR lpCurrentDirectory,
	LPSTARTUPINFO lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	LPCSTR lpDllName
	);

VOID TestShellCode();
VOID PrepareShellCode(BYTE *pShellCode);
VOID WINAPI LoadDllAndRestoreExeEntry(INJECT_DATA *pData);

#ifdef _WIN64
char g_szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll64.dll" ;
char g_szProcPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\HostProc64.exe";
#else
char g_szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll.dll" ;
char g_szProcPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\HostProc.exe";
#endif


int main(int argc, char* argv[])
{
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

	//TestShellCode();
	//return 0 ;


    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
	
	CreateProcessWithDll(
		NULL,
		g_szProcPath,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi,
		g_szDllPath
		);
	return 0;
}


BOOL WINAPI CreateProcessWithDll(
	LPCTSTR lpApplicationName,
	LPTSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCTSTR lpCurrentDirectory,
	LPSTARTUPINFO lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	LPCSTR lpDllPath
	)
{
	CImage Image;
	LONG Status = 0 ;
	BOOL bResult = FALSE ;
	WORD i = 0 ;
	SIZE_T dwIoCnt = 0 ;
	char szErrMsg[1024]={0};
	PROCESS_INFORMATION pi={0};
	ULONG_PTR ProcImageBase = 0 ;
	char szDllExportFunName[]= "Msg";
	HANDLE hFile  = INVALID_HANDLE_VALUE;
	PBYTE ImageBase = 0;
	BYTE IIDThunkBuffer[0x200]={0};
	PBYTE pExeEntry = NULL ;
	SIZE_T SavedEntryCodeSize = 0;
	LPTSTR pExeName = strrchr(lpCommandLine,'\\');
	pExeName = (pExeName == NULL) ? lpCommandLine : (pExeName + 1) ;
	
	printf("[*] Path = %s\n",lpCommandLine);
	
	DWORD dwNewCreationFlags = dwCreationFlags + CREATE_SUSPENDED ;
	
	
	bResult = CreateProcess(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwNewCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		&pi
		);
	
	
	
	//For Debug
	//pi.hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,0x1814);

	//进程创建成功，获取ImageBase并读取PE头
	//由于此时进程内Ldr链表还没建立，所以无法使用EnumProcessModule
	ImageBase = (PBYTE)FindImageBase(pi.hProcess,pExeName);
	if (ImageBase == NULL)
	{
		printf("[-] 无法获取进程的ImageBase!\n",lpCommandLine);
		return FALSE;
	}
	
	//读取目标进程的PE头
	bResult = Image.AttachToProcess(pi.hProcess,ImageBase);
	if (!bResult)
	{
		printf("[-] 读取目标进程exe映像的PE头失败!\n");
		return FALSE;
	}
	printf("[*] PE头读取成功!\n");
	
	pExeEntry = ImageBase + Image.m_dwEntryPoint ;
	printf("[*] Exe 入口点 : 0x%p\n",pExeEntry);
	
	INJECT_DATA Data;
	ZeroMemory(&Data,sizeof(INJECT_DATA));

	PBYTE pMem = (PBYTE)VirtualAllocEx(pi.hProcess,NULL,0x1000,MEM_RESERVE | MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if (pMem == NULL)
	{
		printf("[-] Alloc Memory Failed!\n");
		return FALSE;
	}
	printf("[*] Allocated Mem = 0x%p\n",pMem);
	

	SavedEntryCodeSize = sizeof(ULONG_PTR)*2 ;
	//保存exe入口点的8个字节，实际使用push/retn进行跳转，占用6字节
	bResult = ReadProcessMemory(pi.hProcess,pExeEntry,Data.SavedEntryCode,SavedEntryCodeSize,&dwIoCnt);
	if (!bResult)
	{
		printf("[-] 读取Exe的入口点处原始指令失败!\n");
		return FALSE;
	}

	//修改Exe入口点的属性,用于写入跳转指令,在ShellCode中会进行恢复
	DWORD dwOldProtect = 0 ;
	if (!VirtualProtectEx(pi.hProcess,pExeEntry,SavedEntryCodeSize,PAGE_EXECUTE_READWRITE,&dwOldProtect))
	{
		printf("[-] VirtualProtectEx Failed!\n");
		return FALSE;
	}

	
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");
	
	char szNtdllpath[MAX_PATH] = "System32\\ntdll.dll";
	if (IsWow64())
	{
		//如果是wow64进程，那么进程中会有两个ntdll，必须找32位的那个
		lstrcpy(szNtdllpath,"SysWow64\\ntdll.dll");
	}
	ULONG_PTR uNtdllBaseInTargetProc = (ULONG_PTR)FindImageBase(pi.hProcess,szNtdllpath);
	printf("[*] Ntdll Base In TargetProc = 0x%p\n",uNtdllBaseInTargetProc);
	ULONG_PTR uAddrofZwProtectMemmoryInCurProc = (ULONG_PTR)GetProcAddress(hNtdll,"ZwProtectVirtualMemory");
	ULONG_PTR uAddrofLdrLoadDllInCurProc = (ULONG_PTR)GetProcAddress(hNtdll,"LdrLoadDll");
	ULONG_PTR uAddrofLdrLoadDllInTargetProc = uAddrofLdrLoadDllInCurProc - (ULONG_PTR)hNtdll + uNtdllBaseInTargetProc;
	ULONG_PTR uAddrofZwProtectMemmoryInTargetProc = uAddrofZwProtectMemmoryInCurProc - (ULONG_PTR)hNtdll + uNtdllBaseInTargetProc;
	
	//保存ShellCode
	PrepareShellCode(Data.ShellCode);
	
	//将路径转换为Unicode
	MultiByteToWideChar(CP_ACP,0,lpDllPath,-1,Data.wDllPath,MAX_PATH * sizeof(WCHAR));
	//初始化UNICODE_STRING
	Data.usDllPath.Buffer = (PWCHAR)(FIELD_OFFSET(INJECT_DATA,wDllPath) + (ULONG_PTR)pMem);
	Data.usDllPath.MaximumLength = (USHORT)(MAX_PATH * sizeof(WCHAR));
	Data.usDllPath.Length = (USHORT)(wcslen(Data.wDllPath) * sizeof(WCHAR));
	//设置
	Data.pDllPath = (PUNICODE_STRING)(FIELD_OFFSET(INJECT_DATA,usDllPath) + (ULONG_PTR)pMem);
	Data.AddrOfLdrLoadDll = uAddrofLdrLoadDllInTargetProc;
	Data.AddrOfZwProtectVirtualMemory = uAddrofZwProtectMemmoryInTargetProc;
	Data.ExeEntry = (ULONG_PTR)pExeEntry;
	//ProtectVirtualMemory
	Data.ProtectBase = Data.ExeEntry ;
	Data.OldProtect = dwOldProtect;
	Data.ProtectSize = SavedEntryCodeSize ;
	
	//把ShellCode写入到目标进程
	bResult = WriteProcessMemory(pi.hProcess,pMem,&Data,sizeof(INJECT_DATA),&dwIoCnt);
	if (!bResult)
	{
		printf("[-] 向目标进程写入ShellCode失败!\n");
		return FALSE;
	}
	printf("[*] 写入ShellCode成功!\n");
	
	//向Exe中写入指令转移到ShellCode
#ifdef _WIN64
	SIZE_T JmpCodeSize = 14 ;
	BYTE newEntry[16] = "\xFF\x25\x00\x00\x00\x00"; //jmp
	*(ULONG_PTR*)(newEntry + 6 ) = (ULONG_PTR)pMem; //ShellCode地址，就在申请的内存开头
#else
	SIZE_T JmpCodeSize = 6 ;
	BYTE newEntry[8] = "\x68\x11\x11\x11\x11\xC3"; //push /retn
	*(ULONG_PTR*)(newEntry + 1 ) = (ULONG_PTR)pMem; //ShellCode地址，就在申请的内存开头
#endif
	
	
	//把修改后的入口点指令写入到目标进程
	bResult = WriteProcessMemory(pi.hProcess,pExeEntry,newEntry,JmpCodeSize,&dwIoCnt);
	if (!bResult)
	{
		printf("[-] 向目标进程写入ShellCode失败!\n");
		return FALSE;
	}
	printf("[*] 向Exe入口写入JmpCode成功!\n");

	if (lpProcessInformation != NULL)
	{
		memcpy(lpProcessInformation,&pi,sizeof(PROCESS_INFORMATION));
	}

	//For Debug
	//printf("Press Enter to continue thread...\n");
	//getchar();

	//恢复线程执行
	if (!(dwCreationFlags & CREATE_SUSPENDED))
	{
		printf("[*] 恢复线程执行!\n");
		ResumeThread(pi.hThread);
	}

	return bResult;
}

ULONG_PTR FindImageBase(HANDLE hProc,LPSTR lpCommandLine)
{
	ULONG_PTR uResult = 0 ;
	TCHAR szBuf[1024]={0};
	SIZE_T dwSize = 0 ;
	PBYTE pAddress = NULL ;
	char szLowerCmdLine[MAX_PATH] = {0};
	char szLowerImageFile[MAX_PATH]={0};
	
	lstrcpy(szLowerCmdLine,lpCommandLine);
	strlwr(szLowerCmdLine);

	MEMORY_BASIC_INFORMATION mbi = {0};
	BOOL bFoundMemImage = FALSE ;
	char szImageFilePath[MAX_PATH]={0};
	char *pFileNameToCheck = strrchr(lpCommandLine,'\\');
	pFileNameToCheck = (pFileNameToCheck == NULL) ? lpCommandLine : (pFileNameToCheck + 1) ;

	//获取页的大小
	SYSTEM_INFO sysinfo;
	ZeroMemory(&sysinfo,sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysinfo);
	
	//Found First MEM_IMAGE Page
	pAddress = (PBYTE)sysinfo.lpMinimumApplicationAddress;
	while (pAddress < (PBYTE)sysinfo.lpMaximumApplicationAddress)
	{
		ZeroMemory(&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		dwSize = VirtualQueryEx(hProc,pAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		if (dwSize == 0)
		{
			pAddress += sysinfo.dwPageSize ;
			continue;
		}

		switch(mbi.State)
		{
		case MEM_FREE:
		case MEM_RESERVE:
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
			break;
		case MEM_COMMIT:
			if (mbi.Type == MEM_IMAGE)
			{
				if (GetMappedFileName(hProc,pAddress,szImageFilePath,MAX_PATH) != 0)
				{
					//printf("%s\n",szImageFilePath);
					lstrcpy(szLowerImageFile,szImageFilePath);
					strlwr(szLowerImageFile);
					if (strstr(szLowerImageFile,szLowerCmdLine) != 0)
					{
						bFoundMemImage = TRUE;
						uResult = (ULONG_PTR)pAddress;
						break;
					}
				}
			}
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
		    break;
		default:
		    break;
		}

		if (bFoundMemImage)
		{
			break;
		}
	}
	return uResult ;
}


#ifndef _WIN64
__declspec( naked )
VOID ShellCodeFun(VOID)
{
	__asm
	{
	push eax  //占位，后面将被修改为转移地址
	pushad
	pushfd
	call Next
Next:
	pop ebx
	and bx,0 //ebx低位清零,定位到代码开头
	mov eax, dword ptr ds:[ebx]INJECT_DATA.ExeEntry
	xchg [esp+0x24],eax  //交换eax为返回地址
	lea eax,dword ptr ds:[ebx]INJECT_DATA.ModuleHandle
	push eax		//pModuleHandle
	push dword ptr ds:[ebx]INJECT_DATA.pDllPath		//pDllPath
	lea eax,dword ptr ds:[ebx]INJECT_DATA.DllCharacteristics
	push eax //DllCharacteristics
	xor eax,eax
	push eax //PathToFile
	call dword ptr ds:[ebx]INJECT_DATA.AddrOfLdrLoadDll         ;  LdrLoadDll
	mov edi,dword ptr ds:[ebx]INJECT_DATA.ExeEntry      ;  edi指向exe入口
	lea esi,dword ptr ds:[ebx]INJECT_DATA.SavedEntryCode;  指向保存的指令
	mov ecx,2
	rep movsd
	lea eax, dword ptr ds:[ebx]INJECT_DATA.OldProtect		;OldProtect
	push eax
	push dword ptr ds:[ebx]INJECT_DATA.OldProtect
	lea eax, dword ptr ds:[ebx]INJECT_DATA.ProtectSize		;Size
	push eax
	lea eax, dword ptr ds:[ebx]INJECT_DATA.ExeEntry		;  ExeEntry
	push eax								
	push 0xFFFFFFFF
	call dword ptr ds:[ebx]INJECT_DATA.AddrOfZwProtectVirtualMemory			;  ZwProtectVirtualMemory
	popfd
	popad
	retn
	nop// 5个nop作为结束标记
	nop
	nop
	nop
	nop
	}
}
#endif

//For Get Shellcode
VOID WINAPI LoadDllAndRestoreExeEntry(INJECT_DATA *pData)
{
	PFN_LdrLoadDll fnLoadDll = (PFN_LdrLoadDll)pData->AddrOfLdrLoadDll;
	PFN_ZwProtectVirtualMemory fnProtectVm = (PFN_ZwProtectVirtualMemory)pData->AddrOfZwProtectVirtualMemory;
	memcpy((PVOID)pData->ExeEntry,pData->SavedEntryCode,sizeof(ULONG_PTR)*2);
	fnProtectVm((HANDLE)-1,(PVOID*)&pData->ProtectBase,&pData->ProtectSize,pData->OldProtect,&pData->OldProtect);
	fnLoadDll(NULL,&pData->DllCharacteristics,pData->pDllPath,&pData->ModuleHandle);
}

VOID PrepareShellCode(BYTE *pOutShellCode)
{
#ifdef _WIN64
	//不能使用__asm了，只能直接写ShellCode或单独写入asm文件中
	/*ShellCode的一种写法
	BYTE ShellCode64[]=
		"\x50"							//push    rax
		"\x51"							//push    rcx
		"\x52"							//push    rdx
		"\x53"							//push    rbx
		"\x56"							//push    rsi
		"\x57"							//push    rdi
		"\x41\x50"						//push    r8
		"\x41\x51"			            //push    r9
		"\x9c"							//pushfq
		"\xe8\x00\x00\x00\x00"			//call  next
		"\x5b"							//pop     rbx
		"\x66\x83\xe3\x00"				//and     bx,0
		"\x48\x8b\x83\xd0\x00\x00\x00"  //mov     rax,qword ptr [rbx+D0h]	;OriginalEIP
		"\x48\x87\x44\x24\x40"			//xchg    rax,qword ptr [rsp+40h]	;saved retnaddr
		//RestoreEntryCode
		"\x48\x8b\xbb\xd0\x00\x00\x00"  //mov     rdi,qword ptr [rbx+0D0h]	;  rdi指向exe入口
		"\x48\x8d\xb3\xC0\x00\x00\x00"  //lea     rsi,[rbx+0D0h]			;  指向保存的指令
		"\x48\x31\xc9"					//xor     rcx,rcx
		"\x83\xc1\x02"					//add     ecx,2
		"\x48\xa5"						//movs    qword ptr [rdi],qword ptr [rsi]
		//LdrLoadDll
		"\x48\x83\xec\x30"				//sub     rsp,30h
		"\x4C\x8d\x8b\xa0\x00\x00\x00"	//lea     r9,[rbx+A0h];ModuleHandle
		"\x4C\x8d\x83\xa8\x00\x00\x00"	//lea     r8,[rbx+A8h];ModuleFileName
		"\x48\x8d\x93\xA0\x00\x00\x00"	//lea     rdx,[rbx+A0h];DllCharacteristics
		"\x48\x31\xc9"					//xor rcx,rcx
		"\xff\x93\xb8\x00\x00\x00"		//call    qword ptr [rbx+0B8h];  LdrLoadDll
		//ProtectVM
		"\x48\x83\xec\x30"				//sub     rsp,30h
		"\x8d\x83\xc8\x00\x00\x00"		//lea     eax,[rbx+0C8h]			;OldProtect
		"\x48\x89\x44\x24\x20"			//mov     qword ptr [rsp+20h],rax
		"\x4c\x8b\x8b\xc8\x00\x00\x00"	//mov     r9,qword ptr [rbx+0C8h]	;OldProtect
		"\x4C\x8d\x83\xCC\x00\x00\x00"	//lea     r8,[rbx+A8h];ProtectSize
		"\x48\x8d\x93\xD0\x00\x00\x00"	//lea     rdx,[rbx+D0h];		;  ExeEntry								
		"\x48\x33\xC0"					//xor rax,rax
		"\x48\x8d\x48\xff"				//lea rcx,[rax-1] ;CurrentProcess
		"\xff\x93\xC0\x00\x00\x00"		//call    qword ptr [rbx+0C0h]; ZwProtectVirtualMemory
		"\x9d"							//popfq
		"\x41\x59"						//pop     r9
		"\x41\x58"						//pop     r8
		"\x5f"							//pop     rdi
		"\x5e"							//pop     rsi
		"\x5b"							//pop     rbx
		"\x5a"							//pop     rdx
		"\x59"							//pop     rcx
		"\xc3"							//ret
		"\x90\x90\x90\x90\x90"			//nop
		;
	*/

	//ShellCode的另一种写法
	//On ShellCode Begin ; Esp % 16 = 8 ,所以必须push奇数个
	/*
	BYTE ShellCode64[256] = 
		"\x50"							//push    rax//占位
		"\x51"							//push    rcx
		"\x52"							//push    rdx
		"\x53"							//push    rbx
		"\x55"							//push    rbp
		"\x56"							//push    rsi
		"\x57"							//push    rdi
		"\x41\x51"			            //push    r9
		"\x9c"							//pushfq
		"\xe8\x00\x00\x00\x00"			//call  next
		"\x5b"							//pop     rbx
		"\x66\x83\xe3\x00"				//and     bx,0
		"\x48\x8b\x83\xe0\x00\x00\x00"  //mov     rax,qword ptr [rbx+E0h]	;OriginalEIP
		"\x48\x87\x44\x24\x40"			//xchg    rax,qword ptr [rsp+40h]	;saved retnaddr
		"\x48\x8b\xcb"					//mov     rcx,rbx
		"\xE8\x0A\x00\x00\x00"			//call	  LoadDllAndRestoreExeEntry
		"\x9d"							//popfq
		"\x41\x59"						//pop     r9
		"\x5f"							//pop     rdi
		"\x5e"							//pop     rsi
		"\x5d"							//pop     rbp
		"\x5b"							//pop     rbx
		"\x5a"							//pop     rdx
		"\x59"							//pop     rcx
		"\xc3"							//ret
		//LoadDllAndRestoreExeEntry
		"\x48\x89\x5C\x24\x08"			//mov        qword ptr ss:[rsp+08h], rbx
		"\x57"							//push       rdi
		"\x48\x83\xEC\x30"				//sub        rsp, 30h
		"\x48\x8B\x91\xF8\x00\x00\x00"  //mov        rdx, qword ptr ds:[rcx+000000F8h]
		"\x48\x8B\x81\x08\x01\x00\x00"  //mov        rax, qword ptr ds:[rcx+00000108h]
		"\x48\x8B\xB9\xD8\x00\x00\x00"  //mov        rdi, qword ptr ds:[rcx+000000D8h]
		"\x4C\x8B\x91\x00\x01\x00\x00"  //mov        r10, qword ptr ds:[rcx+00000100h]
		"\x48\x89\x02"					//mov        qword ptr ds:[rdx], rax
		"\x48\x8B\x81\x10\x01\x00\x00"  //mov        rax, qword ptr ds:[rcx+00000110h]
		"\x48\x89\x42\x08"				//mov        qword ptr ds:[rdx+08h], rax
		"\x48\x8D\x81\xE8\x00\x00\x00"  //lea        rax, qword ptr ds:[rcx+000000E8h]
		"\x4C\x8D\x81\xF0\x00\x00\x00"  //lea        r8, qword ptr ds:[rcx+000000F0h]
		"\x44\x8B\x08"					//mov        r9d, dword ptr ds:[rax]
		"\x48\x8D\x91\xE0\x00\x00\x00"  //lea        rdx, qword ptr ds:[rcx+000000E0h]
		"\x48\x8B\xD9"					//mov        rbx, rcx
		"\x48\x83\xC9\xFF"				//or         rcx, FFFFFFFFFFFFFFFFh
		"\x48\x89\x44\x24\x20"			//mov        qword ptr ss:[rsp+20h], rax
		"\x41\xFF\xD2"					//call       r10
		"\x4C\x8B\x83\xC8\x00\x00\x00"  //mov        r8, qword ptr ds:[rbx+000000C8h]
		"\x4C\x8D\x8B\xC0\x00\x00\x00"  //lea        r9, qword ptr ds:[rbx+000000C0h]
		"\x48\x8D\x93\xD0\x00\x00\x00"  //lea        rdx, qword ptr ds:[rbx+000000D0h]
		"\x33\xC9"						//xor        ecx, ecx
		"\x48\x8B\xC7"					//mov        rax, rdi
		"\x48\x8B\x5C\x24\x40"			//mov        rbx, qword ptr ss:[rsp+40h]
		"\x48\x83\xC4\x30"				//add        rsp, 30h
		"\x5F"							//pop        rdi
		"\x48\xFF\xE0"					//jmp        rax
		"\x90\x90\x90\x90\x90"
		;
	BYTE *pShellcodeStart = ShellCode64 ;
	*/

	BYTE *pShellcodeStart = (BYTE*)ShellCodeFun64;
#else
	BYTE *pShellcodeStart = (BYTE*)ShellCodeFun;
#endif
	BYTE *pShellcodeEnd = 0 ;
	SIZE_T ShellCodeSize = 0 ;
	if (pShellcodeStart[0] == 0xE9)
	{
		//Debug模式下，函数开头是一个跳转指令，这里取它的真正地址
		pShellcodeStart = pShellcodeStart + *(ULONG*)(pShellcodeStart +1 ) + 5;
	}

	//搜索Shellcode结束标志
	pShellcodeEnd = pShellcodeStart;
	while (memcmp(pShellcodeEnd,"\x90\x90\x90\x90\x90",5) != 0)
	{
		pShellcodeEnd++;
	}

	ShellCodeSize = pShellcodeEnd - pShellcodeStart;
	printf("[*] Shellcode Len = %d\n",ShellCodeSize);
	memcpy(pOutShellCode,pShellcodeStart,ShellCodeSize);

}

//以下代码用来测试和调试ShellCode
VOID TestShellCode()
{
	HANDLE hProc = GetCurrentProcess();
	INJECT_DATA Data;
	PBYTE pMem = (PBYTE)VirtualAllocEx(hProc,NULL,0x1000,MEM_RESERVE | MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	
	if (pMem == NULL)
	{
		printf("Alloc Memory Failed!\n");
		return ;
	}
	printf("Alloced mem = 0x%p\n",pMem);

	ZeroMemory(&Data,sizeof(INJECT_DATA));
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");

	ULONG_PTR uNtdllBaseInTargetProc = (ULONG_PTR)hNtdll;
	ULONG_PTR uAddrofZwProtectMemmoryInCurProc = (ULONG_PTR)GetProcAddress(hNtdll,"ZwProtectVirtualMemory");
	ULONG_PTR uAddrofLdrLoadDllInCurProc = (ULONG_PTR)GetProcAddress(hNtdll,"LdrLoadDll");
	ULONG_PTR uAddrofLdrLoadDllInTargetProc = uAddrofLdrLoadDllInCurProc - (ULONG_PTR)hNtdll + uNtdllBaseInTargetProc;
	ULONG_PTR uAddrofZwProtectMemmoryInTargetProc = uAddrofZwProtectMemmoryInCurProc - (ULONG_PTR)hNtdll + uNtdllBaseInTargetProc;
	
	printf("Data = 0x%p FirstElement = 0x%p\n",&Data,&Data.ModuleHandle);
	//保存ShellCode
	PrepareShellCode(Data.ShellCode);
	
	//将路径转换为Unicode
	MultiByteToWideChar(CP_ACP,0,g_szDllPath,-1,Data.wDllPath,MAX_PATH * sizeof(WCHAR));
	//初始化UNICODE_STRING
	Data.usDllPath.Buffer = (PWCHAR)(FIELD_OFFSET(INJECT_DATA,wDllPath) + (ULONG_PTR)pMem);
	Data.usDllPath.MaximumLength = (USHORT)(MAX_PATH * sizeof(WCHAR));
	Data.usDllPath.Length = (USHORT)(wcslen(Data.wDllPath) * sizeof(WCHAR));
	//设置
	Data.pDllPath = (PUNICODE_STRING)(FIELD_OFFSET(INJECT_DATA,usDllPath) + (ULONG_PTR)pMem);
	Data.AddrOfLdrLoadDll = uAddrofLdrLoadDllInTargetProc;
	Data.AddrOfZwProtectVirtualMemory = uAddrofZwProtectMemmoryInTargetProc;
	memset(Data.SavedEntryCode,0x90,8);
	Data.ExeEntry = (ULONG_PTR)TestShellCode;
	printf("ExeEntry = 0x%p\n",Data.ExeEntry);
	
	DWORD dwOldProtect = 0 ;
	BOOL bResult = VirtualProtectEx(hProc,(LPVOID)Data.ExeEntry,16,PAGE_EXECUTE_READWRITE,&dwOldProtect);
	DWORD dwErrorCode = GetLastError();
	printf("ErrCode = %d\n",dwErrorCode);
	Data.ProtectBase = Data.ExeEntry ;
	Data.OldProtect = dwOldProtect;
	Data.ProtectSize = 8 ;

	memcpy(pMem,&Data,sizeof(INJECT_DATA));
	
	

	typedef VOID (*PSHELLCODE)(VOID);

	PSHELLCODE pCode = (PSHELLCODE)pMem;

	DebugBreak();
	pCode();
	//LoadDllAndProtectCode(&Data);

	printf("after call shellcode.\n");
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;
	
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
	
    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            // handle error
        }
    }
    return bIsWow64;
}

