#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <new>
using namespace std;

int main() {
	int n;
	int* s;
	cin >> n;

	try {
		s = new int[n];
	} catch (bad_alloc& e) {
		cout << e.what() << endl;
	}
	return 0;
}
