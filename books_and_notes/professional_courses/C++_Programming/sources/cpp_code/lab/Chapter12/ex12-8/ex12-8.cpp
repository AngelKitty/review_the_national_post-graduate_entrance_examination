#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
using namespace std;

class SomeClass
{
public:
	SomeClass(string name):str(name){cout << str <<": In construct function."<<endl;}
	~SomeClass(){cout<< str <<": In Destruct function."<<endl;}
	SomeClass(SomeClass &){cout<<str <<": In copy construct function."<<endl;}
	SomeClass & operator = (SomeClass &){cout<<str <<": In assignment function."<<endl;return *this;}
private:
	string str;
};

int main()
{
	auto_ptr<SomeClass> ptr1(new SomeClass("ptr1"));
	auto_ptr<SomeClass> ptr2(new SomeClass("ptr2"));

	*ptr2 = *ptr1;

	{
		auto_ptr<SomeClass> ptr3(ptr2);
		ptr1 = ptr3;
	}

	return 0;
}
