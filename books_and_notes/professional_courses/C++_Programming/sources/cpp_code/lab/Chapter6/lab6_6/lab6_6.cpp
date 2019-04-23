#include <iostream>
#include "employee.h"
using namespace std;

int main()
{
	Employee emp[5];
	emp[0] = new Employee("Zhang San", "Haidian", "Beijing", "100084");
	emp[1] = new Employee("Li Si", "Xuanwu", "Beijing", "100031");
	emp[2] = new Employee("Wang Wu", "Nanfu", "Shanghai", "012345");
	emp[3] = new Employee("Zhu Ge", "Wuhou", "Chengdu", "543210");
	emp[4] = new Employee("Sun ying", "Zhifu", "Yantai", "264000");
	for (int i = 0; i < 5; i ++) {
		cout << "Number" << i << ':' << endl;
		emp[i].display();
	}
	return 0;
}