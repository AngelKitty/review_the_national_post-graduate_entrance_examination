/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include <stdio.h> //采用C风格精细控制输出格式
#include "../huffman/huffChar.h" //Huffman超字符
#include "../BinTree/BinTree.h" //二叉树
#include "../Huffman/HuffTree.h" //Huffman树
#include "../BST/BST.h" //二叉搜索树
#include "../AVL/AVL.h" //AVL树
#include "../Splay/Splay.h" //伸展树
#include "../redBlack/RedBlack.h" //红黑树
#include "../BTree/BTree.h" //二叉搜索树
#include "../Entry/Entry.h" //词条
#include "../Skiplist/Quadlist.h" //四叉表
#include "../Skiplist/Skiplist.h" //跳转表
#include "../Hashtable/Hashtable.h" //散列表
#include "../PQ_List/PQ_List.h" //基于列表实现的优先级队列
#include "../PQ_ComplHeap/PQ_ComplHeap.h" //基于完全堆实现的优先级队列
#include "../PQ_LeftHeap/PQ_LeftHeap.h" //基于左式堆实现的优先级队列
#include "../graph/Graph.h" //图
#include "../graphMatrix/GraphMatrix.h" //基于邻接矩阵实现的图

/******************************************************************************************
 * 数据元素、数据结构通用输出接口
 ******************************************************************************************/
template <typename T> static void print ( T* x ) {  x ? print ( *x ) : printf ( " <NULL>" );  }
template <typename T> static void print ( T& x ) {  UniPrint::p ( x );  }
template <typename T> static void print ( const T& x ) {  UniPrint::p ( x );  } //for Stack
static void print ( char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理
static void print ( const char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理

class UniPrint {
public:
   static void p ( int );
   static void p ( float );
   static void p ( double );
   static void p ( char );
   static void p ( HuffChar& ); //Huffman（超）字符
   static void p ( VStatus ); //图顶点的状态
   static void p ( EType ); //图边的类型

   template <typename K, typename V> static void p ( Entry<K, V>& ); //Entry
   template <typename T> static void p ( BinNode<T>& ); //BinTree节点
   template <typename T> static void p ( BinTree<T>& ); //二叉树
   template <typename T> static void p ( BTree<T>& ); //B-树
   template <typename T> static void p ( BST<T>& ); //BST
   template <typename T> static void p ( AVL<T>& ); //AVL
   template <typename T> static void p ( RedBlack<T>& ); //RedBlack
   template <typename T> static void p ( Splay<T>& ); //Splay
   template <typename T> static void p ( Quadlist<T>& ); //Quadlist
   template <typename K, typename V> static void p ( Skiplist<K, V>& ); //Skiplist
   template <typename K, typename V> static void p ( Hashtable<K, V>& ); //Hashtable
   template <typename T> static void p ( PQ_List<T>& ); //PQ_List
   template <typename T> static void p ( PQ_ComplHeap<T>& ); //PQ_ComplHeap
   template <typename T> static void p ( PQ_LeftHeap<T>& ); //PQ_LeftHeap
   template <typename Tv, typename Te> static void p ( GraphMatrix<Tv, Te>& ); //Graph
   template <typename T> static void p ( T& ); //向量、列表等支持traverse()遍历操作的线性结构
   template <typename T> static void p ( T* s ) //所有指针
   {  s ? p ( *s ) : print ( "<NULL>" ); } //统一转为引用
}; //UniPrint

#include "print_implementation.h"