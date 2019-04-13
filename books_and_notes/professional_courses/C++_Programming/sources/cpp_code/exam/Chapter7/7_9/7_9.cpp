//7_9.cpp  文件五，主函数
#include "LinearEqu.h"	//类定义头文件
#include <iostream>
using namespace std;

int main() {	//主函数
	double a[]=	{ //方程系数矩阵
		0.2368, 0.2471, 0.2568, 1.2671,		//第一行
		0.1968, 0.2071, 1.2168, 0.2271,		//第二行
		0.1581, 1.1675, 0.1768, 0.1871,		//第三行
		1.1161, 0.1254, 0.1397, 0.1490 };	//第四行
	double b[] = { 1.8471, 1.7471, 1.6471, 1.5471 };//方程右端项
	LinearEqu equ(4);	//定义一个四元方程组对象
	equ.setLinearEqu(a,b);	//设置方程组
	equ.printLinearEqu();	//输出方程组
	if (equ.solve())	//求解方程组
		equ.printSolution();	//输出方程组的解
	else
		cout<<"Fail"<<endl;
	return 0;
}
