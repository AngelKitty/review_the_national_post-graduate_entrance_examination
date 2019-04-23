#include <iostream>
#include <iomanip>
#include "array.h"
using namespace std;


int main()
{
	int n;
	double average,total = 0;
	cout << "请输入学生人数：";
	cin >> n;
	Array<float>  score(n);
	for (int i=0; i<n; i++)
	{
		cout << "请输入第" << i+1 <<"个学生的课程A成绩(0~100)：";
		cin >> score[i];
		total += score[i];
	}
	average = total/n;
	cout << "平均成绩为" << setprecision(4) << average << endl;
	return 0;
}
