//10_2.cpp
#include <iterator>
#include <iostream>
#include <algorithm>
using namespace std;

//求平方的函数
double square(double x) {
	return x * x;
}

int main() {
	//从标准输入读入若干个实数，分别将它们的平方输出
    transform(istream_iterator<double>(cin), istream_iterator<double>(),
		ostream_iterator<double>(cout, "\t"), square);
    cout << endl;
    return 0;
}
