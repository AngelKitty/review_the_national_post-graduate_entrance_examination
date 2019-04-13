//2_12.cpp
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Student {	//学生信息结构体
	int num;		//学号
	string name;	//姓名
	char sex;		//性别
	int age;		//年龄
} stu = { 97001, "Lin Lin", 'F', 19};

int main() {
	cout << setw(7) << stu.num;
	cout << setw(20) << stu.name;
	cout << setw(3) << stu.sex;
	cout << setw(3) << stu.age;
	cout << endl;
	return 0;
}
