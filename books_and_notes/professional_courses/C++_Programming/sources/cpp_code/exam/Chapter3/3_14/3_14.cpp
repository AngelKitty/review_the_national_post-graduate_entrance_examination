//3_14.cpp
#include <iostream>
using namespace std;

const double PI = 3.14159265358979;

//内联函数，根据圆的半径计算其面积
inline double calArea(double radius) {
	return PI * radius * radius;
}

int main() {
	double r = 3.0;	//r是圆的半径
	//调用内联函数求圆的面积，编译时此处被替换为CalArea函数体语句
	double area	= calArea(r);
	cout << area << endl;
	return 0;
}
