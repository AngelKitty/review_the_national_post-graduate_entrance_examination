//6_13.cpp
#include <iostream>
using namespace std;

class Point {	//类的定义
public:	//外部接口
	Point(int x = 0, int y = 0) : x(x), y(y) { }	//构造函数
	int getX() const { return x; }	//返回x
	int getY() const { return y; }	//返回y
private:	//私有数据
	int x, y;
};

int main() {	//主函数
	Point a(4,5);	//定义对象A
	Point *p1 = &a;	//定义对象指针并初始化
	int (Point::*funcPtr)() const = &Point::getX;	//定义成员函数指针并初始化
	
	cout << (a.*funcPtr)() << endl;		//(1)使用成员函数指针和对象名访问成员函数
	cout << (p1->*funcPtr)() << endl;	//(2)使用成员函数指针和对象指针访问成员函数
	cout << a.getX() << endl;			//(3)使用对象名访问成员函数
	cout << p1->getX() << endl;			//(4)使用对象指针访问成员函数

	return 0;
}
