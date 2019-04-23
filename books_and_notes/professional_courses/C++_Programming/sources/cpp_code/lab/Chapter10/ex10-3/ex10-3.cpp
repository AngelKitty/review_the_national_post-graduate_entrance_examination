#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> INTVECTOR;
int main()
{
	INTVECTOR s;
	for (int i = 0; i < 5; i++) {
		s.push_back(i);
		cout << "增加一个元素后，整型向量容器对象s包含" << s.capacity() << "个元素" << endl;
	}
	return 0;
}
