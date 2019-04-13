//8_7.cpp  文件三，主函数
#include "Trapzint.h"	//类定义头文件
#include <iostream>
#include <iomanip>
using namespace std;

int main() {	//主函数
	MyFunction f;		//定义MyFunction类的对象
	Trapz trapz(f);	//定义Trapz类的对象
	//计算并输出积分结果
	cout << "TRAPZ Int: " << setprecision(7) << trapz(0, 2, 1e-7) << endl;	
	return 0;
}
