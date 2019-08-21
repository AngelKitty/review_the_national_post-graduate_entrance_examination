/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //统计二叉树子树规模
int BinTree<T>::size ( BinNodePosi(T) x ) //assert: x为二叉树中的合法位置
{  return x ? size ( x->lc ) + size ( x->RChild ) : 0;  }
