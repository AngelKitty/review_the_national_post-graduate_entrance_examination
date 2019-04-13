//5_9.cpp
#include <iostream>
#include <cmath>
using namespace std;

class Point {	//Point类定义
public:	//外部接口
	Point(int x = 0, int y = 0) : x(x), y(y) { }
	int getX() { return x; }
	int getY() { return y; }
	friend float dist(const Point &p1, const Point &p2);
private:	//私有数据成员
	int x, y;
};

float dist(const Point &p1, const Point &p2) {	//常引用作形参
	double x = p1.x - p2.x;	
	double y = p1.y - p2.y;
	return static_cast<float>(sqrt(x * x + y * y));
}

int main() {	//主函数
	const Point myp1(1, 1), myp2(4, 5);	//定义Point类的对象
	cout << "The distance is: ";
	cout << dist(myp1, myp2) << endl;	//计算两点间的距离
	return 0;
}
