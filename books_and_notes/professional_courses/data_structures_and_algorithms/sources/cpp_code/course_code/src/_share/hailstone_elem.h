/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> struct Hailstone { //函数对象：按照Hailstone规则转化一个T类对象
   virtual void operator() ( T& e ) { //假设T可直接做算术运算
      int step = 0; //转换所需步数
      while ( 1 != e ) { //按奇、偶逐步转换，直至为1
         ( e % 2 ) ? e = 3 * e + 1 : e /= 2;
         step++;
      }
      e = step; //返回转换所经步数
   }
};