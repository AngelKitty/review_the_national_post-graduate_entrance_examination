#include <iostream>
#include <list>
using namespace std;

template<class T>
void exchange(list<T>& l1, typename list<T>::iterator p1, list<T>& l2, typename list<T>::iterator p2) {
	list<T> temp;
	temp.splice(temp.begin(), l1, p1, l1.end());
	l1.splice(l1.end(), l2, p2, l2.end());
	l2.splice(l2.end(), temp, temp.begin(), temp.end());
}

typedef list<int> INTLIST;

int main() {
	INTLIST l1, l2;
	for (int i = 0; i < 4; i++)
		l1.push_back(i);
	for (int i = 6; i < 12; i++)
		l2.push_back(i);
	INTLIST::iterator li;
	cout << "交换前" << endl;
	cout << "l1: ";
	for (li = l1.begin(); li != l1.end(); li++)
		cout << *li << ' ';
	cout << endl;
	cout << "l2: ";
	for (li = l2.begin(); li != l2.end(); li++)
		cout << *li << ' ';
	cout << endl;
	INTLIST::iterator p1, p2;
	p1 = l1.begin();
	p2 = l2.begin();
	exchange(l1, ++(++p1), l2, ++p2);
	cout << "交换后" << endl;
	cout << "l1: ";
	for (li = l1.begin(); li != l1.end(); li++)
		cout << *li << ' ';
	cout << endl;
	cout << "l2: ";
	for (li = l2.begin(); li != l2.end(); li++)
		cout << *li << ' ';
	cout << endl;

	return 0;
}
