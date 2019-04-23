#include <iostream>
using namespace std;

class Base0 {	//定义基类Base0
public:
	int var0;
	void fun0() { cout << "Member of Base0" << endl; }
};

class Base1: virtual public Base0 {	//定义派生类Base1 
public:	//新增外部接口
	int var1;
};

class Base2: virtual public Base0 {	//定义派生类Base2 
public:	//新增外部接口
	int var2;
};

class Derived: public Base1, public Base2 {	//定义派生类Derived 
public:	//新增外部接口
	int var;
	void fun() { cout << "Member of Derived" << endl; }
};

int main() {
	Derived d;	
	Base0 *p = &d;

	cout << "Base pointer: " << &d << endl;
	cout << "Base1's pointer value: " << p << endl;
	cout << "Base1's var pointer: " << &(d.Base1::var1) << endl;
	cout << "Base2's var pointer: " << &(d.Base2::var2) << endl;
	cout << "Derived's var pointer: " << &(d.var) << endl;
	cout << "Base1's Base0's var0 pointer: " << &(d.Base1::Base0::var0) << endl;
	cout << "Base2's Base0's var0 pointer: " << &(d.Base2::Base0::var0) << endl;

	return 0;
}
