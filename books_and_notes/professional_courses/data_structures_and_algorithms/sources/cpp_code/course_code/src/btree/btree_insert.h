/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> bool BTree<T>::insert ( const T& e ) { //将关键码e插入B树中
   BTNodePosi(T) v = search ( e ); if ( v ) return false; //确认目标节点不存在
   Rank r = _hot->key.search ( e ); //在节点_hot的有序关键码向量中查找合适的插入位置
   _hot->key.insert ( r + 1, e ); //将新关键码插至对应的位置
   _hot->child.insert ( r + 2, NULL ); //创建一个空子树指针
   _size++; //更新全树规模
   solveOverflow ( _hot ); //如有必要，需做分裂
   return true; //插入成功
}
