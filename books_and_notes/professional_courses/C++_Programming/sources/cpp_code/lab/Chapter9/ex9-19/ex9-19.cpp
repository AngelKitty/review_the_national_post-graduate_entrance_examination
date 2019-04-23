#include <iostream>
using namespace std;

// 用折半查找方法，在元素呈升序排列的数组list中查找值为key的元素
template <class T>
int binarySearch(T list[], int n, T key)
{
   int mid, low, high;
   T midvalue;
   low=0;
   high=n-1;
   while (low <= high)   // low <= high表示整个数组尚未查找完
   {
      mid = (low+high)/2;     // 求中间元素的下标
      midvalue = list[mid];   // 取出中间元素的值
      if (key == midvalue)    
         return mid;          // 若找到,返回下标
      else if (key < midvalue)
         high = mid-1;  
              // 若key < midvalue将查找范围缩小到数组的前一半
      else
         low = mid+1;   // 否则将查找范围缩小到数组的后一半
   }
   return -1;           // 没有找到返回-1
}

int main()
{
	int i, n;
	
	int data1[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	cout << "输入想查找的数字(1~20)：";
	cin >> n;
	cout << "数据为：" << endl;
	for(i=0;i<20;i++)
		cout << data1[i] << "  ";
	cout << endl;
	i = binarySearch ( data1 , 20 , n );
	if (i<0)
		cout << "没有找到数字" << n << endl;
	else
		cout << n << "是第" << i+1 << "个数字" << endl;
	return 0;
}
