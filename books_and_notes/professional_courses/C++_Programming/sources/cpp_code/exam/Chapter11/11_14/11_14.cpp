//11_14.cpp
#include <iostream>
#include <string>
#include <fstream>
#include <locale>
using namespace std;

int main() {
	locale loc(".936");	//创建本地化配置方案

	wcout.imbue(loc);	//为wcout设置编码方案
	wifstream in("article.txt");	//创建文件宽输入流，打开文件article.txt
	in.imbue(loc);		//为in设置编码方案

	wstring line;			//用来存储一行内容
	unsigned number = 0;	//记录行号
	while (getline(in, line)) {
		number++;			//行号加1
		if (line.find_first_of(L'人') != wstring::npos)	//查找“人”字
			wcout << number << L": " << line << endl;	//输出包含“人”字的行
	}

	return 0;
}
