//10_20.cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

int main() {
	int iarray[] = { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
	vector<int> ivector(iarray, iarray + sizeof(iarray) / sizeof(int));
	int iarray1[] = { 6, 6 };
	vector<int> ivector1(iarray1, iarray1 + sizeof(iarray1) / sizeof(int));
	int iarray2[] = { 5, 6 };
	vector<int> ivector2(iarray2, iarray2 + sizeof(iarray2) / sizeof(int));
	int iarray3[] = { 0, 1, 2, 3, 4, 5, 7, 7, 7, 9, 7 };
	vector<int> ivector3(iarray3, iarray3 + sizeof(iarray3) / sizeof(int));

	//找出ivector之中相邻元素值相等的第一个元素
	cout << *adjacent_find(ivector.begin(), ivector.end()) << endl; 

	//找出ivector之中小于7的元素个数
	cout << count_if(ivector.begin(), ivector.end(), bind2nd(less<int>(), 7)) << endl; 

	//找出ivector之中大于2的第一个元素所在位置的元素
	cout <<  *find_if(ivector.begin(), ivector.end(), bind2nd(greater<int>(), 2)) << endl; 
	
	//子序列ivector2在ivector中出现的起点位置元素
	cout << *search(ivector.begin(), ivector.end(), 
		ivector2.begin(), ivector2.end()) << endl;

	//查找连续出现3个6的起点位置元素
	cout << *search_n(ivector.begin(), ivector.end(), 3, 6, equal_to<int>()) << endl;
	
	//判断两个区间ivector和ivector3相等否(0为假，1为真)
	cout << equal(ivector.begin(), ivector.end(), ivector3.begin()) << endl;

	//查找区间ivector3在ivector中不匹配点的位置
	pair<vector<int>::iterator, vector<int>::iterator> result = 
		mismatch(ivector.begin(), ivector.end(), ivector3.begin());
	cout << result.first - ivector.begin() << endl; 
	return 0;
}
