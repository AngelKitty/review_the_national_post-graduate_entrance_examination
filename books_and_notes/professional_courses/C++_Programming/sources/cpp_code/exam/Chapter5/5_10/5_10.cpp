//文件3，主函数，5_10.cpp
#include "Point.h"
#include <iostream>
using namespace std;

int main() {
	Point a(4, 5);	//定义对象a，其构造函数回使count增1
	cout << "Point A: " << a.getX() << ", " << a.getY();
	Point::showCount();	//输出对象个数

	Point b(a);	//定义对象b，其构造函数回使count增1
	cout << "Point B: " << b.getX() << ", " << b.getY();
	Point::showCount();	//输出对象个数

	return 0;
}
