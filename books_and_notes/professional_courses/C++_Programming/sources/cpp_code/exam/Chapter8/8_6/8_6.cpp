//8_6.cpp
#include <iostream>
using namespace std;

class Base1 { //基类Base1定义
public:
	virtual void display() const = 0;	//纯虚函数
};

class Base2: public Base1 { //公有派生类Base2定义
public:
	void display() const;	//覆盖基类的虚函数
};
void Base2::display() const {
	cout << "Base2::display()" << endl;
}

class Derived: public Base2 { //公有派生类Derived定义
public:
	void display() const;	//覆盖基类的虚函数
};
void Derived::display() const {
	cout << "Derived::display()" << endl;
}

void fun(Base1 *ptr) { //参数为指向基类对象的指针
	ptr->display();	//"对象指针->成员名"
}

int main() {	//主函数
	Base2 base2;	//定义Base2类对象
	Derived derived;	//定义Derived类对象
	fun(&base2);	//用Base2对象的指针调用fun函数
	fun(&derived);	//用Derived对象的指针调用fun函数
	return 0;
}
