/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "hanoi.h"
// 按照Hanoi规则，将柱子Sx上的n只盘子，借助柱子Sy中转，移到柱子Sz上
void hanoi ( int n, Stack<Disk>& Sx, Stack<Disk>& Sy, Stack<Disk>& Sz ) {
   if ( n > 0 )   { //没有盘子剩余时，不再递归
      hanoi ( n - 1, Sx, Sz, Sy ); //递归：将Sx上的n - 1只盘子，借助Sz中转，移到Sy上
      move ( Sx, Sz ); //直接：将Sx上最后一只盘子，移到Sz上
      hanoi ( n - 1, Sy, Sx, Sz ); //递归：将Sy上的n - 1只盘子，借助Sx中转，移到Sz上
   }
}
