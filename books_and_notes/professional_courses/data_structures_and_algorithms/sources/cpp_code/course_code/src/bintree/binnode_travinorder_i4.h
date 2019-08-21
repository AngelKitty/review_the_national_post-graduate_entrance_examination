/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T, typename VST> //元素类型、操作器
void travIn_I4 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历（迭代版#4，无需栈或标志位）
   while ( true )
      if ( HasLChild ( *x ) ) //若有左子树，则
         x = x->lc; //深入遍历左子树
      else { //否则
         visit ( x->data ); //访问当前节点，并
         while ( !HasRChild ( *x ) ) //不断地在无右分支处
            if ( ! ( x = x->succ() ) ) return; //回溯至直接后继（在没有后继的末节点处，直接退出）
            else visit ( x->data ); //访问新的当前节点
         x = x->rc; //（直至有右分支处）转向非空的右子树
      }
}