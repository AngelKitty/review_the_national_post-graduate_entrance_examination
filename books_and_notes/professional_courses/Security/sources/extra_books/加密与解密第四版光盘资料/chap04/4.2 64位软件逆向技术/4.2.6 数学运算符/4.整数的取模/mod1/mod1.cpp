/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

//mod1.cpp
#include "stdafx.h"
int _tmain(int argc, _TCHAR* argv[]) {
	long long nNum;
	scanf("%ld", &nNum);
	printf("%d\r\n", argc % 3);
	printf("%d\r\n", nNum % 10);
	return 0;
}