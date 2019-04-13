//10_15.cpp
#include <iostream>   
#include <numeric>   //包含数值算法头文件
#include <functional>  //包含标准函数对象头文件
using namespace std;	
int main() {
	int a[] = { 1, 2, 3, 4, 5 };
	const int N = sizeof(a) / sizeof(int);
	cout << "The result by multipling all elements in A is "
		<< accumulate(a, a + N, 1, multiplies<int>()) //将标准函数对象传递给通用算法
		<< endl;
	return 0;
}
