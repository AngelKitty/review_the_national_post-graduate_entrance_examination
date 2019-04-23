#include <iostream>
#include <cmath>
using namespace std;

int fn1(int i);

int main()
{
	int i;
	cout << "请输入一个正整数：";
	cin >> i ;
	
	cout << "从1累加到" <<i << "的和为：" << fn1(i) << endl;
	return 0;
}

int fn1(int i)
{
	if (i == 1)
		return 1;
	else
		return i + fn1(i -1);
}
