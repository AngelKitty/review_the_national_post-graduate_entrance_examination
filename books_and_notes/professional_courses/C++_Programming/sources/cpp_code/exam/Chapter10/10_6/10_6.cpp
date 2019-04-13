//10_6.cpp
#include <list>
#include <iterator>
#include <string>
#include <iostream>
using namespace std;

int main() {
	string names1[] = { "Alice", "Helen", "Lucy", "Susan" };
	string names2[] = { "Bob", "David", "Levin", "Mike" };
	list<string> s1(names1, names1 + 4); //用names1数组的内容构造列表s1
	list<string> s2(names2, names2 + 4); //用names2数组的内容构造列表s2

	//将s1的第一个元素放到s2的最后
	s2.splice(s2.end(), s1, s1.begin());
	list<string>::iterator iter1 = s1.begin(); //iter1指向s1首
	advance(iter1, 2); //iter1前进2个元素，它将指向s1第3个元素
	list<string>::iterator iter2 = s2.begin();  //iter2指向s2首
	++iter2; //iter2前进1个元素，它将指向s2第2个元素
	list<string>::iterator iter3 = iter2; //用iter2初始化iter3
	advance(iter3, 2); //iter3前进2个元素，它将指向s2第4个元素
	//将[iter2, iter3)范围内的结点接到s1中iter1指向的结点前
	s1.splice(iter1, s2, iter2, iter3); 

	//分别将s1和s2输出
	copy(s1.begin(), s1.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	copy(s2.begin(), s2.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	return 0;
}
