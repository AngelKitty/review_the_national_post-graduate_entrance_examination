/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../_share/util.h"
#include "../UniPrint/print.h"
#include "../vector/vector.h"
#include "majority.h"
#include "majorityCandidate.h"
#include "majorityCheck.h"

/******************************************************************************************
 * 众数算法测试
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <n>\a\a\n", argv[0] ); return 1; }
   int n = atoi ( argv[1] );
   if ( n < 1 ) { printf ( "Invalid arguments\a\a\n" ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
// 随机生成向量
   Vector<int> A; //以整型为例
   int maj = rand() % n; printf ( "%d is highly possible to be a majority\n", maj ); //随机确定一个majEle候选
   while ( A.size() < n ) { //继续不断插入n，直至共有n个元素
      int ele = ( rand() % 100 < 50 ) ? maj : rand() % n; //49%的概率确定是maj，51%随机（可实验不同概率的效果）
      A.insert ( A.size(), ele );
   }
   print ( A ); //均匀随机置乱
// 计算众数
   majority ( A, maj ) ?
   printf ( "Majority element found as %d\n", maj ) :
   printf ( "Majority element NOT found\n" );
// 通过排序进一步验证
   A.sort(); print ( A );
// 返回
   return 0;
}