#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main()
{
	locale loc("zh_CN.GBK");	//创建本地化配置方案
	wcout.imbue(loc);	//为wcout设置编码方案
	wifstream in("习题9-10输入.txt");	//创建文件宽输入流
	wofstream out("习题9-10输出.txt");	//创建文件宽输入流
	in.imbue(loc);		//为in设置编码方案
	out.imbue(loc);		//为out设置编码方案
	wstring line;		//用来存储一行内容
	map<wchar_t, int> counter;
	
	while(getline(in,line))
	{
		for(int i=0; i<line.length(); ++i)
		{
			counter[line[i]] ++;
		}
	}

	map<wchar_t, int>::iterator itor;
	int i = 1;
	for(itor=counter.begin(); itor != counter.end(); ++itor, ++i)
	{
		out<<itor->first<<"\t"<<itor->second<<"\t";
		if(i%4 == 0)
		{
			out<<endl;
		}
	}
	in.close();
	out.close();
	return 0;
}
