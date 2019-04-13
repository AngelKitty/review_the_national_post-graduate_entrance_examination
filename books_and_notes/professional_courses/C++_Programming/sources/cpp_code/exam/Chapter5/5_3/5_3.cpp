//5_3.cpp
#include<iostream>
using namespace std;

class Clock {	//时钟类定义
public:	//外部接口
	Clock();
	void setTime(int newH, int newM, int newS);   //三个形参均具有函数原型作用域
	void showTime();
private:	//私有数据成员
	int hour, minute, second;
};

//时钟类成员函数实现
Clock::Clock() : hour(0), minute(0), second(0) { }	//构造函数

void Clock::setTime(int newH, int newM, int newS) {//三个形参均具有局部作用域
	hour = newH;
	minute = newM;
	second = newS;
}

void Clock::showTime() {
	cout << hour << ":" << minute << ":" << second << endl;
}

Clock globClock;  //声明对象globClock，具有静态生存期，命名空间作用域
//由缺省构造函数初始化为0:0:0
int main() {	//主函数
	cout << "First time output:" << endl;	
	//引用具有命名空间作用域的对象globClock：
	globClock.showTime();	//对象的成员函数具有类作用域
	//显示0:0:0
	globClock.setTime(8,30,30);	//将时间设置为8:30:30

	Clock myClock(globClock);	//声明具有局部作用域的对象myClock
	//调用拷贝构造函数，以globClock为初始值
	cout<<"Second time output:"<<endl;	
	myClock.showTime();	//引用具有局部作用域的对象myClock
	//输出8:30:30

	return 0;
}
