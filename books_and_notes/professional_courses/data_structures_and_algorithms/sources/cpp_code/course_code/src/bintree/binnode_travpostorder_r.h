/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T, typename VST> //元素类型、操作器
void travPost_R ( BinNodePosi(T) x, VST& visit ) { //二叉树后序遍历算法（递归版）
   if ( !x ) return;
   travPost_R ( x->lc, visit );
   travPost_R ( x->rc, visit );
   visit ( x->data );
}
