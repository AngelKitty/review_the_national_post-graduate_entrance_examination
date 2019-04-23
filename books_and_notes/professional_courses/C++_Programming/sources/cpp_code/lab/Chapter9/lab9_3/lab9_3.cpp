#include <iostream>
#include "queue.h"
using namespace std;

int main()
{
	int item;
	Queue<int> q;
	cout << "输入5个要插入的元素" << endl;
	for (int i = 0; i < 5; i ++) {
		cin >> item;
		q.push(item);
	}
	for (int i = 0; i < 5; i ++)
		cout << "取出元素" << q.pop() << endl;
	return 0;
}