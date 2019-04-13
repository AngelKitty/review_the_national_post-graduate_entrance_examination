//Stack.h
#ifndef STACK_H
#define STACK_H
#include <cassert> 

//模板的定义，SIZE为栈的大小
template <class T, int SIZE = 50>
class Stack {
private:
	T list[SIZE];	//数组，用于存放栈的元素
	int top;	//栈顶位置（数组下标）
public:
	Stack();	//构造函数，初始化栈
	void push(const T &item);	//将元素item压入栈
	T pop();	//将栈顶元素弹出栈
	void clear();	//将栈清空
	const T &peek() const;	//访问栈顶元素
	bool isEmpty() const;	//测试是否栈满
	bool isFull() const;	//测试是否栈空
};

//模板的实现
template <class T, int SIZE>
Stack<T, SIZE>::Stack() : top(-1) { }	//构造函数，栈顶初始化为-1

template <class T, int SIZE>
void Stack<T, SIZE>::push(const T &item) {	//将元素item压入栈
	assert(!isFull());	//如果栈满了，则报错
	list[++top] = item;	//将新元素压入栈顶
}

template <class T, int SIZE>
T Stack<T, SIZE>::pop() {	//将栈顶元素弹出栈
	assert(!isEmpty());	//如果栈为空，则报错
	return list[top--];	//返回栈顶元素，并将其弹出栈顶
}

template <class T, int SIZE>
const T &Stack<T, SIZE>::peek() const {	//访问栈顶元素
	assert(!isEmpty());	//如果栈为空，则报错
	return list[top];	//返回栈顶元素
}

template <class T, int SIZE>
bool Stack<T, SIZE>::isEmpty() const {	//测试栈是否空
	return top == -1;
}

template <class T, int SIZE>
bool Stack<T, SIZE>::isFull() const {	//测试是否栈满
	return top == SIZE - 1;
}

template <class T, int SIZE>
void Stack<T, SIZE>::clear() {	//清空栈
	top = -1;
}

#endif	//STACK_H
