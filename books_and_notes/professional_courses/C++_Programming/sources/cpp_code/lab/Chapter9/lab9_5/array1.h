#ifndef ARRAY1_H_
#define ARRAY1_H_

#include <iostream>
using namespace std;

template <class T>
class Array
{
	T* alist;
	int size;
public:
	Array() {size = 0;}
	Array(int sz) {alist = new T[sz]; size = sz;}
	Array(T a[], int sz) {
		size = sz;
		alist = new T[size];
		for (int i = 0; i < size; i ++)
			alist[i] = a[i];
	}
	~Array() {size = 0; delete []alist;}
	int getSize() {return size;}
	T& operator [](int i) {return alist[i];}
	void insertSort();
	void selectSort();
	void bubbleSort();
	int seqSearch(T key) {
		int i;
		for (i = 0; i < size; i ++)
			if (alist[i] == key)
				return i;
		if (i == size) {
			cout << "没有找到元素" << endl;
			return -1;
		}
	}
};

template <class T>
void Array<T>::insertSort()
{
	int i, j;
	T temp;
	//将下标为1～size-1的元素逐个插入到已排序序列中适当的位置
	for (i = 1; i < size; i ++) 
	{
		//从a[i-1]开始向a[0]方向扫描各元素,寻找适当位置插入a[i]
		j = i;
		temp = alist[i];
		while (j > 0 && temp < alist[j-1]) 
		{   //逐个比较，直到temp>=a[j-1]时，j便是应插入的位置。
			alist[j] = alist[j-1];  //将元素逐个后移，以便找到插入位置时可立即插入。
			j--;
		}
		//插入位置已找到，立即插入。
		alist[j] = temp;
	}
	//输出数据
	for(i = 0; i < size; i ++)
		cout << alist[i] << "  ";
	cout << endl;
}

template <class T>
void Array<T>::selectSort()
{
	int minIndex; //每一次选出的最小元素之下标
	int i, j;
	T temp;
	//sort a[0]..a[size-2], and a[size-1] is in place
	for (i = 0; i < size - 1; i ++) 
	{
		minIndex = i;    //最小元素之下标初值设为i
		//在元素a[i+1]..a[size-1]中逐个比较显出最小值
		for (j = i + 1; j < size; j++) 
			//minIndex始终记录当前找到的最小值的下标
			if (alist[j] < alist[minIndex])
				minIndex = j;
		//将这一趟找到的最小元素与a[i]交换
		temp = alist[i];
		alist[i] = alist[minIndex];
		alist[minIndex] = temp;
	}
	//输出数据
	for(i = 0; i < size; i ++)
		cout << alist[i] << "  ";
	cout << endl;
}

template <class T>
void Array<T>::bubbleSort()
{
	T temp;
	int i, j;
	for (i = 1; i < size; i ++) {
		for (j = size - 1; j >= i; j --) {
			if (alist[j - 1] > alist[j]) {
				temp = alist[j];
				alist[j] = alist[j - 1];
				alist[j - 1] = temp;
			}
		}
	}
	for(i = 0; i < size; i ++)
		cout << alist[i] << "  ";
	cout << endl;
}

#endif //ARRAY1_H_

