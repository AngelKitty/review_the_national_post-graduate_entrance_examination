/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../nest_stack/nest.h"

void trim ( const char exp[], int& lo, int& hi ) { //删除exp[lo, hi]不含括号的最长前缀、后缀
   while ( ( lo <= hi ) && ( exp[lo] != '(' ) && ( exp[lo] != ')' ) ) lo++; //查找第一个和
   while ( ( lo <= hi ) && ( exp[hi] != '(' ) && ( exp[hi] != ')' ) ) hi--; //最后一个括号
}

int divide ( const char exp[], int lo, int hi ) { //切分exp[lo, hi]，使exp匹配仅当子表达式匹配
   int mi = lo; int crc = 1; //crc为[lo, mi]范围内左、右括号数目之差
   while ( ( 0 < crc ) && ( ++mi < hi ) ) //逐个检查各字符，直到左、右括号数目相等，或者越界
      {  if ( exp[mi] == ')' )  crc--; if ( exp[mi] == '(' )  crc++;  } //左、右括号分别计数
   return mi; //若mi <= hi，则为合法切分点；否则，意味着局部不可能匹配
}

bool paren ( const char exp[], int lo, int hi ) { //检查表达式exp[lo, hi]是否括号匹配（递归版）
   /*DSA*/displaySubstring ( exp, lo, hi );
   trim ( exp, lo, hi ); if ( lo > hi ) return true; //清除不含括号的前缀、后缀
   if ( exp[lo] != '(' ) return false; //首字符非左括号，则必不匹配
   if ( exp[hi] != ')' ) return false; //末字符非右括号，则必不匹配
   int mi = divide ( exp, lo, hi ); //确定适当的切分点
   if ( mi > hi ) return false; //切分点不合法，意味着局部以至整体不匹配
   return paren ( exp, lo + 1, mi - 1 ) && paren ( exp, mi + 1, hi ); //分别检查左、右子表达式
}