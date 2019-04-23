#include <iostream>
using namespace std;

class Vehicle
{
public:
	int MaxSpeed;
	int Weight;
	void Run();
	void Stop();
};

class Bicycle : virtual public Vehicle
{
public:
	int Height;
};

class Motorcar : virtual public Vehicle
{
public:
	int SeatNum;
};

class Motorcycle : public Bicycle, public Motorcar
{
};

int main()
{
	return 0;
}

