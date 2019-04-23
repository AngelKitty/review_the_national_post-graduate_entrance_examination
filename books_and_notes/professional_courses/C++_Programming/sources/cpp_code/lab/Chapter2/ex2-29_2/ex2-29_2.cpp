#include <iostream>
#include <cmath>
using namespace std ;

int main()
{
	int i,j,k,flag;
	i = 2;
	do{
		flag = 1;
		k = sqrt(1.0*i);
		j = 2;
		do{
			if(i%j == 0)
			{
				flag = 0;
				break;
			}
			j++;
		}while (j <= k);
		if (flag)
			cout << i << "ÊÇÖÊÊý." << endl;
		i++;
	}while(i <= 100);
	return 0;
}
