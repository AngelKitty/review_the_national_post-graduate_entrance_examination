//9_4.cpp
#include <iostream>
#include <iomanip>
#include "Array.h"
using namespace std;

int main() {
	Array<int> a(10);	// 用来存放质数的数组，初始状态有10个元素。
	int count = 0;

	int n;
	cout << "Enter a value >= 2 as upper limit for prime numbers: ";
	cin >> n;

	for (int i = 2; i <= n; i++) {
		//检查i是否能被比它小的质数整除
		bool isPrime = true;
		for (int j = 0; j < count; j++)
			if (i % a[j] == 0) {	//若i被a[j]整除，说明i不是质数
				isPrime = false;
				break;
			}

		//把i写入质数表中
		if (isPrime) {
			//如果质数表满了，将其空间加倍
			if (count == a.getSize())
				a.resize(count * 2);
			a[count++] = i;
		}
	}

	for (int i = 0; i < count; i++)	//输出质数
		cout << setw(8) << a[i];
	cout << endl;

	return 0;
}
