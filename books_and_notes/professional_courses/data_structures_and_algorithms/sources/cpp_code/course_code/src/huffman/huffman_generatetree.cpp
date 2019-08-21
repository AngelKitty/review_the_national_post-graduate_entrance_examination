/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "huffman_PQ.h"

HuffTree* minHChar ( HuffForest* forest ) { //在Huffman森林中找出权重最小的（超）字符
   ListNodePosi ( HuffTree* ) p = forest->first(); //从首节点出发查找
   ListNodePosi ( HuffTree* ) minChar = p; //最小Huffman树所在的节点位置
   int minWeight = p->data->root()->data.weight; //目前的最小权重
   while ( forest->valid ( p = p->succ ) ) //遍历所有节点
      if ( minWeight > p->data->root()->data.weight ) //若当前节点所含树更小，则
         {  minWeight = p->data->root()->data.weight; minChar = p;  } //更新记录
   return forest->remove ( minChar ); //将挑选出的Huffman树从森林中摘除，并返回
}

HuffTree* generateTree ( HuffForest* forest ) { //Huffman编码算法
   while ( 1 < forest->size() ) {
      HuffTree* T1 = minHChar ( forest ); HuffTree* T2 = minHChar ( forest );
      HuffTree* S = new HuffTree(); /*DSA*/printf ( "\n################\nMerging " ); print ( T1->root()->data ); printf ( " with " ); print ( T2->root()->data ); printf ( " ...\n" );
      S->insertAsRoot ( HuffChar ( '^', T1->root()->data.weight + T2->root()->data.weight ) );
      S->attachAsLC ( S->root(), T1 ); S->attachAsRC ( S->root(), T2 );
      forest->insertAsLast ( S ); /*DSA*/ //print(forest);
   } //assert: 循环结束时，森林中唯一（列表首节点中）的那棵树即Huffman编码树
   return forest->first()->data;
}