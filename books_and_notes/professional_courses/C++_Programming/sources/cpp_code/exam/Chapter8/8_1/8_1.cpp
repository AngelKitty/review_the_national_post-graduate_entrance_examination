//8_1.cpp
#include <iostream>
using namespace std;

class Complex {	//复数类定义
public:	//外部接口
	Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) { }	//构造函数
	Complex operator + (const Complex &c2) const;	//运算符+重载成员函数
	Complex operator - (const Complex &c2) const;	//运算符-重载成员函数
	void display() const;	//输出复数
private:	//私有数据成员
	double real;	//复数实部
	double imag;	//复数虚部
};	

Complex Complex::operator + (const Complex &c2) const {	//重载运算符函数实现
	return Complex(real + c2.real, imag + c2.imag); //创建一个临时无名对象作为返回值
}

Complex Complex::operator - (const Complex &c2) const {	//重载运算符函数实现
	return Complex(real - c2.real, imag - c2.imag); //创建一个临时无名对象作为返回值
}

void Complex::display() const {
	cout << "(" << real << ", " << imag << ")" << endl;
}

int main() {	//主函数
	Complex c1(5, 4), c2(2, 10), c3;	//定义复数类的对象
	cout << "c1 = "; c1.display();
	cout << "c2 = "; c2.display();
	c3 = c1 - c2;	//使用重载运算符完成复数减法
	cout << "c3 = c1 - c2 = "; c3.display();
	c3 = c1 + c2;	//使用重载运算符完成复数加法
	cout << "c3 = c1 + c2 = "; c3.display();
	return 0;
}
