/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../BST/BST.h" //基于BST实现RedBlack
template <typename T> class RedBlack : public BST<T> { //RedBlack树模板类
protected:
   void solveDoubleRed ( BinNodePosi(T) x ); //双红修正
   void solveDoubleBlack ( BinNodePosi(T) x ); //双黑修正
   int updateHeight ( BinNodePosi(T) x ); //更新节点x的高度
public:
   BinNodePosi(T) insert ( const T& e ); //插入（重写）
   bool remove ( const T& e ); //删除（重写）
// BST::search()等其余接口可直接沿用
};

#include "RedBlack_implementation.h"