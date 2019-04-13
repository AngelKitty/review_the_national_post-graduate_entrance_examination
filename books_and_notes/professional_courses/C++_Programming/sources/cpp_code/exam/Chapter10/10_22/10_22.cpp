//10_22.cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
using namespace std;

int main() {
	int iarray[] = { 26, 17, 15, 22, 23, 33, 32, 40 };
	vector<int> ivector(iarray, iarray + sizeof(iarray) / sizeof(int));
	
	// 查找并输出第一个最大值元素及其位置
	vector<int>::iterator p = max_element(ivector.begin(), ivector.end());
	int n = p  - ivector.begin();
	cout << "max element: " << *p << " found at " << n << endl;

	//局部排序并复制到别处
	vector<int> ivector1(5);
	partial_sort_copy(ivector.begin(), ivector.end(), ivector1.begin(), ivector1.end());
	copy(ivector1.begin(), ivector1.end(), ostream_iterator<int>(cout, " "));
	cout << endl; 

	//排序，缺省为递增。
	sort(ivector.begin(), ivector.end());
	copy(ivector.begin(), ivector.end(), ostream_iterator<int>(cout, " "));
	cout << endl; 

	// 返回小于等于24和大于等于24的元素的位置
	cout << *lower_bound(ivector.begin(), ivector.end(), 24) << endl;
	cout << *upper_bound(ivector.begin(), ivector.end(), 24) << endl;

	//对于有序区间，可以用二分查找方法寻找某个元素
    cout << binary_search(ivector.begin(), ivector.end(), 33) << endl;

	//合并两个序列ivector和ivector1，并将结果放到ivector2中
	vector<int> ivector2(13);
	merge(ivector.begin(), ivector.end(), ivector1.begin(), ivector1.end(), ivector2.begin());
	copy(ivector2.begin(), ivector2.end(), ostream_iterator<int>(cout, " "));
	cout << endl; 

	//将小于*(ivector.begin()+5)的元素放置在该元素之左
	//其余置于该元素之右。不保证维持原有的相对位置
	nth_element(ivector2.begin(), ivector2.begin() + 5, ivector2.end());
	copy(ivector2.begin(), ivector2.end(), ostream_iterator<int>(cout, " "));
	cout << endl; 

	//排序，并保持原来相对位置
	stable_sort(ivector2.begin(), ivector2.end());
	copy(ivector2.begin(), ivector2.end(), ostream_iterator<int>(cout, " "));
	cout << endl; 

	//合并两个有序序列，然后就地替换
	int iarray3[] = { 1, 3, 5, 7, 2, 4, 6, 8 };
	vector<int> ivector3(iarray3, iarray3 + sizeof(iarray3) / sizeof(int));
	inplace_merge(ivector3.begin(), ivector3.begin() + 4, ivector3.end());
	copy(ivector3.begin(), ivector3.end(), ostream_iterator<int>(cout, " "));  
	cout<<endl; 
	
	//以字典顺序比较序列ivector3和ivector4
	int iarray4[] = { 1, 3, 5, 7, 1, 5, 9, 3 };
	vector<int> ivector4(iarray4, iarray4 + sizeof(iarray4) / sizeof(int));
	cout<< lexicographical_compare(ivector3.begin(), ivector3.end(), ivector4.begin(), ivector4.end()) << endl;
	return 0;
}
