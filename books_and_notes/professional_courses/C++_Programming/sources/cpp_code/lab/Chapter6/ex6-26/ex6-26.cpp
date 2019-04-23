#include <iostream>
using namespace std;
void move (int *matrix ,int n)
{
	int i, j, k;
	for(i=0; i<n; i++)
		for (j=0; j<i; j++)
		{
			k = *(matrix + i*n + j);
			*(matrix + i*n + j) = *(matrix + j*n + i);
			*(matrix + j*n + i) = k;
		}
}
int main()
{
	int n, i, j;
	int *p;
	cout << "请输入矩阵的行、列数：";
	cin >> n;
	p = new int[n*n];
	cout << "输入矩阵的元素" << endl;
	for(i=0; i<n; i++)
		for (j=0; j<n; j++)
		{
			cout << "第" << i+1 << "行第" << j+1
			<<"个元素为：";
			cin >> p[i*n + j];
		}
	cout << "输入的矩阵的为：" << endl;
	for(i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
			cout << p[i*n + j] << " ";
		cout << endl;
	}
	move(p, n);
	cout << "转置后的矩阵的为：" << endl;
	for(i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
			cout << p[i*n + j] << " ";
		cout << endl;
	}
	return 0;
}