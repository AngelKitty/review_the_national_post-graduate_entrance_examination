//LinearEqu.cpp  文件四，LinearEqu类实现
#include "LinearEqu.h"	//包含类的定义头文件
#include <iostream>
#include <cmath>
using namespace std;

LinearEqu::LinearEqu(int size/* = 2 */) : Matrix(size) {	//用size调用基类构造函数
	sums = new double[size];	//动态内存分配
	solution = new double[size];
}

LinearEqu::~LinearEqu()	{ //派生类LinearEqu的析构函数
	delete[] sums;	//释放内存
	delete[] solution;
	//会自动调用基类析构函数
}

void LinearEqu::setLinearEqu(const double *a, const double *b) {	//设置线性方程组
	setMatrix(a);	//调用基类函数
	for(int i = 0; i < getSize(); i++)
		sums[i] = b[i];
}

void LinearEqu::printLinearEqu() const {	//显示线性方程组
	cout << "The Line eqution is:" << endl;
	for (int i = 0; i < getSize(); i++) {
		for (int j = 0; j < getSize(); j++)
			cout << element(i, j) << " ";
			cout << "     " << sums[i] << endl;
		}
}

void LinearEqu::printSolution() const {	//输出方程的解
	cout << "The Result is: " << endl;
	for (int i = 0; i < getSize(); i++)
		cout << "x[" << i << "] = " << solution[i] << endl;
}

inline void swap(double &v1, double &v2) {	//交换两个实数
	double temp = v1;
	v1 = v2;
	v2 = temp;
}

bool LinearEqu::solve() {	//全选主元素高斯消去法求解方程
	int *js=new int[getSize()];	//存储主元素所在列号的数组
	for (int k = 0; k < getSize() - 1; k++)	{ 
		//选主元素
		int is;	//主元素所在行号
		double max = 0;	//所有元素的最大值
		for (int i = k; i < getSize(); i++)
			for (int j = k; j < getSize(); j++) {
				double t = fabs(element(i, j));
				if (t > max) {
					max = t;
					js[k] = j;
					is = i;
				}
			}
		if (max == 0) {
			delete[] js;
			return false;
		} else {
			//通过行、列交换，把主元素交换到第k行第k列
			if (js[k] != k)
				for (int i = 0; i < getSize(); i++)
					swap(element(i, k), element(i, js[k]));
			if (is != k) {
				for (int j = k; j < getSize(); j++)
					swap(element(k, j), element(is, j));
				swap(sums[k], sums[is]);
			}
		}
		//消去过程
		double major = element(k, k);
		for (int j = k + 1; j < getSize(); j++)
			element(k, j) /= major;
		sums[k] /= major;
		for (int i = k + 1; i < getSize(); i++) {
			for (int j = k + 1; j < getSize(); j++)
				element(i, j) -= element(i, k) * element(k, j);
			sums[i] -= element(i, k) * sums[k];
		}
	}
	//判断剩下的一个元素是否等于0
	double d = element(getSize() - 1, getSize() - 1);
	if (fabs(d) < 1e-15) {
		delete[] js;
		return false;
	}
	//回代过程
	solution[getSize() - 1] = sums[getSize() - 1] / d;	
	for (int i = getSize() - 2; i >= 0; i--) {
		double t = 0.0;
		for (int j = i + 1; j <= getSize() - 1; j++)
			t += element(i, j) * solution[j];
		solution[i] = sums[i] - t;
	}
	js[getSize() - 1] = getSize() - 1;
	for (int k = getSize() - 1; k >= 0; k--)
		if (js[k] != k) swap(solution[k], solution[js[k]]);
	delete[] js;
	return true;
}
