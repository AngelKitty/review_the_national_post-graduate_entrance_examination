/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include   "../queen_brute_force_1/queen.h"

/******************************************************************************************
 * n皇后算法（递归版）
 * 初始启动：placeQueens(nQueen, 0);
 ******************************************************************************************/
void placeQueens ( int nQueen, int k ) { //放置n个皇后中的第k个（假设0至k-1均已就位）
   static int solu[QUEEN_MAX]; //解法
   if ( nQueen <= k ) { //若所有皇后都已就位，则
      nSolu++; displaySolution ( solu, nQueen ); //输出可行解
   } else //否则
      for ( int i = 0; i < nQueen; i++ ) { //依次
         solu[k] = i; //试着将当前皇后放在（当前行的）各列上
         if ( !collide ( solu, k ) ) //若没有冲突，则
            placeQueens ( nQueen, k + 1 ); //进而考虑下一皇后
      } //思考：这里的回溯，通过什么机制实现？
}