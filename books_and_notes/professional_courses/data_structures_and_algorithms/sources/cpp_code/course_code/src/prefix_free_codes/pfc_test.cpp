/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "PFC_test.h"
int main ( int argc, char* argv[] ) { //PFC编码、解码算法统一测试入口
   /*DSA*/if ( 2 > argc ) { printf ( "Usage: %s <message#1> [message#2] ...\a\n", argv[0] ); return -1; }
   PFCForest* forest = initForest(); //初始化PFC森林
   PFCTree* tree = generateTree ( forest ); release ( forest ); //生成PFC编码树
   /*DSA*/print ( tree );
   PFCTable* table = generateTable ( tree ); //将PFC编码树转换为编码表
   /*DSA*/for ( int i = 0; i < N_CHAR; i++ ) printf ( " %c: %s\n", i + 0x20, * ( table->get ( i + 0x20 ) ) ); //输出编码表
   for ( int i = 1; i < argc; i++ ) { //对于命令行传入的每一明文串
      /*DSA*/printf ( "\nEncoding: %s\n", argv[i] ); //开始编码
      Bitmap codeString; //二进制编码串
      int n = encode ( table, codeString, argv[i] ); //将根据编码表生成（长度为n）
      /*DSA*/printf ( "%s\n", codeString.bits2string ( n ) ); //输出当前文本的编码串
      /*DSA*/printf ( "Decoding: " ); //开始解码
      decode ( tree, codeString, n ); //利用编码树，对长度为n的二进制编码串解码（直接输出）
   }
   release ( table ); release ( tree ); return 0; //释放编码表、编码树
}