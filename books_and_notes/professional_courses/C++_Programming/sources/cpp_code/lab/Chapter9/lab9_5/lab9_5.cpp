#include <iostream>
#include "array1.h"
using namespace std;

int main()
{
	int a[5] = {3, 6, 1, 8, 2};
	double d[4] = {4.1, 3.2, 5.6, 1.9};
	char c[3] = {'g', 'c', 'A'};
	Array<int> iArray(a, 5);
	iArray.insertSort();
	Array<double> dArray(d, 4);
	dArray.selectSort();
	Array<char> cArray(c, 3);
	cArray.bubbleSort();
	return 0;
}
