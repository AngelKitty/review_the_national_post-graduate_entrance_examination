//6_12.cpp
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
	Point a(4, 5);	//定义并初始化对象a
	Point *p1 = &a;	//定义对象指针，用a的地址将其初始化
	cout << p1->getX() << endl;	//利用指针访问对象成员
	cout << a.getX() << endl; 	//利用对象名访问对象成员
	return 0;
}
