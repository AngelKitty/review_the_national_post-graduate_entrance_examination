//6_16.cpp
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
	cout << "Step one: " << endl;
	Point *ptr1 = new Point;	//动态创建对象，没有给出参数列表，因此调用缺省构造函数
	delete ptr1;	//删除对象，自动调用析构函数

	cout << "Step two: " << endl;
	ptr1 = new Point(1,2);		//动态创建对象，并给出参数列表，因此调用有形参的构造函数
	delete ptr1;	//删除对象，自动调用析购函数

	return 0;
}
