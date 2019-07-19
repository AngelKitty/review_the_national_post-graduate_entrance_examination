/*******************************************************
*程序:Enumerated.cpp                                                                                     *
*用途:IDA枚举类型分析                                                                          *
*看雪软件安全网站                                                                                   *
*     www.pediy.com, kanxue  2002.8                                                                *
********************************************************/
#include <stdio.h> 
int main(void)
{
	enum weekday { MONDAY, TUESDAY, WEDNESDAY, THUSDAY, FRIDAY, SATURDAY, SUNDAY }; 

	printf("%d,%d,%d,%d,%d,%d,%d",MONDAY,TUESDAY, WEDNESDAY, THUSDAY, FRIDAY, SATURDAY, SUNDAY );

	return 0;
}
