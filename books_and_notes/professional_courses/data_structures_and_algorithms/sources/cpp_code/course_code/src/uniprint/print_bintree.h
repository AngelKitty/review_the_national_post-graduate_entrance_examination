/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 二叉树输出打印
 ******************************************************************************************/
#include "../Bitmap/Bitmap.h" //使用位图记录分支转向

#define ROOT 0
#define L_CHILD 1
#define R_CHILD -1*L_CHILD

/******************************************************************************************
 * 基础BinTree
 ******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p ( BinTree<T> & bt ) { //引用
   printf ( "%s[%d]*%d:\n", typeid ( bt ).name(), &bt, bt.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( bt.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );
}

/******************************************************************************************
 * 基于BinTree实现的BST
 ******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p ( BST<T> & bt ) { //引用
   printf ( "%s[%d]*%d:\n", typeid ( bt ).name(), &bt, bt.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( bt.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );
}

/******************************************************************************************
 * 基于BST实现的AVL
 * 其中调用的BinNode的打印例程，可以显示BF状态
 ******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p ( AVL<T> & avl ) { //引用
   printf ( "%s[%d]*%d:\n", typeid ( avl ).name(), &avl, avl.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( avl.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );
}

/******************************************************************************************
 * 基于BST实现的RedBlack
 * 其中调用的BinNode的打印例程，可以显示BF状态
 ******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p ( RedBlack<T> & rb ) { //引用
   printf ( "%s[%d]*%d:\n", typeid ( rb ).name(), &rb, rb.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( rb.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );
}

/******************************************************************************************
 * 基于BST实现的Splay
 * 鉴于Splay不必设置bf之类的附加标识，其打印例程与BST完全一致
 ******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p ( Splay<T> & bt ) { //引用
   printf ( "%s[%d]*%d:\n", typeid ( bt ).name(), &bt, bt.size() ); //基本信息
   Bitmap* branchType = new Bitmap; //记录当前节点祖先的方向
   printBinTree ( bt.root(), -1, ROOT, branchType ); //树状结构
   release ( branchType ); printf ( "\n" );
}

/******************************************************************************************
 * 二叉树各种派生类的统一打印
 ******************************************************************************************/
template <typename T> //元素类型
static void printBinTree ( BinNodePosi(T) bt, int depth, int type, Bitmap* bType ) {
   if ( !bt ) return;
   if ( -1 < depth ) //设置当前层的拐向标志
      R_CHILD == type ? bType->set ( depth ) : bType->clear ( depth );
   printBinTree ( bt->rc, depth + 1, R_CHILD, bType ); //右子树（在上）
   print ( bt ); printf ( " *" );
   for ( int i = -1; i < depth; i++ ) //根据相邻各层
      if ( ( 0 > i ) || bType->test ( i ) == bType->test ( i + 1 ) ) //的拐向是否一致，即可确定
         printf ( "      " ); //是否应该
      else  printf ( "│    " ); //打印横线
   switch ( type ) {
      case  R_CHILD  :  printf ( "┌─" );  break;
      case  L_CHILD  :  printf ( "└─" );  break;
      default        :  printf ( "──" );  break; //root
   }
   print ( bt );
#if defined(DSA_HUFFMAN)
   if ( IsLeaf ( *bt ) ) bType->print ( depth + 1 ); //输出Huffman编码
#endif
   printf ( "\n" );
   printBinTree ( bt->lc, depth + 1, L_CHILD, bType ); //左子树（在下）
}
