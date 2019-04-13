//3_4.cpp
#include <iostream>
using namespace std;

//判断n是否为回文数
bool symm(unsigned n) {
	unsigned i = n;
	unsigned m = 0;
	while (i > 0) {
		m = m * 10 + i % 10;
		i /= 10;
	}
	return m == n;
}

int main() {
	for (unsigned m = 11; m < 1000; m++)
		if (symm(m) && symm(m * m) && symm(m * m * m)) {
			cout << "m = " << m;
			cout << "  m * m = " << m * m;
			cout << "  m * m * m = " << m * m * m << endl;
		}
	return 0;
}
