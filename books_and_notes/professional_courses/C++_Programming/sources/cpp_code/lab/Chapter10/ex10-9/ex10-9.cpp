#include <iostream>
#include <iterator>
#include <algorithm>
using namespace std;

int main() {
	int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	cout << "数组的元素：";
	copy(a, a+10, ostream_iterator<int>(cout," "));
	cout << endl;
	swap(a[5], a[6]);
	cout << "swap函数进行交换后，数组的元素：";
	copy(a, a+10, ostream_iterator<int>(cout," "));
	cout << endl;
	iter_swap(&a[1], &a[4]);
	cout << "iter_swap函数进行交换后，数组的元素：";
	copy(a, a+10, ostream_iterator<int>(cout," "));
	cout << endl;
	swap_ranges(a, a+5, a+5);
	cout << "swap_ranges函数进行交换后，数组的元素：";
	copy(a, a+10, ostream_iterator<int>(cout," "));
	cout << endl;
	return 0;
}
