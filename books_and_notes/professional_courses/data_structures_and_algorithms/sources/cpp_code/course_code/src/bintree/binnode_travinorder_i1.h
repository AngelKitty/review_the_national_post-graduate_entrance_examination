/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongLeftBranch ( BinNodePosi(T) x, Stack<BinNodePosi(T)>& S ) {
   while ( x ) { S.push ( x ); x = x->lc; } //当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I1 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历算法（迭代版#1）
   Stack<BinNodePosi(T)> S; //辅助栈
   while ( true ) {
      goAlongLeftBranch ( x, S ); //从当前节点出发，逐批入栈
      if ( S.empty() ) break; //直至所有节点处理完毕
      x = S.pop(); visit ( x->data ); //弹出栈顶节点并访问之
      x = x->rc; //转向右子树
   }
}
