#include <fstream>
#include <iostream>
using namespace std;

int main()  
{
	char ch;
	ifstream file2("test.txt");
	while (file2.get(ch))
		cout << ch;
	file2.close();
	return 0;
}
