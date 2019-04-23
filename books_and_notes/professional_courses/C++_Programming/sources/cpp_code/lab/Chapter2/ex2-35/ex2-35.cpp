#include <iostream>
#include <iomanip>
using namespace std ;

int main() {
	int i, j;
	cout << ' ';
	for (i = 1; i < 10; i++)
		cout << setw(4) << i;
	cout << endl;
	for (i = 1; i < 10; i++) {
		cout << i;
		for (j = 1; j < 10; j++)
			cout << setw(4) << (i * j);
		cout << endl;
	}
	return 0;
}
