/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


int g_ary[4] = { 4, 5, 6, 7 };
int _tmain(int argc, _TCHAR* argv[]) {
	int ary[4] = { 1, 2, 3, 4 };
	printf("%d %d\r\n", ary[2], ary[argc]);
	printf("%d %d\r\n", g_ary[3], g_ary[argc]);
	return 0;
}
