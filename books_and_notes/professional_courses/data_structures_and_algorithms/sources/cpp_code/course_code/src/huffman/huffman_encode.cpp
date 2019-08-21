/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "Huffman_PQ.h"

int encode ( HuffTable* table, Bitmap* codeString, char* s ) { //按照编码表对Bitmap串编码
   int n = 0; //待返回的编码串总长n
   for ( size_t m = strlen ( s ), i = 0; i < m; i++ ) { //对于明文中的每个字符
      char** pCharCode = table->get ( s[i] ); //取出其对应的编码串
      if ( !pCharCode ) pCharCode = table->get ( s[i] + 'A' - 'a' ); //小写字母转为大写
      if ( !pCharCode ) pCharCode = table->get ( ' ' ); //无法识别的字符统一视作空格
      printf ( "%s", *pCharCode ); //输出当前字符的编码
      for ( size_t m = strlen ( *pCharCode ), j = 0; j < m; j++ ) //将当前字符的编码接入编码串
         '1' == * ( *pCharCode + j ) ? codeString->set ( n++ ) : codeString->clear ( n++ );
   }
   printf ( "\n" ); return n;
} //二进制编码串记录于位图codeString中
