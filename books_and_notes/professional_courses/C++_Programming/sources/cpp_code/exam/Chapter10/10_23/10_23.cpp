//10_23.cpp
#include <iostream>
#include <numeric>
#include <functional>
#include <iterator>
#include <vector>
using namespace std;

int main() {
	int iarray[] = { 1, 2, 3, 4, 5 };
	vector<int> ivector(iarray, iarray + sizeof(iarray) / sizeof(int));

	//元素的累计
	cout << accumulate(ivector.begin(), ivector.end(), 0) << endl;
	//向量的内积
	cout << inner_product(ivector.begin(), ivector.end(), ivector.begin(), 10) << endl;
	//向量容器中元素局部求和
	partial_sum(ivector.begin(), ivector.end(), ostream_iterator<int>(cout," "));
	cout << endl;
	//向量容器中相邻元素的差值
	adjacent_difference(ivector.begin(), ivector.end(), ostream_iterator<int>(cout," "));
	cout << endl;
	return 0;
}
