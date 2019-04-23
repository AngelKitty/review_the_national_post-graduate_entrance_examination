#include <iostream>
using namespace std;

// 辅助函数：交换x和y的值
template <class T>
void swapData(T &x, T &y)
{
	T temp;
	
	temp = x;
	x = y;
	y = temp;
}

// 用选择法对数组A的n个元素进行排序
template <class T>
void selectSort(T a[], int n)
{
	int smallIndex;    //每以趟中选出的最小元素之下标
	int i, j;
	
	// sort a[0]..a[n-2], and a[n-1] is in place
	for (i = 0; i < n-1; i++) 
	{
		smallIndex = i;    //最小元素之下标初值设为i
		// 在元素a[i+1]..a[n-1]中逐个比较显出最小值
		for (j = i+1; j < n; j++) 
			// smallIndex始终记录当前找到的最小值的下标
			if (a[j] < a[smallIndex])
				smallIndex = j;
			// 将这一趟找到的最小元素与a[i]交换
			swapData(a[i], a[smallIndex]);
			//输出数据
			for(int k=0;k<n;k++)
				cout << a[k] << "  ";
			cout << endl;
			//结束输出	
			
	}
}

int main()
{
	int i;
	
	int data1[]={1,3,5,7,9,11,13,15,17,19,2,4,6,8,10,12,14,16,18,20};
	cout << "排序前的数据：" << endl;
	for(i=0;i<20;i++)
		cout << data1[i] << "  ";
	cout << endl;
	cout << "开始排序..." << endl;
	selectSort(data1, 20);
	cout << "排序后的数据：" << endl;
	for(i=0;i<20;i++)
		cout << data1[i] << "  ";
	cout << endl;
	return 0;
}
