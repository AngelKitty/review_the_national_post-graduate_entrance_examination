#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Employee
{
private:
	char name[30];
	char street[30];
	char city[18];
	char zip[6];
public:
	Employee(char *n, char *str, char *ct, char *z);
	void setName(char *n);
	void display();
};

Employee::Employee (char *n,char *str,char *ct, char *z)
{
	strcpy(name, n);
	strcpy(street, str);
	strcpy(city, ct);
	strcpy(zip, z);
}

void Employee::setName (char *n)
{
	strcpy(name, n);
}

void Employee::display ()
{
	cout << name << "\t" << street << "\t";
	cout << city  << "\t"<< zip;
}

int main()
{
	Employee e1("张三","平安大街3号", "北京", "100000");
	e1.display();
	cout << endl;
	e1.setName("李四");
	e1.display();
	cout << endl;
}
