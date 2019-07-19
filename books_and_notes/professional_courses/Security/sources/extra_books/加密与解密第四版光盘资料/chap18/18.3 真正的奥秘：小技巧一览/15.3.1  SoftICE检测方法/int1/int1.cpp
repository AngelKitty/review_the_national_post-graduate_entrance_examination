/*--------------------------------------------------------------
   int1.cpp -- 利用int 1检测SoftICE
                           (c) www.pediy.com 段钢, 2003
  --------------------------------------------------------------*/

#include <windows.h>

BOOL SoftICELoaded();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if( SoftICELoaded() )
		MessageBox(NULL,TEXT ("SoftICE is active!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,TEXT ("Can't find SoftICE with this method!"),TEXT ("Error"),MB_ICONEXCLAMATION);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// 此法仅Windows 2000/XP系统有效，实际操作时，还必须加入判断操作系统的代码
//////////////////////////////////////////////////////////////////////

BOOL SoftICELoaded()
{
	int mark=0;
	_asm {
		

			push offset handler
			push dword ptr fs:[0]        // 建立SEH链
			mov  dword ptr fs:[0],esp
			xor  eax,eax
			int 1              // 如果SICE已启动,且存在3级调试器,则不发生异常,否则发生异常
			inc  eax
			inc  eax
			pop  dword ptr fs:[0] 
			add esp,4
			or   eax,eax
			jz   found
			cmp mark, 0
            jnz   found
			jmp  Nofound

	handler:   	// 异常回调处理函数		
	    	mov ebx,[esp+0ch]
			add dword ptr [ebx+0b8h],02h  // CONTEXT.EIP+2
		    mov ebx,[esp+4] 
			cmp [ebx], 80000004h  // 0C0000005正常,80000004被调试
			jz Table
			inc mark          
     Table:			
			xor eax,eax
			ret                           //表示已经修复，从现在的CONTEXT.EIP处执行
	
			}
Nofound:
		return FALSE;	
		
found:
		return TRUE;

}