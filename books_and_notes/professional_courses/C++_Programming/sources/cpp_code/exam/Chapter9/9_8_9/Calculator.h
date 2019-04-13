//Calculator.h
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "Stack.h"	// 包含栈类模板定义文件

class Calculator {	//计算器类
private:
	Stack<double> s;	// 操作数栈
	void enter(double num);	//将操作数num压入栈
	//连续将两个操作数弹出栈，放在opnd1和opnd2中
	bool getTwoOperands(double &opnd1, double &opnd2);
	void compute(char op);	//执行由操作符op指定的运算
public:
	void run();		//运行计算器程序
	void clear();	//清空操作数栈
};

#endif //CALCULATOR_H
