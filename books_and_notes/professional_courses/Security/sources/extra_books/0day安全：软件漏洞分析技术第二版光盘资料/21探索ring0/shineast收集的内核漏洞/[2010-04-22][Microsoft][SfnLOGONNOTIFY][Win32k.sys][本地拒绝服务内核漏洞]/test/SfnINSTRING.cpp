#include "stdio.h"
#include "windows.h"
int main(int argc, char* argv[])
{
	char title[MAX_PATH]={0};
	DWORD pid=0;

	 
	HWND hwnd = FindWindow("DDEMLEvent" , NULL); 
	if (hwnd == 0 )
	{
		printf("cannot find DDEMLEvent Window!\n");
		return 0 ; 
	}

	GetWindowText(hwnd,title,MAX_PATH);
	printf("hwnd=%08X title=%s\n",hwnd,title);   

	GetWindowThreadProcessId(hwnd,&pid);
	printf("pid=%d\n",pid); 


	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	printf("hProcess=%08X\n",hProcess);   



	//PostMessage(hwnd , 0x4c , 0x4 , 0x80000000);
	return 0 ;
}
