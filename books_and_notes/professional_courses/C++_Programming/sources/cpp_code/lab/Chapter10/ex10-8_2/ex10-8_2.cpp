#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
	string str;
	map<string, int> ismap;
	int i = 0;
	while (1) {
		cout << "输入字符串: ";
		cin >> str;
		if (str == "QUIT")
			break;
		int counter = ismap.count(str);
		if (counter > 0)
			cout << str << "在映射中曾经出现过" << counter << "次" << endl;
		else
			cout << str << "在映射中没有出现过" << endl;
		ismap.insert(map<string, int>::value_type(str, i));
		i ++;
	}
	return 0;
}
