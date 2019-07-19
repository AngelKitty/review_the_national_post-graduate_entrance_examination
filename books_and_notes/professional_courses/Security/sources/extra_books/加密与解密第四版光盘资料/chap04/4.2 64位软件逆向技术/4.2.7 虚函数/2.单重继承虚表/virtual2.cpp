/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

// Example4-2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


class CBase {
public:
	CBase() {
		m_nMember = 1;
		printf("CBase()\r\n");
	}
	virtual ~CBase() {
		printf("~CBase()\r\n");
	}
	virtual void fun1() {
		printf("CBase::fun1()\r\n");
	}
private:
	int m_nMember;
};

class CDerived :public CBase {
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
	virtual void fun2() {
		printf("CDerived::fun2()\r\n");
	}
private:
	int m_nMember;
};

int _tmain(int argc, _TCHAR* argv[]) {
	CBase *pBase = new CDerived();
	pBase->fun1();
	delete pBase;
	return 0;
}
