#include <iostream>
using namespace std;

// 辅助函数：交换x和y的值
template <class T>
void swapData (T &x, T &y)
{
	T temp;
	
	temp = x;
	x = y;
	y = temp;
}

// 用起泡法对数组A的n个元素进行排序
template <class T>
void BubbleSort(T a[], int n)
{
	int i,j;             
	int lastExchangeIndex; 
                //用于记录每趟被交换的最后一对元素中较小的下标
	i = n-1;      // i是下一趟需参与排序交换的元素之最大下标
	
	while (i > 0) 
     //持续排序过程，直到最后一趟排序没有交换发生，或已达n-1趟
	{
		lastExchangeIndex = 0; 
                //每一趟开始时，设置交换标志为0（未交换）
		for (j = 0; j < i; j++)  //每一趟对元素a[0]..a[i]进行比较和交换
			if (a[j+1] > a[j])   //如果元素a[j+1] < a[j]，交换之
			{ 
				swapData(a[j],a[j+1]);
				lastExchangeIndex = j; 
                           //记录被交换的一对元素中较小的下标
			}
			
			// 将i设置为本趟被交换的最后一对元素中较小的下标
			i = lastExchangeIndex;
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
	BubbleSort(data1, 20);
	cout << "排序后的数据：" << endl;
	for(i=0;i<20;i++)
		cout << data1[i] << "  ";
	cout << endl;
	return 0;
}
