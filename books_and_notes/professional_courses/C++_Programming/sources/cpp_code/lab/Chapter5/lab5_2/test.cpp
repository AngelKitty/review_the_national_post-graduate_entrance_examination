#include <iostream>
#include "client.h"
using namespace std;

int Client::ClientNum = 0;
char Client::ServerName = 'a';

int main()
{
	Client c1;
	c1.ChangeServerName('a');
	cout << c1.getClientNum() << endl;
	Client c2;
	c2.ChangeServerName('b');
	cout << c2.getClientNum() << endl;
	return 0;
}