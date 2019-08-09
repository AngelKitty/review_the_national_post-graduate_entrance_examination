#include<stdio.h> 
int main(int argc,char **argv){ 
   char buf[10]; 
   strcpy(buf,argv[1]); 
   printf("buf's 0x%8x\n",&buf); 
   getchar();
   return 0; 
} 
