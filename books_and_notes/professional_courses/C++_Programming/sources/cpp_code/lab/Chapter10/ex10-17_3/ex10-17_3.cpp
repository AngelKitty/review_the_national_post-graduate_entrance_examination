#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
using namespace std;
using namespace boost::lambda;

int main() {
	int intArr[] = { 30, 90, 10, 40, 70, 50, 20, 80 };
	const int N = sizeof(intArr) / sizeof(int);
	vector<int> a(intArr, intArr + N);
	vector<int>::iterator p = find_if(a.begin(), a.end(), _1 > 40);
	if (p == a.end())
		cout << "no element greater than 40" << endl;
	else
		cout << "first element greater than 40 is: " << *p << endl;
	return 0;
}
