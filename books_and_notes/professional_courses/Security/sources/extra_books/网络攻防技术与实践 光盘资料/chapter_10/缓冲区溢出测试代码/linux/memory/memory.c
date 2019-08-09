#include <stdio.h>
#include <stdlib.h>

int g1=0;
int g2=0;
int g3=0;
int gu1;
int gu2;
int gu3;

int main (int argc, char **argv)
{
static int s1=0;
static int s2=0;
static int s3=0;
static int su1;
static int su2;
static int su3;
int* d1;
int* d2;
int* d3;
int v1=0;
int v2=0;
int v3=0;

//´òöö¿µÄڴæַ
printf("argc & argv\n");
printf("argc: 0x%08x\n",&argc);
printf("argv: 0x%08x\n\n",&argv);


printf("local pointers\n");
printf("0x%08x\n",&d1); //´ò����Ø¸ÕµÄڴæַ
printf("0x%08x\n",&d2);
printf("0x%08x\n\n",&d3);

printf("local vars\n");
printf("0x%08x\n",&v1); //´ò����رäµÄڴæַ stack
printf("0x%08x\n",&v2);
printf("0x%08x\n\n",&v3);

printf("global vars\n");
printf("0x%08x\n",&g1); //´ò÷ֱäµÄڴæַ .data
printf("0x%08x\n",&g2);
printf("0x%08x\n\n",&g3);

printf("static vars\n");
printf("0x%08x\n",&s1); //´ò÷¬±äµÄڴæַ .data
printf("0x%08x\n",&s2);
printf("0x%08x\n\n",&s3);

printf("global uninitialized vars\n");
printf("0x%08x\n",&gu1); //´ò÷õ¯ȫ¾ֱäµÄڴæַ .bss
printf("0x%08x\n",&gu2);
printf("0x%08x\n\n",&gu3);

printf("static uninitialized vars\n");
printf("0x%08x\n",&su1); //´ò÷õ¯¾²̬±äµÄڴæַ .bss
printf("0x%08x\n",&su2);
printf("0x%08x\n\n",&su3);

d1 = malloc(sizeof(int));	
d2 = malloc(sizeof(int));
d3 = malloc(sizeof(int));
printf("dynamic allocated vars\n");
printf("0x%08x\n",d1); //´ò÷¬±äµÄڴæַ heap
printf("0x%08x\n",d2);
printf("0x%08x\n\n",d3);



return 0;
}


