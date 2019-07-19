/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


class CAdd {
public:
	int Add(int nNum1, int nNum2) {
		return nNum1 + nNum2;
	}
};

int _tmain(int argc, _TCHAR* argv[]) {
	CAdd Object;
	printf("%d\r\n", Object.Add(1, 2));
	return 0;
}

