/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


struct tagPoint {
	int x1;
	int y1;
};

void fun(tagPoint pt) {
	printf("x=%d y=%d\r\n", pt.x1, pt.y1);
}

int _tmain(int argc, _TCHAR* argv[]) {
	tagPoint pt = { 1, 2 };
	fun(pt);
	return 0;
}
