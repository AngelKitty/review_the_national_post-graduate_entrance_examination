//3_1.cpp
#include <iostream>
using namespace std;

//计算x的n次方
double power(double x, int n) {
	double val = 1.0;
	while (n--)
		val *= x;
	return val;
}

int main() {
	cout <<  "5 to the power 2 is " << power(5, 2) << endl; 
	//函数调用作为一个表达式出现在输出语句中。
	return 0;
}
