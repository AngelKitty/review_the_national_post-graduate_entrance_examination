/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "rpn.h"

char* removeSpace ( char* s ) { //剔除s[]中的白空格
   char* p = s, *q = s;
   while ( true ) {
      while ( isspace ( *q ) ) q++;
      if ( '\0' == *q ) { *p = '\0'; return s; }
      *p++ = *q++;
   }
}

int main ( int argc, char* argv[] ) { //表达式求值（入口）
   for ( int i = 1; i < argc; i++ ) { //逐一处理各命令行参数（表达式）
      /*DSA*/system ( "cls" ); printf ( "\nPress any key to evaluate: [%s]\a\n", argv[i] ); getchar();
      char* rpn = ( char* ) malloc ( sizeof ( char ) * 1 );   rpn[0] = '\0'; //逆波兰表达式
      float value = evaluate ( removeSpace ( argv[i] ), rpn ); //求值
      /*DSA*/printf ( "EXPR\t: %s\n", argv[i] ); //输出原表达式
      /*DSA*/printf ( "RPN\t: [ %s]\n", rpn ); //输出RPN
      /*DSA*/printf ( "Value\t= %.1f = %d\n", value, ( int ) value ); //输出表达式的值
      free ( rpn );   rpn = NULL;
      /*DSA*/getchar();
   }
   return 0;
}