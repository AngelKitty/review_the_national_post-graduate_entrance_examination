/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


int g_ary[2][3] = { 7, 8, 9, 10, 11, 12 };

int _tmain(int argc, _TCHAR* argv[]) {
	int ary[2][3] = { 1, 2, 3, 4, 5, 6 };
	printf("%d %d\r\n", ary[1][2], ary[argc][1]);
	printf("%d %d\r\n", g_ary[0][1], g_ary[argc][2]);
	return 0;
}

