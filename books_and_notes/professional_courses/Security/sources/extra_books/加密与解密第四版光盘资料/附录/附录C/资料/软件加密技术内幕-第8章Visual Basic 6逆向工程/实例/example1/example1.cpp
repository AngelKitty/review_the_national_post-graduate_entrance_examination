#include <iostream>
using namespace std;

class Base {
public:
  Base() {
    cout << "In Base" << endl;
    cout << "Virtual Pointer = " 
         << (int*)this << endl;
    cout << "Address of Vtable = " 
         << (int*)*(int*)this << endl;
    cout << "Value at Vtable 1st entry = " 
         << (int*)*((int*)*(int*)this+0) << endl;
    cout << "Value at Vtable 2nd entry = " 
         << (int*)*((int*)*(int*)this+1) << endl;
    cout << "Value at Vtable 3rd entry = " 
         << (int*)*((int*)*(int*)this+2) << endl;
    cout << endl;
  }
  virtual void f1() { cout << "Base::f1" << endl; }
  virtual void f2() { cout << "Base::f2" << endl; }
  virtual void f3() { cout << "Base::f3" << endl; }
};


int main() {
  Base d;
  system("PAUSE"); 
  return 0;
}



