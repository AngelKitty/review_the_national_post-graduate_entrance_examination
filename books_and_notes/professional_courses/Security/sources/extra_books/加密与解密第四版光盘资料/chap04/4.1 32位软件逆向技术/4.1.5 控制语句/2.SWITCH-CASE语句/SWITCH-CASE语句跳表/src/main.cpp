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
		case 3:printf("a=3");
					break;
		case 4:printf("a=4");
					break;
		case 5:printf("a=5");
					break;
		case 6:printf("a=6");
					break;
		case 7:printf("a=7");
					break;

		default :printf("a=default");
					 break;
	 }

	 return 0;
 }

