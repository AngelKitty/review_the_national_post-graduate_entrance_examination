/*******************************************************
/*《加密与解密》第三版配套实例
/*  第15章 反跟踪技术
/*(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/
#include <stdio.h>
#include <stdlib.h>

 int main( int argc, char *argv[] )
 {
 if( argc != 2 )
  {
     printf("输入一个字符串\n");
     return 1;
   }
  printf( argv[1] );
 printf( "\n" );
 return 0;
 }
