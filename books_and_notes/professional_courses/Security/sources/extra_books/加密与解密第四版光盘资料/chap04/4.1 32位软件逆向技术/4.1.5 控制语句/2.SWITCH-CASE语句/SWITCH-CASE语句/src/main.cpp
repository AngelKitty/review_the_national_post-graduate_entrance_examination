/*******************************************************
《加密与解密》第四版配套实例
4.1.5 SWITCH-CASE语句
(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/

#include <stdio.h>

int main(void)
 {
	 int a;
     scanf("%d",&a);
	
	 switch(a)
	 {
		case 1 :printf("a=1");
				    break;
		case 2 :printf("a=2");
					break;
		case 10:printf("a=10");
					break;
		default :printf("a=default");
					 break;
	 }

	 return 0;
 }

