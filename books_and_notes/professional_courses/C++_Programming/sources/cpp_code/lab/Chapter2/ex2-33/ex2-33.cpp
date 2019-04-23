#include <iostream>
using namespace std;

enum Weekday {
	SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
};

int main() {
	int i;
	Weekday d = THURSDAY;
	cout << "d = " << d << endl;
	i = d;
	cout << "i = " << i << endl;

	d = (Weekday) 6;
	cout << "d = " << d << endl;
	d = Weekday(4);
	cout << "d = " << d << endl;
	return 0;
}
