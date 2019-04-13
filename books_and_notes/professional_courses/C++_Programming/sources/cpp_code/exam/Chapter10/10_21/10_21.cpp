//10_21.cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
using namespace std;

class evenByTwo {
private:
	int x;
public:
	evenByTwo() : x(0) { }
	int operator () () { return x += 2; }
};

int main() {
	int iarray1[]= { 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6, 7, 8 };
	int iarray2[] = { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
	vector<int> ivector1(iarray1, iarray1 + sizeof(iarray1) / sizeof(int));
	vector<int> ivector2(iarray2, iarray2 + sizeof(iarray2) / sizeof(int));
	vector<int> ivector3(2); 
	ostream_iterator<int> output(cout, " ");  //定义流迭代器用于输出数据
	
	//迭代遍历ivector3区间，每个元素填上-1
	fill(ivector3.begin(), ivector3.end(), -1);
	copy(ivector3.begin(), ivector3.end(), output);	// 使用copy进行输出
	cout << endl;

	//迭代遍历ivector3区间，对每一个元素进行evenByTwo操作
	generate(ivector3.begin(), ivector3.end(), evenByTwo());
	copy(ivector3.begin(), ivector3.end(), output);
	cout << endl; 

	//将删除元素6后的ivector2序列置于另一个容器ivector4之中
	vector<int> ivector4;
	remove_copy(ivector2.begin(), ivector2.end(), back_inserter(ivector4), 6);
	copy(ivector4.begin(), ivector4.end(), output);
	cout << endl; 

	//删除小于6的元素
	ivector2.erase(remove_if(ivector2.begin(), ivector2.end(), bind2nd(less<int>(), 6)), ivector2.end());
	copy(ivector2.begin(), ivector2.end(), output);
	cout << endl; 
	
	//将所有的元素值6，改为元素值3
	replace(ivector2.begin(), ivector2.end(), 6, 3);
	copy(ivector2.begin(), ivector2.end(), output);
	cout << endl; 
	
	//逆向重排每一个元素
	reverse(ivector2.begin(), ivector2.end());
	copy(ivector2.begin(), ivector2.end(), output);
	cout << endl; 
	
	//旋转（互换元素）[first, middle),  和[middle, end)，结果直接输出
	rotate_copy(ivector2.begin(), ivector2.begin() + 3, ivector2.end(), output);
	cout << endl;
	return 0;
}
