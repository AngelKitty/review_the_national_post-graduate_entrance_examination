//4_7.cpp
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Student {	//学生信息结构体
	int num;		//学号
	string name;	//姓名
	char sex;		//性别
	int age;		//年龄
};

int main() {
	Student stu = { 97001, "Lin Lin", 'F', 19 };
	cout << "Num:  " << stu.num << endl;
	cout << "Name: " << stu.name << endl;
	cout << "Sex:  " << stu.sex << endl;
	cout << "Age:  " << stu.age << endl;
	return 0;
}
