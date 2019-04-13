//11_12.cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <class T>
inline T fromString(const string &str) {
	istringstream is(str);	//创建输入字符串流
	T v;
	is >> v;				//从输入字符串流中读取变量v
	return v;				//返回变量v
}

int main() {
	int v1 = fromString<int>("5");
	cout << v1 << endl;
	double v2 = fromString<double>("1.2");
	cout << v2 << endl;
	return 0;
}
