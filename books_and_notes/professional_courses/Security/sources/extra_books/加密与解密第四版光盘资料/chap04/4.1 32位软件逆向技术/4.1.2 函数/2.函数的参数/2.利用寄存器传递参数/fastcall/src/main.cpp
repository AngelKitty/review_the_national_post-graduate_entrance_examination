/*******************************************************
*  fastcall调用                                                                                                   *
*   (c)  看雪学院 www.kanxue.com 2000-2018                       *
********************************************************/

int  __fastcall Add(char,long,int,int);

main(void)
 {
	    Add(1,2,3,4);
	    return 0;
 }

int __fastcall Add(char a, long b, int c, int d)

{
	   return (a + b + c + d);
}
