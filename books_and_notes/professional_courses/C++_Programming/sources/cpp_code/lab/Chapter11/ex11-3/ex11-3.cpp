#include <fstream>
using namespace std;

int main()  
{
	ofstream file1("test.txt");
	file1 << "已成功写入文件！";
	file1.close();
	return 0;
}
