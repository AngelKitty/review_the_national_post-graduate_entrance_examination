#include <iostream>
using namespace std;

int max1(int x, int y) {
	return x > y ? x : y;
}

int max1(int x, int y, int z) {
	return max1(x, max1(y, z));
}

double max1(double x, double y) {
	return x > y ? x : y;
}

double max1(double x, double y, double z) {
	return max1(x, max1(y, z));
}

int main()
{
	cout << max1(2, 4) << endl;
	cout << max1(2, 4, 7) << endl;
	cout << max1(2.1, 5.0/3) << endl;
	cout << max1(2.1, 4.5, 5.0/3) << endl;
	return 0;
}