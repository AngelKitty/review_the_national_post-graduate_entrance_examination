#include "windows.h"
#include "tchar.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
/*ffe4.cpp By Backend
*/
bool we_loaded_it = false;
HINSTANCE h;
TCHAR dllname[] = _T("User32");

if(argc>1)      {
        strcpy(dllname,argv[1]);
}

h = GetModuleHandle(dllname);
if(h == NULL)
{
h = LoadLibrary(dllname);
cout<<"DLL " << dllname << " at 0x"<<hex<<h<<endl;
if(h == NULL)
{
cout<<"ERROR LOADING DLL: "<<dllname<<endl;
return 1;
}
we_loaded_it = true;
}

BYTE* ptr = (BYTE*)h;
bool done = false;
for(int y = 0;!done;y++)
{
try
{
if(ptr[y] == 0xFF && ptr[y+1] == 0xE4)
{
int pos = (int)ptr + y;
cout<<"OPCODE found at 0x"<<hex<<pos<<endl;
}
}
catch(...)
{
cout<<"END OF "<<dllname<<" MEMORY REACHED"<<endl;
done = true;
}
}
if(we_loaded_it) FreeLibrary(h);
}