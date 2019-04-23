#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
	const int SIZE1 = 6, SIZE2 = 4;
	int a1[SIZE1];
	int a2[SIZE2];
	ostream_iterator<int> output(cout, " ");

	cout << "输入数组a1的6个元素: ";
	for (int i = 0; i < SIZE1; i ++)
		cin >> a1[i];
	cout << "输入数组a2的4个元素: ";
	for (int i = 0; i < SIZE2; i ++)
		cin >> a2[i];

	cout << "数组a1的元素: ";
	copy(a1, a1 + SIZE1, output);
	cout << "\n数组a2的元素: ";
	copy(a2, a2 + SIZE2, output);

	int intersection[SIZE1+SIZE2];
	int *ptr = set_intersection(a1, a1 + SIZE1, a2, a2 + SIZE2, intersection);
	cout << "\na1和a2的交集: ";
	copy(intersection, ptr, output);

	int unionSet[SIZE1];
	ptr = set_union(a1, a1 + SIZE1, a2, a2 + SIZE2, unionSet);
	cout << "\na1和a2的并集: ";
	copy(unionSet, ptr, output);

	int difference[SIZE1];
	ptr = set_difference(a1, a1 + SIZE1, a2, a2 + SIZE2, difference);
	cout << "\na1/a2集合: ";
	copy(difference, ptr, output);

	return 0;
}
