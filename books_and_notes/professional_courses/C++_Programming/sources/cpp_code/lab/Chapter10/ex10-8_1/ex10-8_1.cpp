#include <iostream>
#include <string>
#include <set>
using namespace std;

int main() {
	string str;
	multiset<string> strset;
	while (1) {
		cout << "输入字符串: ";
		cin >> str;
		if (str == "QUIT")
			break;
		int counter = strset.count(str);
		if (counter > 0)
			cout << str << "在集合中曾经出现过" << counter << "次" << endl;
		else
			cout << str << "在集合中没有出现过" << endl;
		strset.insert(str);
	}
	return 0;
}
