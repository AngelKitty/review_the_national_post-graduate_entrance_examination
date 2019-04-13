//10_5.cpp
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

int main() {
	istream_iterator<int> i1(cin), i2;	//建立一对儿输入流迭代器
	vector<int> s1(i1, i2);	//通过输入流迭代器从标准输入流中输入数据
	sort(s1.begin(), s1.end());			//将输入的整数排序
	deque<int> s2;
	//以下循环遍历s1
	for (vector<int>::iterator iter = s1.begin(); iter != s1.end(); ++iter) {
		if (*iter % 2 == 0)	//偶数放到s2尾部
			s2.push_back(*iter);
		else				//奇数放到s2首部
			s2.push_front(*iter);
	}
	//将s2的结果输出
	copy(s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
	return 0;
}
