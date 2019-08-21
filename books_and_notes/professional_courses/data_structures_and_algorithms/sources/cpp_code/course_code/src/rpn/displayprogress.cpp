/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "rpn.h"
#include "../UniPrint/print.h"

/******************************************************************************************
 * 输出运算符
 ******************************************************************************************/
void printChar ( char& p )
{ ( '\0' == p ) ? printf ( "\\0 " ) : printf ( "%c  ", p ); }

/******************************************************************************************
 * 显示表达式处理进展
 ******************************************************************************************/
void displayProgress ( char* expr, char* pCh, Stack<float>& opndStk, Stack<char>& optrStk, char* rpn ) {
   system ( "cls" );
   printf ( "\n" );
   for ( char* p = expr; '\0' != *p; p++ ) printf ( " %c", *p ); printf ( " $\n" );
   for ( char* p = expr; p < pCh; p++ ) printf ( "  " );
   if ( '\0' != * ( pCh - 1 ) )
      { for ( char* p = pCh; '\0' != *p; p++ ) printf ( " %c", *p ); printf ( " $" ); }
   printf ( "\n" );
   for ( char* p = expr; p < pCh; p++ ) printf ( "--" ); printf ( " ^\n\n" );
   print ( optrStk ); printf ( "\n" );
   print ( opndStk ); printf ( "\n" );
   printf ( "RPN:\n %s\n\n", rpn ); //输出RPN
   getchar();
}