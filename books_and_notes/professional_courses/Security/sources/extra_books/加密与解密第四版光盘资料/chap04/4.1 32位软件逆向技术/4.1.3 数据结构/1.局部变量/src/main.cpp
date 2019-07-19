/*******************************************************
《加密与解密》第四版配套实例
 4.1.3 局部变量
(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/
int add(int x,int y);
int main(void)
 {
	 int a=5,b=6;
	 add(a,b);
	 return 0;
 }

 int add(int x,int y)
 {
	 int z;
	 z=x+y;
	 return(z);
 }