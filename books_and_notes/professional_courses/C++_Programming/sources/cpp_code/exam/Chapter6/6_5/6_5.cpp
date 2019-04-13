//6_5.cpp
#include <iostream>
using namespace std;
int main() {
	int i;			//定义int型数i
	int *ptr = &i;	//取i的地址赋给ptr
	i = 10;			//int型数赋初值
	cout << "i = " << i << endl;			//输出int型数的值
	cout << "*ptr = " << *ptr << endl;	//输出int型指针所指地址的内容
	return 0;
}
