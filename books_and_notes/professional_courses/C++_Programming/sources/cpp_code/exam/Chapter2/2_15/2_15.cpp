#include <iostream>
using namespace std;

enum Level { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };
enum Grade { A, B, C, D };

struct Student {
	unsigned number : 27;
	Level level : 2;
	Grade grade : 2;
};

int main() {
	Student s = { 12345678, SOPHOMORE, B };
	cout << "Size of Student: " << sizeof(Student) << endl;
	cout << "Number:    " << s.number << endl;
	cout << "Level:     ";
	switch (s.level) {
		case FRESHMAN:  cout << "freshman"; break;
		case SOPHOMORE: cout << "sophomore"; break;
		case JUNIOR:    cout << "junior"; break;
		case SENIOR:    cout << "senior"; break;
	}
	cout << endl;
	cout << "Grade:     ";
	switch (s.grade) {
		case A: cout << "A"; break;
		case B: cout << "B"; break;
		case C: cout << "C"; break;
		case D: cout << "D"; break;
	}
	cout << endl;
	return 0;
}
