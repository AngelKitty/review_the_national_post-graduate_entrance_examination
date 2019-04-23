#include <iostream>
using namespace std;

float p(int n, int x);

int main()
{
	int n,x;
	cout << "请输入正整数n：";
	cin >> n;
	cout << "请输入正整数x：";
	cin >> x;
	
	cout << "n = " << n << endl;
	cout << "x = " << x << endl;
	cout << "P" << n << "(" << x << ") = " << p(n,x) << endl;
	return 0;
}

float p(int n, int x)
{
	if (n == 0)
		return 1;
	else if (n == 1)
		return x;
	else
		return ((2*n-1)*x*p(n-1,x) - (n-1)*p(n-2,x)) /n ;
}
