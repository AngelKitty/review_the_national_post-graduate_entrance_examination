/*--------------------------------------------
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------*/

#include "stdafx.h"


struct tagPoint {
	int x1;
	int y1;
	int x2;
	int y2;
};
void fun(tagPoint pt) {
	printf("x1=%d y1=%d x2=%d y2=%d\r\n", pt.x1, pt.y1, pt.x2, pt.y2);
}

int _tmain(int argc, _TCHAR* argv[]) {
	tagPoint pt = { 1, 2, 3, 4 };
	fun(pt);
	return 0;
}
