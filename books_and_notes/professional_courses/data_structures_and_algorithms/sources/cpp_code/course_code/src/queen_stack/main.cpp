/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "queen_stack.h"

#define QUEEN_MAX 20 //皇后最大数量

/******************************************************************************************
 * 全局变量
 ******************************************************************************************/
RunMode runMode; //演示模式
int nSolu = 0; //解的总数
int nCheck = 0; //尝试的总次数

/******************************************************************************************
 * n皇后（迭代版）
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   for ( int i = 1; i < argc - 1; i++ ) //检查参数
      if ( !strcmp ( "-step", argv[i] ) ) runMode = Step; //设定为单步演示模式
   int nQueen = atoi ( argv[argc-1] ); //棋盘大小 = 皇后总数
   if ( QUEEN_MAX < nQueen || 1 > nQueen ) { //检查参数
      fprintf ( stderr, "\nUsage: %s [-step] <nQueen(%d)>\n", argv[0], QUEEN_MAX );
      return -1;
   }
   placeQueens ( nQueen ); //启动查找
   cout  << nSolu << " solution(s) found after "
         << nCheck << " check(s) for "
         << nQueen << " queen(s)\n"; //输出解的总数
   return 0;
}