//6_10.cpp
#include <iostream>
using namespace std;

//将实数x分成整数部分和小数部分，形参intpart、fracpart是指针
void splitFloat(float x, int *intPart, float *fracPart) {
	*intPart = static_cast<int>(x);	//取x的整数部分
	*fracPart = x - *intPart;		//取x的小数部分
}

int main() {
	cout << "Enter 3 float point numbers:" << endl;
	for(int i = 0; i < 3; i++) {
		float x, f;
		int n;
		cin >> x;
		splitFloat(x, &n, &f);	//变量地址作为实参
		cout << "Integer Part = " << n << " Fraction Part = " << f << endl;
	}
	return 0;
}
