// SEH_NoSafeSEH_JUMP.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}
void jump()
{
__asm{
	pop eax
	pop eax
	retn
	}
}


