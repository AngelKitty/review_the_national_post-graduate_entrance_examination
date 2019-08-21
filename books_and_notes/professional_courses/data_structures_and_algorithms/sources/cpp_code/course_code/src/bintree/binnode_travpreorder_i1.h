/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T, typename VST> //元素类型、操作器
void travPre_I1 ( BinNodePosi(T) x, VST& visit ) { //二叉树先序遍历算法（迭代版#1）
   Stack<BinNodePosi(T)> S; //辅助栈
   if ( x ) S.push ( x ); //根节点入栈
   while ( !S.empty() ) { //在栈变空之前反复循环
      x = S.pop(); visit ( x->data ); //弹出并访问当前节点，其非空孩子的入栈次序为先右后左
      if ( HasRChild ( *x ) ) S.push ( x->rc ); if ( HasLChild ( *x ) ) S.push ( x->lc );
   }
}