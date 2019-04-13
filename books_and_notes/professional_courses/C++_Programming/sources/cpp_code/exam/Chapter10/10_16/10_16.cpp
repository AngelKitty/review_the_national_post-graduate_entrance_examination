//10_16.cpp
#include <functional>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

int main() {
	int intArr[] = { 30, 90, 10, 40, 70, 50, 20, 80 };
	const int N = sizeof(intArr) / sizeof(int);
	vector<int> a(intArr, intArr + N);

	cout << "before sorting:" << endl;
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, "\t"));
	cout << endl;

	sort(a.begin(), a.end(), greater<int>());

	cout << "after sorting:" << endl;
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, "\t"));
	cout << endl;
	return 0;
}
