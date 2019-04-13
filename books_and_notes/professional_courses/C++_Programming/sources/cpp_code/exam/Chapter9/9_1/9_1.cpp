//9_1.cpp
#include <iostream>
using namespace std;

template <class T>	//定义函数模板
void outputArray(const T *array, int count) {
	for (int i = 0; i < count; i++)
		cout << array[i] << " ";
	cout << endl;
}	

int main() {	 //主函数
	const int A_COUNT = 8, B_COUNT = 8, C_COUNT = 20;
	int a [A_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8 };	//定义int数组
	double b[B_COUNT] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8 }; //定义double数组
	char c[C_COUNT] = "Welcome to see you!";	//定义char数组

	cout << " a array contains:" << endl;
	outputArray(a, A_COUNT);	//调用函数模板
	cout << " b array contains:" << endl;
	outputArray(b, B_COUNT);	//调用函数模板
	cout << " c array contains:" << endl;
	outputArray(c, C_COUNT);	//调用函数模板

	return 0;
}
