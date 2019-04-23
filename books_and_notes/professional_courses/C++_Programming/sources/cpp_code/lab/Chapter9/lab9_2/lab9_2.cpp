#include <iostream>
#include "link.h"
using namespace std;

int main()
{
	LinkedList<int> A, B;
    int i, item;
	
    cout << "请输入加入链表A的五个整数：";
	for (i = 0; i < 5; i ++) {
		cin >> item;
		A.insertRear(item);
	}
	cout << "请输入加入链表B的五个整数：";
	for (i = 0; i < 5; i ++) {
		cin >> item;
		B.insertRear(item);
	}
	cout << endl << "有序链表A中的元素为：";
	A.reset();
	while(!A.endOfList()) {
		cout << A.data() << "  ";
		A.next();
	}
	cout << endl << "有序链表B中的元素为：";
	B.reset();
	while(!B.endOfList()) {
		A.insertRear(B.data());
		cout << B.data() << "  ";
		B.next();
	}
	
	cout << endl << "加入链表B中元素后，链表A中的元素为：";
	A.reset();
	while(!A.endOfList()) {
		cout << A.data() << "  ";
		A.next();    
	}
	cout << endl;
	return 0;
}
