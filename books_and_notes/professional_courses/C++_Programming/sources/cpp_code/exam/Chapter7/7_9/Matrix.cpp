//Matrix.cpp  文件三，Matrix类实现
#include "Matrix.h"	//包含类的定义头文件
#include <iostream>
using namespace std;

void Matrix::setMatrix(const double *values) {	//设置矩阵
	for (int i = 0; i < size * size; i++)
		elements[i] = values[i];	//矩阵成员赋初值
}

Matrix::Matrix(int size/* = 2 */) : size(size) {	//矩阵Matrix类的构造函数
	elements = new double[size * size];	//动态内存分配
}

Matrix::~Matrix() {	//矩阵Matrix类的析构函数
	delete[] elements;	//内存释放
}

void Matrix::printMatrix() const { //显示矩阵的元素
	cout << "The Matrix is:" << endl;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++)
			cout << element(i, j) << " ";
		cout << endl;
	}
}
