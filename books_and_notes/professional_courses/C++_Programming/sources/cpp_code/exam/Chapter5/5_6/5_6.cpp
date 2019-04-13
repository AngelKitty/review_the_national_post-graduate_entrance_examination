//5_6.cpp
#include <iostream>
#include <cmath>
using namespace std;

class Point {	//Point类定义
public:	//外部接口
	Point(int x = 0, int y = 0) : x(x), y(y) { }
	int getX() { return x; }
	int getY() { return y; }
	friend float dist(Point &p1, Point &p2);	//友元函数声明
private:	//私有数据成员
	int x, y;
};

float dist(Point &p1, Point &p2) {	//友元函数实现
	double x = p1.x - p2.x;	//通过对象访问私有数据成员
	double y = p1.y - p2.y;
	return static_cast<float>(sqrt(x * x + y * y));
}

int main() {	//主函数
	Point myp1(1, 1), myp2(4, 5);	//定义Point类的对象
	cout << "The distance is: ";
	cout << dist(myp1, myp2) << endl;	//计算两点间的距离
	return 0;
}
