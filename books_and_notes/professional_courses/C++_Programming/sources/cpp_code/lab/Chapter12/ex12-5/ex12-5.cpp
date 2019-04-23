#include <iostream>
using namespace std;

int main()
{
    char *buf;
    try
    {
        buf = new char[512];
        if( buf == 0 )
            throw "内存分配失败!";
    }
    catch( char * str )
    {
        cout << "有异常产生：" << str << endl;
    }
	return 0;
}
