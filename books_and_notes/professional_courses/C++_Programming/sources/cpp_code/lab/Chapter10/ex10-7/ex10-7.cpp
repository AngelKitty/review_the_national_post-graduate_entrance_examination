#include <iostream>
#include <stack>
#include <queue>
using namespace std;


int main()
{
	int a[] = {5, 1, 4, 6};
	cout << "存放整型元素的栈的操作：" << endl;
	stack<int> iStack;
	for (int i = 0; i < 4; i ++)
		iStack.push(a[i]);
	if (!iStack.empty()) {
		cout << "第一次pop操作，取出的元素是：" << iStack.top() << endl;
		iStack.pop();
	}
	if (!iStack.empty()) {
		cout << "第二次pop操作，取出的元素是：" << iStack.top() << endl;
		iStack.pop();
	}
	iStack.push(2);
	iStack.push(3);
	if (!iStack.empty()) {
		cout << "第三次pop操作，取出的元素是：" << iStack.top() << endl;
		iStack.pop();
	}
	if (!iStack.empty()) {
		cout << "第四次pop操作，取出的元素是：" << iStack.top() << endl;
		iStack.pop();
	}

	cout << "存放整型元素的队列的操作：" << endl;
	queue<int> iQueue;
	for (int i = 0; i < 4; i ++)
		iQueue.push(a[i]);
	if (!iQueue.empty()) {
		cout << "第一次pop操作，取出的元素是：" << iQueue.front() << endl;
		iQueue.pop();
	}
	if (!iQueue.empty()) {
		cout << "第二次pop操作，取出的元素是：" << iQueue.front() << endl;
		iQueue.pop();
	}
	iQueue.push(2);
	iQueue.push(3);
	if (!iQueue.empty()) {
		cout << "第三次pop操作，取出的元素是：" << iQueue.front() << endl;
		iQueue.pop();
	}
	if (!iQueue.empty()) {
		cout << "第四次pop操作，取出的元素是：" << iQueue.front() << endl;
		iQueue.pop();
	}

	cout << "存放整型元素的优先级队列的操作：" << endl;
	priority_queue<int> iPriQueue;
	for (int i = 0; i < 4; i ++)
		iPriQueue.push(a[i]);
	if (!iPriQueue.empty()) {
		cout << "第一次pop操作，取出的元素是：" << iPriQueue.top() << endl;
		iPriQueue.pop();
	}
	if (!iPriQueue.empty()) {
		cout << "第二次pop操作，取出的元素是：" << iPriQueue.top() << endl;
		iPriQueue.pop();
	}
	iPriQueue.push(2);
	iPriQueue.push(3);
	if (!iPriQueue.empty()) {
		cout << "第三次pop操作，取出的元素是：" << iPriQueue.top() << endl;
		iPriQueue.pop();
	}
	if (!iPriQueue.empty()) {
		cout << "第四次pop操作，取出的元素是：" << iPriQueue.top() << endl;
		iPriQueue.pop();
	}

	return 0;
}
