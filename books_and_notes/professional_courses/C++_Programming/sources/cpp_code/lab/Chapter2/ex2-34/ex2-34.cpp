#include<iostream>
using namespace std ;

int main()
{
	enum color{red,yellow,blue,white,black};
	enum color pri;
	int n,loop,i,j,k;
	char c;
	n=0;
	for(i=red;i<=black;i++)
		for(j=red;j<=black;j++)
			if(i!=j) //前两个球不同
			{
				for(k=red;k<=black;k++)
					if((k!=i)&&(k!=j)) //第三个球不同于前两个
					{  
						n=n+1;
						cout.width(4);
						cout<<n;
						
						for(loop=1;loop<=3;loop++)
						{
							switch(loop)
							{
							case 1: pri=(enum color)i; break;
							case 2: pri=(enum color)j; break;
							case 3: pri=(enum color)k; break;
							default: break;
							}
							switch(pri)
							{  
							case red: cout<<"       red"; break;
							case yellow: cout<<"    yellow"; break;
							case blue: cout<<"      blue"; break;
							case white: cout<<"     white"; break;
							case black: cout<<"     black"; break;
							default: break;
							}
						}
						cout<<endl;
					}
			}
			cout<<"total:"<<n<<endl;
	return 0;
}

