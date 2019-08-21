/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "convert.h"

/******************************************************************************************
 * 进制转换
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   /*DSA*/if ( argc < 3 ) //参数检查
      /*DSA*/{ cout << "Usage: " << argv[0] << " <integer> <base>" << endl; return -1; }
   for ( int i = 1; i < argc; i += 2 ) {
      /*DSA*/system ( "cls" );
      __int64 n = _atoi64 ( argv[i] ); //待转换的十进制数
      /*DSA*/if ( 0 >= n ) //参数检查
         /*DSA*/{ cout << "But " << n << " is not a positive integer" << endl;   return -2; }
      int base = atoi ( argv[i+1] ); //目标进制
      /*DSA*/if ( 2 > base || base > 16 ) //参数检查
         /*DSA*/{ cout << "But " << base << " is not between 2 and 16" << endl; return -2; }
      Stack<char> S; //用栈记录转换得到的各数位
      convert ( S, n, base ); //进制转换
      /*DSA*/printf ( "%20I64d_(10) = ", n );
      while ( !S.empty() ) printf ( "%c", ( S.pop() ) ); //逆序输出栈内数位，即正确结果
      /*DSA*/printf ( "_(%d)\a\n", base ); getchar();
   }
   return 0;
}