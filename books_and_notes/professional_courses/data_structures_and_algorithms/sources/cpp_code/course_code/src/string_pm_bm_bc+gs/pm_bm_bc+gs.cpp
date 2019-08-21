/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../string_pm/string_pm_test.h"

int match ( char* P, char* T ) { //Boyer-Morre算法（完全版，兼顾Bad Character与Good Suffix）
   int* bc = buildBC ( P ); int* gs = buildGS ( P ); //构造BC表和GS表
   size_t i = 0; //模式串相对于文本串的起始位置（初始时与文本串左对齐）
   while ( strlen ( T ) >= i + strlen ( P ) ) { //不断右移（距离可能不止一个字符）模式串
      int j = strlen ( P ) - 1; //从模式串最末尾的字符开始
      while ( P[j] == T[i + j] ) //自右向左比对
         if ( 0 > --j ) break; /*DSA*/showProgress ( T, P, i, j ); printf ( "\n" ); getchar();
      if ( 0 > j ) //若极大匹配后缀 == 整个模式串（说明已经完全匹配）
         break; //返回匹配位置
      else //否则，适当地移动模式串
         i += __max ( gs[j], j - bc[ T[i + j] ] ); //位移量根据BC表和GS表选择大者
   }
   delete [] gs; delete [] bc; //销毁GS表和BC表
   return i;
}