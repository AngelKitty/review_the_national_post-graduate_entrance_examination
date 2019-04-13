//6_23.cpp
#include <string>
#include <iostream>
using namespace std;

//根据value的值输出true或false，title为提示符
inline void test(const char *title, bool value) {
	cout << title << " returns " << (value ? "true" : "false") << endl;
}

int main() {
	string s1 = "DEF";
	cout << "s1 is " << s1 << endl;

	string s2;
	cout << "Please enter s2: ";
	cin >> s2;
	cout << "length of s2: " << s2.length() << endl;

	//比较运算符的测试
	test("s1 <= \"ABC\"", s1 <= "ABC"); 
	test("\"DEF\" <= s1", "DEF" <= s1);
	//连接运算符的测试
	s2 += s1;
	cout << "s2 = s2 + s1: " << s2 << endl;
	cout << "length of s2: " << s2.length() << endl;
	return 0;
}
