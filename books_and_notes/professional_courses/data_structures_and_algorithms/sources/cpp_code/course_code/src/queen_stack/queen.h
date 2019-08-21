/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

struct Queen { //皇后类
   int x, y; //皇后在棋盘上的位置坐标
   Queen ( int xx = 0, int yy = 0 ) : x ( xx ), y ( yy ) {};
   bool operator== ( Queen const& q ) const { //重载判等操作符，以检测不同皇后之间可能的冲突
      return    ( x == q.x ) //行冲突（这一情况其实并不会发生，可省略）
                || ( y == q.y ) //列冲突
                || ( x + y == q.x + q.y ) //沿正对角线冲突
                || ( x - y == q.x - q.y ); //沿反对角线冲突
   }
   bool operator!= ( Queen const& q ) const { return ! ( *this == q ); } //重载不等操作符 /*DSA*/可否写成：return *this != q？
};