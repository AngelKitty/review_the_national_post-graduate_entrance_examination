// AcsVio.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
    HINSTANCE hLib; 
    int * ProcAdd; 
 
	printf("Hi, every one. \nThis simple utility will simulate an access violation.\n--Raymond\n");
    // Get a handle to the DLL module.
    hLib = LoadLibrary("Kernel32.dll"); 
 
    // If the handle is valid, try to get the function address.
    if (hLib != NULL) 
    { 
        ProcAdd = (int *)GetProcAddress(hLib, "ReadFile"); 
 
        // If the function address is valid, call the function.
 
        if (NULL != ProcAdd) 
        {
			*ProcAdd = 0;
        }
 
        // Free the DLL module.
        FreeLibrary(hLib); 
    } 
 
	return 0;
}

