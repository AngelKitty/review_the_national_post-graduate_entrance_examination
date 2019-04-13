#include <string>
#include <iostream>
using namespace std;

class ExamInfo {
public:
	//三种构造函数，分别用等级、是否通过和百分来初始化
	ExamInfo(string name, char grade)
		: name(name), mode(GRADE), grade(grade) { }
	ExamInfo(string name, bool pass)
		: name(name), mode(PASS), pass(pass) { }
	ExamInfo(string name, int percent)
		: name(name), mode(PERCENTAGE), percent(percent) { }
	void show();

private:
	string name;	//课程名称
	enum {
		GRADE,
		PASS,
		PERCENTAGE
	} mode;			//采用何种计分方式
	union {
		char grade;		//等级制的成绩
		bool pass;		//是否通过
		int percent;	//百分制的成绩
	};
};

void ExamInfo::show() {
	cout << name << ": ";
	switch (mode) {
	case GRADE:
		cout << grade;
		break;
	case PASS:
		cout << (pass ? "PASS" : "FAIL");
		break;
	case PERCENTAGE:
		cout << percent;
		break;
	}
	cout << endl;
}

int main() {
	ExamInfo course1("English", 'B');
	ExamInfo course2("Calculus", true);
	ExamInfo course3("C++ Programming", 85);
	course1.show();
	course2.show();
	course3.show();
	return 0;
}
