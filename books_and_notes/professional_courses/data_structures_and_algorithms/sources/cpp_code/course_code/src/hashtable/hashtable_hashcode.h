/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

static size_t hashCode ( char c ) { return ( size_t ) c; } //字符
static size_t hashCode ( int k ) { return ( size_t ) k; } //整数以及长长整数
static size_t hashCode ( long long i ) { return ( size_t ) ( ( i >> 32 ) + ( int ) i ); }
static size_t hashCode ( char s[] ) { //生成字符串的循环移位散列码（cyclic shift hash code）
   int h = 0; //散列码
   for ( size_t n = strlen ( s ), i = 0; i < n; i++ ) //自左向右，逐个处理每一字符
      { h = ( h << 5 ) | ( h >> 27 ); h += ( int ) s[i]; } //散列码循环左移5位，再累加当前字符
   return ( size_t ) h; //如此所得的散列码，实际上可理解为近似的“多项式散列码”
} //对于英语单词，"循环左移5位"是实验统计得出的最佳值
