#include <iostream>
using namespace ::std;

int main()
{
	ios_base::fmtflags original_flags = cout.flags();	  //1  
	cout<< 812<<'|';
	cout.setf(ios_base::left,ios_base::adjustfield);      //2
	cout.width(10);                                 //3
	cout<< 813 << 815 << '\n';
	cout.unsetf(ios_base::adjustfield);                 //4
	cout.precision(2);
	cout.setf(ios_base::uppercase|ios_base::scientific);  //5
	cout << 831.0 ;
	
	cout.flags(original_flags);
	return 0;                         //6
}
