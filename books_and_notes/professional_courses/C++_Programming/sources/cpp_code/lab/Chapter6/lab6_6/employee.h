#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <iostream>
using namespace std;

class Employee {
	char* name;
	char* address;
	char* city;
	char* code;
public:
	Employee(char* n = "", char* add = "", char *ct = "", char *cd = "") : name(n), address(add), city(ct), code(cd) {}
	Employee(Employee* e) {
		name = e->name;
		address = e->address;
		city = e->city;
		code = e->code;
	}
	void display() {
		cout << "name: " << name << endl;
		cout << "address: " << address << endl;
		cout << "city: " << city << endl;
		cout << "code: " << code << endl;
	}
	void change_name(char* nm) {
		name = nm;
	}
};

#endif
