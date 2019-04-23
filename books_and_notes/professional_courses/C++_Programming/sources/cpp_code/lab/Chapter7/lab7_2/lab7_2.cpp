#include <iostream>
using namespace std;

class BaseClass {
public:
	int Number;
	int getNumber() {return Number;}
};

class DerivedClass : BaseClass {
public:
	DerivedClass() {
		Number = 0;
		Number ++;
		cout << "Construction. Number = " << getNumber() << endl;
	}
	~DerivedClass() {
		Number --;
		cout << "Destruction. Number = " << getNumber() << endl;
	}
};

int main()
{
	DerivedClass d;
	return 0;
}
