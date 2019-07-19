/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

// Example4-4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class A {
public:
	A() {
		m_nMember = 1;
		printf("A()\r\n");
	}
	~A() {
		printf("~A()\r\n");
	}
	virtual void fun1() {
		printf("A::fun1()\r\n");
	}
private:
	int m_nMember;
};



class B :virtual public A{
public:
	B() {
		m_nMember = 2;
		printf("B()\r\n");
	}
	~B() {
		printf("~B()\r\n");
	}
	virtual void fun2() {
		printf("B::fun2()\r\n");
	}
private:
	int m_nMember;
};

class C :virtual public A{
public:
	C() {
		m_nMember = 3;
		printf("C()\r\n");
	}
	~C() {
		printf("~C()\r\n");
	}
	virtual void fun3() {
		printf("C::fun3()\r\n");
	}
private:
	int m_nMember;
};

class BC :public B, public C {
public:
	BC() {
		m_nMember = 4;
		printf("BC()\r\n");
	}
	~BC() {
		printf("~BC()\r\n");
	}
	virtual void fun3() {
		printf("BC::fun3()\r\n");
	}
	virtual void fun4() {
		printf("BC::fun4()\r\n");
	}
private:
	int m_nMember;
};

int _tmain(int argc, _TCHAR* argv[]) {
	BC theBC;
	return 0;
}


