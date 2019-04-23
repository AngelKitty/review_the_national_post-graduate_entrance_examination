#include <iostream>
#include <string>
using namespace std;

string reverse(string& str) {
	if (str.length() > 1) {
		string sub = str.substr(1, str.length() - 2);
		return str.substr(str.length() - 1, 1) + reverse(sub) + str.substr(0, 1);
	} else
		return str;
}

int main() {
	string str;
	cout << "输入一个字符串：";
	cin >> str;
	cout << "原字符串为：" << str << endl;
	cout << "倒序反转后为：" << reverse(str) << endl;
	return 0;
}
