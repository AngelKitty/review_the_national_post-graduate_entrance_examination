//6_7_3.cpp
#include <iostream>
using namespace std;

int main() {
	int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	for (int *p = a; p < (a + 10); p++)
		cout << *p << "  ";
	cout << endl;
	return 0;
}
