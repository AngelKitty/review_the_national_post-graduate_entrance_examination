//7_7.cpp
#include <iostream>
using namespace std;

class Base0 {	//定义基类Base0
public:
	int var0;
	void fun0() { cout << "Member of Base0" << endl; }
};

class Base1: public Base0 {	//定义派生类Base1
public:	//新增外部接口
	int var1;
};

class Base2: public Base0 {	//定义派生类Base2
public:	//新增外部接口
	int var2;
};

class Derived: public Base1, public Base2 {	//定义派生类Derived
public:	//新增外部接口
	int var;
	void fun() { cout << "Member of Derived" << endl; }
};

int main() {	//程序主函数
	Derived d;			//定义Derived类对象d
	d.Base1::var0 = 2;	//使用直接基类
	d.Base1::fun0();
	d.Base2::var0 = 3;	//使用直接基类
	d.Base2::fun0();
	return 0;
}
