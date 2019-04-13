//11_6.cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <class T>
inline string toString(const T &v) {
	ostringstream os;	//创建输出字符串流
	os << v;			//将变量v的值写入字符串流
	return os.str();	//返回输出流生成的字符串
}

int main() {
	string str1 = toString(5);
	cout << str1 << endl;
	string str2 = toString(1.2);
	cout << str2 << endl;
	return 0;
}
