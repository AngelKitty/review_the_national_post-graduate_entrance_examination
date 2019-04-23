#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

int main()
{
	int a[8] = {3, 2, 5, 6, 4, 1, 9, 8};
	cout << "ÔªËØ9µÄË÷ÒýÊÇ£º" << find(a, a+8, 9) - a << endl;
	cout << "ÉýÐòÅÅÐò£º" << endl;
	sort(a, a+8);
	for (int i = 0; i < 8; i ++)
		cout << a[i] << ' ';
	cout << endl;
	cout << "½µÐòÅÅÐò£º" << endl;
	sort(a, a+8, greater<int>());
	for (int i = 0; i < 8; i ++)
		cout << a[i] << ' ';
	cout << endl;
	return 0;
}
