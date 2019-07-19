/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

// if-elseif-else.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
int _tmain(int argc, _TCHAR* argv[]) {
	if (argc > 2)
		printf("argc > 2\r\n");
	else if (argc == 2)
		printf("argc == 2\r\n");
	else
		printf("argc <= 1\r\n");
	return 0;
}
