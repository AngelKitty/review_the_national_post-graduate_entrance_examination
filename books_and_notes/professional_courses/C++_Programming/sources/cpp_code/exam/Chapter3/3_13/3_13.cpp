//3_13.cpp
#include <iostream>
#include <iomanip>
using namespace std;

void fiddle(int in1, int &in2) {
	in1 = in1 + 100;
	in2 = in2 + 100;
	cout << "The values are ";
	cout << setw(5) << in1;
	cout << setw(5) << in2 << endl;
}

int main() { 
	int v1 = 7, v2 = 12;
	cout << "The values are ";
	cout << setw(5) << v1;
	cout << setw(5) << v2 << endl;
	fiddle(v1, v2);
	cout << "The values are ";
	cout << setw(5) << v1;
	cout << setw(5) << v2 << endl;
	return 0;
}
