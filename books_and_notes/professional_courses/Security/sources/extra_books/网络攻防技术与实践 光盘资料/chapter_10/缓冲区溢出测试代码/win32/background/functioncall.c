
#include "stdio.h"

int func(int a, int b){
  int retVal = a + b;
//  printf("b: 0x%08x\n",&b);  
//  printf("a: 0x%08x\n",&a); 

//  printf("ret addr here: 0x%08x\n",&a-1); 
//  printf("stored ebp here: 0x%08x\n",&a-2); 

//  printf("retVal: 0x%08x\n\n",&retVal);  

  return retVal;
}
int main(int argc, char* argv[])
{
	int result = func(1, 2);
//	printf("Hello World!\n");
	return 0;
}
