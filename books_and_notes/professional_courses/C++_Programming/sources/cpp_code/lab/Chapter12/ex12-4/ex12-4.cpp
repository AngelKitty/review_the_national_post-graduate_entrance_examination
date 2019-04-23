#include <iostream>
using namespace std;

class Exception
{
public:
	Exception(){}
    virtual ~Exception(){}
    virtual void PrintError() = 0;
};

class OutOfMemory : public Exception
{
public:
    OutOfMemory(){}
    ~OutOfMemory(){}
    virtual void PrintError();
};

void OutOfMemory::PrintError()
{
    cout << "Out of Memory!!\n";
}

class RangeError : public Exception
{
public:
    RangeError(unsigned long number){BadNum = number;}
    ~RangeError(){}
    virtual void PrintError();
    virtual unsigned long GetNumber() { return BadNum; }
    virtual void SetNumber(unsigned long number) {BadNum = number;}
private:
    unsigned long BadNum;
};

void RangeError::PrintError()
{
    cout << "Number out of range. You used " << GetNumber() << " !\n";
}


void fn1();
unsigned int * fn2();
void fn3(unsigned int *);

int main()
{
    try
    {
		fn1();
    }
	
    catch (Exception& theException)
    {
		theException.PrintError();
    }
    return 0;
}

unsigned int * fn2()
{
    unsigned int *n = new unsigned int;
	if (n == 0)
		throw OutOfMemory();
	return n;
}

void fn1()
{
	unsigned int *p = fn2();
	
	fn3(p);
	cout << "The number is : " << *p << endl;
	delete p;
}

void fn3(unsigned int *p)
{
	long Number;
	cout << "Enter an integer(0~~1000): ";
	cin >> Number;
	
	if (Number > 1000 || Number < 0)
		throw RangeError(Number);
	*p = Number;
}
