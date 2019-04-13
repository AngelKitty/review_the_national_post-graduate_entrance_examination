//5_5.cpp
#include <iostream>
using namespace std;

class Point {	//Point类定义
public:	//外部接口
	Point(int x = 0, int y = 0) : x(x), y(y) { //构造函数
		//在构造函数中对count累加，所有对象共同维护同一个count
		count++;
	}	
	Point(Point &p) {	//拷贝构造函数
		x = p.x;
		y = p.y;
		count++;
	}
	~Point() {  count--; }
	int getX() { return x; }
	int getY() { return y; }

	static void showCount() {		//静态函数成员
		cout << "  Object count = " << count << endl;
	}
private:	//私有数据成员
	int x, y;
	static int count;	//静态数据成员声明，用于记录点的个数
};

int Point::count = 0;	//静态数据成员定义和初始化，使用类名限定

int main() {	//主函数
	Point a(4, 5);	//定义对象a，其构造函数回使count增1
	cout << "Point A: " << a.getX() << ", " << a.getY();
	Point::showCount();	//输出对象个数

	Point b(a);	//定义对象b，其构造函数回使count增1
	cout << "Point B: " << b.getX() << ", " << b.getY();
	Point::showCount();	//输出对象个数

	return 0;
}
