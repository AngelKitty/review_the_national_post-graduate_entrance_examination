//3_5.cpp
#include <iostream>
#include <cmath>  //头文件cmath中具有对C++标准库中数学函数的说明
using namespace std;

const double TINY_VALUE = 1e-10;

double tsin(double x) {
	double g = 0;
	double t = x;
	int n = 1;
	do {
		g += t;
		n++;
		t = -t * x * x / (2 * n - 1) / (2 * n - 2);
	} while (fabs(t) >= TINY_VALUE); 
	return g;
}

int main() {
	double k, r, s;
	cout << "r = ";
	cin >> r;
	cout << "s = ";
	cin >> s;
	if (r * r <= s * s)
		k = sqrt(tsin(r) * tsin(r) + tsin(s) * tsin(s));
	else
		k = tsin(r * s) / 2;
	cout << k << endl;
	return 0;
}

