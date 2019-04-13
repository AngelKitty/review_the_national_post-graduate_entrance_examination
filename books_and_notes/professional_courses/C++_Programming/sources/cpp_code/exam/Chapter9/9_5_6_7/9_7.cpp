//9_7.cpp
#include <iostream>
#include "LinkedList.h"
using namespace std;

int main() {
	LinkedList<int> list;

	//输入10个整数依次向表头插入
	for (int i = 0; i < 10; i++) {
		int item;
		cin >> item;
		list.insertFront(item);
	}

	//输出链表
	cout << "List: ";
	list.reset();
	//输出各结点数据，直到链表尾
	while (!list.endOfList()) {
		cout << list.data() << "  ";
		list.next();	//使游标指向下一个结点
	}
	cout << endl;

	//输入需要删除的整数
	int key;
	cout << "Please enter some integer needed to be deleted: ";
	cin >> key;

	//查找并删除结点
	list.reset();
	while (!list.endOfList()) {
		if (list.data() == key) 
			list.deleteCurrent();
		list.next();
	}

	//输出链表
	cout << "List: ";
	list.reset();
	//输出各结点数据，直到链表尾
	while (!list.endOfList()) {
		cout << list.data() << "  ";
		list.next();  //使游标指向下一个结点
	}
	cout << endl;

	return 0;
}
