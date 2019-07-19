/*******************************************************
《加密与解密》第四版配套实例
 4.1.3 全局变量
(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/
int z;

int add(int x,int y);
int main(void)
 {
	 int a=5,b=6;
	 z=7;
	 add(a,b);
	 return 0;
 }

 int add(int x,int y)
 {
	 return(x+y+z);
 }