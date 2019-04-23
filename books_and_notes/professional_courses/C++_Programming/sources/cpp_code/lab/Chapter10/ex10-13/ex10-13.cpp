#include <iostream>
#include <iterator>
#include <cstdio>
#include <ctime>
#include <list>
#include <cmath>
#include <algorithm>
using namespace std;

int arandom() {
	return rand()%10;
}
int power(int x, int y) {
	return (int)(pow(1.0*x, 1.0*y)+0.5);
}
int power1(int x) {
	return power(5, x);
}
int power2(int x) {
	return power(x, 7);
}
int power3(int x) {
	return power(x, x);
}

int main() {
	srand((int)time(0));
	list<int> l(20);
	generate(l.begin(), l.end(), arandom);
	list<int> l1(l), l2(l), l3(l);
	cout << "list的元素：" << endl;
	copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
	transform(l1.begin(), l1.end(), l1.begin(), power1);
	cout << "\n对每个元素n求5^n：" << endl;
	copy(l1.begin(), l1.end(), ostream_iterator<int>(cout, " "));
	transform(l2.begin(), l2.end(), l2.begin(), power2);
	cout << "\n对每个元素n求n^7：" << endl;
	copy(l2.begin(), l2.end(), ostream_iterator<int>(cout, " "));
	transform(l3.begin(), l3.end(), l3.begin(), power3);
	cout << "\n对每个元素n求n^n：" << endl;
	copy(l3.begin(), l3.end(), ostream_iterator<int>(cout, " "));
	return 0;
}
