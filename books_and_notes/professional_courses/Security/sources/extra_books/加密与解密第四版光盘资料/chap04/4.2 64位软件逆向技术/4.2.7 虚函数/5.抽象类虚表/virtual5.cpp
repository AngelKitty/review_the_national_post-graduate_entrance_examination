/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

// Example4-5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


class IBase {
public:
	IBase() {
		m_nMember = 1;
		printf("IBase()\r\n");
	}
	virtual void fun1() = 0;
	virtual void fun2() = 0;
private:
	int m_nMember;
};

class CDerived :public IBase
{
public:
	CDerived()
	{
		printf("CDerived()\r\n");
	}
	virtual void fun1(){};
	virtual void fun2() {};
};

int _tmain(int argc, _TCHAR* argv[]) {
	IBase *pBase = new CDerived();
	delete pBase;
	return 0;
}
