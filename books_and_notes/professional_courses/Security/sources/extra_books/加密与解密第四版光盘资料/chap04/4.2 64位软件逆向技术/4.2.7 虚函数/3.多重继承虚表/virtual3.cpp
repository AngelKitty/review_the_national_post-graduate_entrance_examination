/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

// Example4-3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class CBase1 {
public:
	CBase1() {
		m_nMember = 1;
		printf("CBase1()\r\n");
	}
	~CBase1() {
		printf("~CBase1()\r\n");
	}
	virtual void fun1() {
		printf("CBase1::fun1()\r\n");
	}
private:
	int m_nMember;
};
class CBase2 {
public:
	CBase2() {
		m_nMember = 2;
		printf("CBase2()\r\n");
	}
	~CBase2() {
		printf("~CBase2()\r\n");
	}
	virtual void fun2() {
		printf("CBase2::fun1()\r\n");
	}
private:
	int m_nMember;
};

class CDerived :public CBase1, public CBase2 {
public:
	CDerived() {
		m_nMember = 2;
		printf("CDerived()\r\n");
	}
	~CDerived() {
		printf("~CDerived()\r\n");
	}
	virtual void fun1() {
		printf("CDerived::fun1()\r\n");
	}
	virtual void fun3() {
		printf("CDerived::fun3()\r\n");
	}
private:
	int m_nMember;
};


int _tmain(int argc, _TCHAR* argv[]) {
	CDerived derievd;
	return 0;
}

