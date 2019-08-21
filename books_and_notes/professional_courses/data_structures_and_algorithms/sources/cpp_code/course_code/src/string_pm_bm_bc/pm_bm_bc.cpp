/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../string_pm/string_pm_test.h"

int match ( char* P, char* T ) { //Boyer-Morre算法(简化版，只考虑Bad Character Shift)
   int* bc = buildBC ( P ); //预处理
   size_t n = strlen ( T ), i = 0; //文本串长度、与模式串首字符的对齐位置
   size_t m = strlen ( P ); //模式串长度
   while ( n >= i + m ) { //在到达最右端前，不断右移模式串（可能不止一个字符）
      int j = m - 1; //从模式串最末尾的字符开始
      while ( P[j] == T[i+j] ) //自右向左比对
         if ( 0 > --j ) break; /*DSA*/showProgress ( T, P, i, j ); getchar();
      if ( j < 0 ) //若极大匹配后缀 == 整个模式串，则说明已经完全匹配，故
         break; //返回匹配位置
      else //否则，根据BC表
         i += __max ( 1, j - bc[T[i+j]] ); //相应地移动模式串，使得T[i+j]与P[bc[T[i+j]]]对齐
   }
   delete [] bc; //销毁BC表
   return i;
}