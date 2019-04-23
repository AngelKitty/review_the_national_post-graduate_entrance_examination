#include <iostream>
using namespace std;

class BaseClass
{
public:
    BaseClass();
};

BaseClass::BaseClass()
{
    cout << "构造基类对象!" << endl;

}

class DerivedClass : public BaseClass
{
public:
    DerivedClass();   
};

DerivedClass::DerivedClass()
{
    cout << "构造派生类对象!" << endl;
}


int main()
{
    DerivedClass d;
}
