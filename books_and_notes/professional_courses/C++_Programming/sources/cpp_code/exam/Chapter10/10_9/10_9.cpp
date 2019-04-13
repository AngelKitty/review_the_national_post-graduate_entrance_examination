//10_9.cpp
#include <set>
#include <iterator>
#include <utility>
#include <iostream>
using namespace std;

int main() {
	set<double> s;
	while (true) {
		double v;
		cin >> v;
		if (v == 0) break;	//输入0表示结束
		pair<set<double>::iterator, bool> r = s.insert(v);	//尝试将v插入
		if (!r.second)		//如果v已存在，输出提示信息
			cout << v << " is duplicated" << endl;
	}
	set<double>::iterator iter1 = s.begin();	//得到第一个元素的迭代器
	set<double>::iterator iter2 = s.end();		//得到末尾的迭代器
	double medium = (*iter1 + *(--iter2)) / 2;	//得到最小和最大元素的中值
	//输出小于或等于中值的元素
	cout << "<= medium: ";
	copy(s.begin(), s.upper_bound(medium), ostream_iterator<double>(cout, " "));
	cout << endl;
	//输出大于或等于中值的元素
	cout << ">= medium: ";
	copy(s.lower_bound(medium), s.end(), ostream_iterator<double>(cout, " "));
	cout << endl;
	return 0;
}
