//6_8.cpp
#include <iostream>
using namespace std;

int main() {
	int line1[] = { 1, 0, 0 };	//定义数组，矩阵的第一行
	int line2[] = { 0, 1, 0 };	//定义数组，矩阵的第二行
	int line3[] = { 0, 0, 1 };	//定义数组，矩阵的第三行
	
	//定义整型指针数组并初始化
	int *pLine[3] = { line1, line2, line3 };	
	
	cout << "Matrix test:" << endl;	//输出单位矩阵
	for (int i = 0; i < 3; i++) {	//对指针数组元素循环
		for (int j = 0; j < 3; j++) 	//对矩阵每一行循环
			cout << pLine[i][j] << " ";
		cout << endl;
	}
	return 0;
}
