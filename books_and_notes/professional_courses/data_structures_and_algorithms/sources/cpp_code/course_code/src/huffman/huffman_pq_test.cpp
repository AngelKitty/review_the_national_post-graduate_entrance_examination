/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "Huffman_PQ_test.h"

/******************************************************************************************
 * 无论编码森林由列表、完全堆还是左式堆实现，本测试过程都可适用
 * 编码森林的实现方式采用优先级队列时，编译前对应的工程只需设置相应标志：
 *    DSA_PQ_List、DSA_PQ_ComplHeap或DSA_PQ_LeftHeap
 ******************************************************************************************/
int main ( int argc, char* argv[] ) { //Huffman编码算法统一测试
   /*DSA*/if ( 3 > argc ) { printf ( "Usage: %s <sample-text-file> <message#1> [message#2] ...\a\n", argv[0] ); return -1; }
   int* freq = statistics ( argv[1] ); //根据样本文件，统计各字符的出现频率
   HuffForest* forest = initForest ( freq ); release ( freq ); //创建Huffman森林
   HuffTree* tree = generateTree ( forest ); release ( forest ); //生成Huffman编码树
   /*DSA*/print ( tree ); //输出编码树
   HuffTable* table = generateTable ( tree ); //将Huffman编码树转换为编码表
   /*DSA*/for ( int i = 0; i < N_CHAR; i++ ) //输出编码表
      /*DSA*/printf ( " %c: %s\n", i + 0x20, * ( table->get ( i + 0x20 ) ) );
   for ( int i = 2; i < argc; i++ ) { //对于命令行传入的每一明文串
      /*DSA*/printf ( "\nEncoding: %s\n", argv[i] ); //以下测试编码
      Bitmap* codeString = new Bitmap; //二进制编码串
      int n = encode ( table, codeString, argv[i] ); //将根据编码表生成（长度为n）
      /*DSA*/printf ( "%s\n", codeString->bits2string ( n ) ); //输出该编码串
      /*DSA*/printf ( "Decoding: " ); //以下测试解码
      decode ( tree, codeString, n ); //利用Huffman编码树，对长度为n的二进制编码串解码
      release ( codeString );
   }
   release ( table ); release ( tree ); return 0; //释放编码表、编码树
}