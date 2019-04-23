typedef unsigned short  USHORT;
#include <iostream>
using namespace std;

class Counter
{
public:
   Counter();
   Counter(USHORT initialValue);
   ~Counter(){}
   USHORT getValue()const { return value; }
   void setValue(USHORT x) {value = x; }
   Counter operator+ (const Counter &);
private:
   USHORT value;
};

Counter::Counter(USHORT initialValue):
value(initialValue)
{
}

Counter::Counter():
    value(0)
{
}

Counter Counter::operator+ (const Counter & rhs)
{
   return Counter(value + rhs.getValue());
}

int main()
{
   Counter varOne(2), varTwo(4), varThree;
   varThree = varOne + varTwo;
   cout << "varOne: " << varOne.getValue()<< endl;
   cout << "varTwo: " << varTwo.getValue() << endl;
   cout << "varThree: " << varThree.getValue() << endl;

   return 0;
}
