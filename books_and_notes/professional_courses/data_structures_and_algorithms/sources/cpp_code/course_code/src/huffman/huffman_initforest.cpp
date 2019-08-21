/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "Huffman_PQ.h"

HuffForest* initForest ( int* freq ) { //根据频率统计表，为每个字符创建一棵树
   HuffForest* forest = new HuffForest; //以List实现的Huffman森林
   for ( int i = 0; i < N_CHAR; i++ ) { //为每个字符
      forest->insertAsLast ( new HuffTree ); //生成一棵树，并将字符及其频率
      forest->last()->data->insertAsRoot ( HuffChar ( 0x20 + i, freq[i] ) ); //存入其中
   }
   return forest;
}