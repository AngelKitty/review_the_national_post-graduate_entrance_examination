/*******************************************************
《加密与解密》第四版配套实例
4.1.4 虚函数
(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/

#include <stdio.h>
class CSum
{
public:
	virtual int Add(int a, int b) 
	{
		return (a + b);
	}

	virtual	int	Sub(int a, int b )
	{
		return (a - b);
	}

};

void main()
{   
	CSum*	pCSum = new CSum ;  

	pCSum->Add(1,2);
	pCSum->Sub(1,2);


} 
