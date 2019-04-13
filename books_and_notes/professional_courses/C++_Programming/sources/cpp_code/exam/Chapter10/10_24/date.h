//date.h
#ifndef __DATE_H__
#define __DATE_H__

class Date {	//日期类
private:
	int year;		//年
	int month;		//月
	int day;		//日
	int totalDays;	//该日期是从公元元年1月1日开始的第几天

public:
	Date(int year = 1, int month = 1, int day = 1);	//用年、月、日构造日期
	static Date read();
	int getYear() const { return year; }
	int getMonth() const { return month; }
	int getDay() const { return day; }
	int getMaxDay() const;		//获得当月有多少天
	bool isLeapYear() const {	//判断当年是否为闰年
		return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
	}
	void show() const;			//输出当前日期
	//计算两个日期之间差多少天	
	int operator - (const Date& date) const {
		return totalDays - date.totalDays;
	}
	//判断两个日期的前后顺序
	bool operator < (const Date& date) const {
		return totalDays < date.totalDays;
	}
};

#endif //__DATE_H__
