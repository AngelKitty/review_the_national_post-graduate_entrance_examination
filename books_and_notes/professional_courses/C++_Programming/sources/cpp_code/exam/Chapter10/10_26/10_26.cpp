//10_26.cpp
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
using namespace std;

//将来自输入迭代器p的n个数值排序，将结果通过输出迭代器result输出
template <class InputIterator, class OutputIterator>
void mySort(InputIterator first, InputIterator last, OutputIterator result) {
	//通过输入迭代器p将输入数据存入向量容器s中
	vector<typename iterator_traits<InputIterator>::value_type> s(first, last);
	//对s进行排序，sort函数的参数必须是随机访问迭代器
	sort(s.begin(), s.end());
	//将s序列通过输出迭代器输出
	copy(s.begin(), s.end(), result);
}

int main() {
	//将s数组的内容排序后输出
	double a[5] = { 1.2, 2.4, 0.8, 3.3, 3.2 };
	mySort(a, a + 5, ostream_iterator<double>(cout, " "));
	cout << endl;
	//从标准输入读入若干个整数，将排序后的结果输出
	mySort(istream_iterator<int>(cin), istream_iterator<int>(), ostream_iterator<int>(cout, " "));
	cout << endl;
	return 0;
}
