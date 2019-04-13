//9_11.h
#ifndef HEADER_9_11_H
#define HEADER_9_11_H

//用直接插入排序法对数组A中的元素进行升序排列
template <class T>
void insertionSort(T a[], int n) {
	int i, j;
	T temp;

	//将下标为1～n-1的元素逐个插入到已排序序列中适当的位置
	for (int i = 1; i < n; i++) {
		//从a[i - 1]开始向a[0]方向扫描各元素,寻找适当位置插入a[i]
		int j = i;
		T temp = a[i];
		while (j > 0 && temp < a[j - 1]) {
			//逐个比较，直到temp >= a[j - 1]时，j便是应插入的位置。
			//若达到j == 0，则0是应插入的位置。
			a[j] = a[j - 1];    //将元素逐个后移，以便找到插入位置时可立即插入。
			j--;
		}
		//插入位置已找到，立即插入。
		a[j] = temp;
	}
}
#endif	//HEADER_9_11_H
