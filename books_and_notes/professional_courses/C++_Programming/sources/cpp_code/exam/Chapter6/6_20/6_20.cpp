//6_20.cpp
#include <iostream>
#include <vector>
using namespace std;

//计算数组arr中元素的平均值
double average(const vector<double> &arr) {
	double sum = 0;
	for (unsigned i = 0; i < arr.size(); i++)
		sum += arr[i];
	return sum / arr.size();
}

int main() {
	unsigned n;
	cout << "n = ";
	cin >> n;

	vector<double> arr(n);	//创建数组对象
	cout << "Please input " << n << " real numbers:" << endl;
	for (unsigned i = 0; i < n; i++)
		cin >> arr[i];

	cout << "Average = " << average(arr) << endl;
	return 0;
}
