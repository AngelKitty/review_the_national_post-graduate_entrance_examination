//10_4.cpp
#include <iostream>
#include <list>
#include <deque>
#include <iterator>
#include <algorithm>
using namespace std;

//输出指定的整型顺序容器的元素
template <class T>
void printContainer(const char* msg, const T& s) {
	cout << msg << ": ";
	copy(s.begin(), s.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

int main() {
	//从标准输入读入10个整数，将它们分别从s的头部加入
	deque<int> s;
	for (int i = 0; i < 10; i++) {
		int x;
		cin >> x;
		s.push_front(x);
	}
	printContainer("deque at first", s);

	//用s容器的内容的逆序构造列表容器l
	list<int> l(s.rbegin(), s.rend());
	printContainer("list at first", l);

	//将列表容器l的每相邻两个容器顺序颠倒
	list<int>::iterator iter = l.begin();
	while (iter != l.end()) {
		int v = *iter;			//得到一个元素
		iter = l.erase(iter);	//将该元素删除，得到指向下一个元素的迭代器
		l.insert(++iter, v);	//将刚刚删除的元素插入下一个位置
	}
	printContainer("list at last", l);

	//用列表容器l的内容给s赋值，将s输出
	s.assign(l.begin(), l.end());
	printContainer("deque at last", s);

	return 0;
}
