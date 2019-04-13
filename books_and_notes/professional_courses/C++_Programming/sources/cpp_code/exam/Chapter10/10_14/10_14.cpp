//10_14.cpp
#include <iostream>
#include <numeric>	//包含数值算法头文件
using namespace std;

class MultClass	{  //定义MultClass类
public:
	int operator() (int x, int y) const { return x * y; }	//重载操作符operator()
};

int main() {
	int a[] = { 1, 2, 3, 4, 5 };
	const int N = sizeof(a) / sizeof(int);
	cout << "The result by multipling all elements in a is "
		<< accumulate(a, a + N, 1, MultClass())	//将类multclass传递给通用算法
		<< endl;
	return 0;
}
