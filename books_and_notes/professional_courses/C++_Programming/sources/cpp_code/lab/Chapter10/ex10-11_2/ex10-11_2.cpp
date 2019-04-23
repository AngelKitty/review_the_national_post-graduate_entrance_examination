#include <iostream>
#include <list>
#include <iterator>
using namespace std;

int main() {
	int arr[] = { 1, 1, 4, 4, 5 };
	list<int> s(arr, arr + 5);
	s.unique();
	copy(s.begin(), s.end(), ostream_iterator<int> (cout, "\n"));
	return 0;
}
