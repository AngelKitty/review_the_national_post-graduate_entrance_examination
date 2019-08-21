/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "Huffman_PQ.h"

/******************************************************************************************
 * Huffman树构造算法：对传入的Huffman森林forest逐步合并，直到成为一棵树
 ******************************************************************************************
 * forest基于优先级队列实现，此算法适用于符合PQ接口的任何实现方式
 * 为Huffman_PQ_List、Huffman_PQ_ComplHeap和Huffman_PQ_LeftHeap共用
 * 编译前对应工程只需设置相应标志：DSA_PQ_List、DSA_PQ_ComplHeap或DSA_PQ_LeftHeap
 ******************************************************************************************/
HuffTree* generateTree ( HuffForest* forest ) {
   while ( 1 < forest->size() ) {
      HuffTree* s1 = forest->delMax(); HuffTree* s2 = forest->delMax();
      HuffTree* s = new HuffTree();
      /*DSA*/printf ( "Merging " ); print ( s1->root()->data ); printf ( " with " ); print ( s2->root()->data ); printf ( " ...\n" );
      s->insertAsRoot ( HuffChar ( '^', s1->root()->data.weight + s2->root()->data.weight ) );
      s->attachAsLC ( s->root(), s1 ); s->attachAsRC ( s->root(), s2 );
      forest->insert ( s ); //将合并后的Huffman树插回Huffman森林
   }
   HuffTree* tree = forest->delMax(); //至此，森林中的最后一棵树
   return tree; //即全局Huffman编码树
}