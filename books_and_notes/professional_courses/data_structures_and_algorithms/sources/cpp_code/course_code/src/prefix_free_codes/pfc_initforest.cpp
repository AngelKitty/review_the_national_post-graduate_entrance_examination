/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "PFC.h"

PFCForest* initForest() { //PFC编码森林初始化
   PFCForest* forest = new PFCForest; //首先创建空森林，然后
   for ( int i = 0; i < N_CHAR; i++ ) { //对每一个可打印字符[0x20, 0x80)
      forest->insert ( i, new PFCTree() ); //创建一棵对应的PFC编码树，初始时其中
      ( *forest ) [i]->insertAsRoot ( 0x20 + i ); //只包含对应的一个（叶、根）节点
   }
   return forest; //返回包含N_CHAR棵树的森林，其中每棵树各包含一个字符
}