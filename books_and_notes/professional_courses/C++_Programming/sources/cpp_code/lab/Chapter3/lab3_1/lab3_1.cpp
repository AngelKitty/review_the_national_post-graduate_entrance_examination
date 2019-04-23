#include <iostream>
using namespace std;

float ftoc(int f) {
	return (f-32)*5.0/9;
}

int main()
{
	cout << "输入华氏度值：" << endl;
	int f;
	cin >> f;
	cout << "对应摄氏度值为" << ftoc(f) << endl;
	
	return 0;
}