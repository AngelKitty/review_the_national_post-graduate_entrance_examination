#include <iostream>
using namespace std ;

int main()
{
	int i,score;
	
	cout << "你考试考了多少分?(0~100):";
	cin >> score;
	if (score>100 || score<0)
		cout << "分数值必须在0到100之间!";
	else
	{
	
		i = score/10;
		switch (i)
		{
		case 10:
		case 9:
			cout << "你的成绩为优！";
			break;
		case 8:
			cout << "你的成绩为良！";
			break;
		case 7:
		case 6:
			cout << "你的成绩为中！";
			break;
		default:
			cout << "你的成绩为差！";
		}
	}
	return 0;
}
