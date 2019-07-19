// ShowCPPExp.cpp : Defines the entry point for the console application.
//
/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	char *szStr = NULL ;
	try
	{
		cout<<"In Try."<<endl;
		if (szStr == NULL)
		{
			throw szStr;
		}
		szStr[0] = 'A';
	}
	catch (int e)
	{
		cout<<"int exception catched."<<endl;	
	}
	catch (char* e)
	{
		cout<<"string exception catched."<<endl;
	}
	catch (...)
	{
		cout<<"unknown exception catched."<<endl;
	}
	return 0;
}

