/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


int Add(int nNum1, int nNum2, int nNum3, int nNum4, int nNum5, int nNum6) {
	return nNum1 + nNum2 + nNum3 + nNum4 + nNum5 + nNum6;
}

int _tmain(int argc, _TCHAR* argv[]) {
	printf("%d\r\n", Add(1, 2, 3, 4, 5, 6));
	return 0;
}
