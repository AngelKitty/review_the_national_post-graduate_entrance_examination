//10_11.cpp
#include <iostream>
#include <map>
#include <cctype>
using namespace std;

int main() {
	map<char, int> s;	//用来存储字母出现次数的映射
	char c;		//存储输入字符

	do {
		cin >> c;			//输入下一个字符
		if (isalpha(c)) {	//判断是否是字母
			c = tolower(c);	//将字母转换为小写
			s[c]++;			//将该字母的出现频率加1
		}
	} while (c != '.');		//碰到“.”则结束输入

	//输出每个字母出现次数
	for (map<char, int>::iterator iter = s.begin(); iter != s.end(); ++iter)
		cout << iter->first << " " << iter->second << "  ";
	cout << endl;

	return 0;
}
