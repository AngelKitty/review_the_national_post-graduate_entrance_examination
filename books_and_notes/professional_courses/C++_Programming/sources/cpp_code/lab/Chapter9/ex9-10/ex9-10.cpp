#include <iostream>
using namespace std;


template <class T>
void insertSort(T A[], int n)
{
	int i, j;
	T   temp;
	
	// 将下标为1～n-1的元素逐个插入到已排序序列中适当的位置
	for (i = 1; i < n; i++) 
	{
		//从A[i-1]开始向A[0]方向扫描各元素,寻找适当位置插入A[i]
		j = i;
		temp = A[i];
		while (j > 0 && temp < A[j-1]) 
		{   //逐个比较，直到temp>=A[j-1]时，j便是应插入的位置。
			//若达到j==0，则0是应插入的位置。
			A[j] = A[j-1];                    //将元素逐个后移，以便找到插入位置时可立即插入。
			j--;
		}
		// 插入位置已找到，立即插入。
		A[j] = temp;
		
		//输出数据
		for(int k=0;k<n;k++)
			cout << A[k] << "  ";
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
	insertSort(data1, 20);
	cout << "排序后的数据：" << endl;
	for(i=0;i<20;i++)
		cout << data1[i] << "  ";
	cout << endl;
	return 0;
}
