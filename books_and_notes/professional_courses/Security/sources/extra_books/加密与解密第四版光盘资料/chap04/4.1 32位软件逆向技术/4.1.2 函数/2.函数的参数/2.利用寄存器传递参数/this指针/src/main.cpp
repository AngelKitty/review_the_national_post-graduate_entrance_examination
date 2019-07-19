/*******************************************************
*  this指针                                                                                                        *
*   (c)  看雪学院 www.kanxue.com 2000-2018                *
********************************************************/
#include <stdio.h>
class CSum
{
public:
	int Add(int a, int b) //实际Add原型具有如下形式:Add(this,int a,int b)
	{
		return (a + b);
	}
};

void main()
{     
	CSum sum;
	sum.Add(1, 2);
} 
