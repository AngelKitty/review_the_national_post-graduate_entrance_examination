//6_17.cpp
#include <iostream>
using namespace std;

class Point {
public:
	Point() : x(0), y(0) {
		cout<<"Default Constructor called."<<endl;
	}

	Point(int x, int y) : x(x), y(y) {
		cout<< "Constructor called."<<endl;
	}

	~Point() { cout<<"Destructor called."<<endl; }

	int getX() const { return x; }

	int getY() const { return y; }

	void move(int newX, int newY) {
		x = newX;
		y = newY;
	}
private:
	int x, y;
};

int main() {
	Point *ptr = new Point[2];	//创建对象数组
	ptr[0].move(5, 10);		//通过指针访问数组元素的成员
	ptr[1].move(15, 20);	//通过指针访问数组元素的成员
	cout << "Deleting..." << endl;
	delete[] ptr;	//删除整个对象数组
	return 0;
}
