#include <iostream>
#include "node.h"
using namespace std;

int main()
{
	int a[10];
	Node<int> n[10];
	cout << "输入10个整数：" << endl;
	for (int i = 0; i < 10; i ++) {
		cin >> a[i];
	}
	for (int i = 0; i < 9; i ++) {
		n[i].data = a[i];
		n[i].insertAfter(&n[i+1]);
	}
	n[9].data = a[9];
	Node<int>* np = &n[0];
	while (np != NULL) {
		cout << np->data << ' ';
		np = np->nextNode();
	}
	cout << endl;
	int f;
	cout << "请输入要查找的数：";
	cin >> f;
	Node<int> p(0, &n[0]);
	np = &p;
	while (np->nextNode() != NULL) {
		while (np->nextNode()->data == f)
			np->deleteAfter();
		np = np->nextNode();
	}
	cout << "删除后的链表：" << endl;
	np = &n[0];
	while (np != NULL) {
		cout << np->data << ' ';
		np = np->nextNode();
	}
	np = &p;
	while (np->nextNode() != NULL)
		np->deleteAfter();
	cout << endl;
	return 0;
}
