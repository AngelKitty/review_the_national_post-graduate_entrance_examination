#include <iostream>
#include <map>
#include <utility>
#include <string>
using namespace std;

int main() {
	multimap<string, string> courses;
	typedef multimap<string, string>::iterator CourseIter;

	//将课程上课时间插入courses映射中
	courses.insert(make_pair("C++", "2-6"));
	courses.insert(make_pair("COMPILER", "3-1"));
	courses.insert(make_pair("COMPILER", "5-2"));
	courses.insert(make_pair("OS", "1-2"));
	courses.insert(make_pair("OS", "4-1"));
	courses.insert(make_pair("OS", "5-5"));
	//输入一个课程名，直到找到该课程为止，记下每周上课次数
	string name;
	int count;
	do {
		cin >> name;
		count = courses.count(name);
		if (count == 0)
			cout << "Cannot find this course!" << endl;
	} while (count == 0);
	//输出每周上课次数和上课时间
	cout << count << " lesson(s) per week: ";
	pair<CourseIter, CourseIter> range = courses.equal_range(name);
	for (CourseIter iter = range.first; iter != range.second; ++iter)
		cout << iter->second << " ";
	cout << endl;
	
	return 0;
}
