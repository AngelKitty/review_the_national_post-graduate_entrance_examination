#include <iostream>
#include "link.h"  //参见"实验9"部分
using namespace std;


template <class T>
class Link : public LinkedList<T>
{
public:
	void insertOrder(const T& item);
};

template <class T>
void Link<T>::insertOrder(const T& item)
{
    Link<T>::reset();
    while (!Link<T>::endOfList()) {
		if (item < Link<T>::data())
			break;
		Link<T>::next();
    }   
    insertAt(item);
}

int main()
{
	Link<int> A, B;
    int i, item;
	
    cout << "请输入加入链表A的五个整数：";
	for (i = 0; i < 5; i ++) {
		cin >> item;
		A.insertOrder(item);
	}
	cout << "请输入加入链表B的五个整数：";
	for (i = 0; i < 5; i ++) {
		cin >> item;
		B.insertOrder(item);
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
		A.insertOrder(B.data());
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
