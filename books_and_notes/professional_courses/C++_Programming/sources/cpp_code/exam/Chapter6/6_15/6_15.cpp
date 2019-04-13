//6_15.cpp
#include <iostream>
using namespace std;

class Point {	//Point类定义
public:	//外部接口
	Point(int x = 0, int y = 0) : x(x), y(y) { //构造函数
		count++;
	}	
	Point(const Point &p) : x(p.x), y(p.y) {	//拷贝构造函数
		count++;
	}
	~Point() {  count--; }
	int getX() const { return x; }
	int getY() const { return y; }

	static void showCount() {		//输出静态数据成员
		cout << "  Object count = " << count << endl;
	}

private:	//私有数据成员
	int x, y;
	static int count;	//静态数据成员声明，用于记录点的个数
};

int Point::count = 0;	//静态数据成员定义和初始化，使用类名限定

int main() {	//主函数实现
	void (*funcPtr)() = Point::showCount;	//定义一个指向函数的指针，指向类的静态成员函数

	Point a(4, 5);	//定义对象A
	cout << "Point A: " << a.getX() << ", " << a.getY();
	funcPtr();	//输出对象个数，直接通过指针访问静态函数成员

	Point b(a);	//定义对象B
	cout << "Point B: " << b.getX() << ", " << b.getY();
	funcPtr();	//输出对象个数，直接通过指针访问静态函数成员

	return 0;
}
