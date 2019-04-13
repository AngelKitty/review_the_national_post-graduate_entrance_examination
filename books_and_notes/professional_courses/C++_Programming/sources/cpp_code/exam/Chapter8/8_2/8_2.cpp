//8_2.cpp
#include <iostream>
using namespace std;
class Clock	{	//时钟类声明定义
public:	//外部接口
	Clock(int hour = 0, int minute = 0, int second = 0);
	void showTime() const;
	Clock& operator ++ ();	//前置单目运算符重载
	Clock operator ++ (int);	//后置单目运算符重载
private:	//私有数据成员
	int hour, minute, second;
};

Clock::Clock(int hour/* = 0 */, int minute/* = 0 */, int second/* = 0 */) {	//构造函数
	if (0 <= hour && hour < 24 && 0 <= minute && minute < 60 && 0 <= second && second < 60) {
		this->hour = hour;
		this->minute = minute;
		this->second = second;
	} else
		cout << "Time error!" << endl;
}

void Clock::showTime() const {	//显示时间函数
	cout << hour << ":" << minute << ":" << second << endl;
}

Clock & Clock::operator ++ () {	//前置单目运算符重载函数
	second++;
	if (second >= 60) {
		second -= 60;
		minute++;
		if (minute >= 60) {
			minute -= 60;
			hour = (hour + 1) % 24;
		}
	}
	return *this;
}

Clock Clock::operator ++ (int) {	//后置单目运算符重载
	//注意形参表中的整型参数
	Clock old = *this;
	++(*this);	//调用前置“++”运算符
	return old;
}

int main() {
	Clock myClock(23, 59, 59);
	cout << "First time output: ";
	myClock.showTime();
	cout << "Show myClock++:    ";
	(myClock++).showTime();
	cout << "Show ++myClock:    ";
	(++myClock).showTime();
	return 0;
}
