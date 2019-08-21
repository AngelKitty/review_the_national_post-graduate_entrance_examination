/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "Huffman_PQ.h"

// 根据编码树对长为n的Bitmap串做Huffman解码
void decode ( HuffTree* tree, Bitmap* code, int n ) {
   BinNodePosi ( HuffChar ) x = tree->root();
   for ( int i = 0; i < n; i++ ) {
      x = code->test ( i ) ? x->rc : x->lc;
      if ( IsLeaf ( *x ) ) {  printf ( "%c", x->data.ch ); x = tree->root();  }
   }
   /*DSA*/if ( x != tree->root() ) printf ( "..." ); printf ( "\n" );
} //解出的明码，在此直接打印输出；实用中可改为根据需要返回上层调用者