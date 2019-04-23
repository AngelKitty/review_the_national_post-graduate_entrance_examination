#include <iostream>
#include <cmath>
using namespace std;

int prime(int i); 		//判一个数是否是质数的函数

int main()
{
	int i;
	cout << "请输入一个整数：";
	cin >> i;
	if (prime(i))
		cout << i << "是质数." << endl;
	else
		cout << i << "不是质数." << endl;
	return 0;
}

int prime(int i)
{
	int j,k,flag;
	flag = 1;
	k = sqrt(1.0*i);
	for (j = 2; j <= k; j++)
	{
		if(i%j == 0)
		{
			flag = 0;
			break;
		}
	}
	return flag;
	
}
