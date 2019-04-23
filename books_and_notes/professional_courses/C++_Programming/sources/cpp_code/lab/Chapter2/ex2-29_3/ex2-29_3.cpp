#include <iostream>
#include <cmath>
using namespace std ;

int main()
{
	int i,j,k,flag;
	for(i = 2; i <= 100; i++)
	{
		flag = 1;
		k = sqrt(1.0*i);
		for (j = 2; j <= k; j++)
		{
			if(i%j == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
			cout << i << "ÊÇÖÊÊý." << endl;
	}
	return 0;
}
