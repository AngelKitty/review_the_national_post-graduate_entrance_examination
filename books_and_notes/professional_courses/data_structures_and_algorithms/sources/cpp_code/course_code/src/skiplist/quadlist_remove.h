/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //删除Quadlist内位置p处的节点，返回其中存放的词条
T Quadlist<T>::remove ( QlistNodePosi(T) p ) { //assert: p为Quadlist中的合法位置
   p->pred->succ = p->succ; p->succ->pred = p->pred; _size--;//摘除节点
   T e = p->entry; delete p; //备份词条，释放节点
   return e; //返回词条
}

template <typename T> int Quadlist<T>::clear() { //清空Quadlist
   int oldSize = _size;
   while ( 0 < _size ) remove ( header->succ ); //逐个删除所有节点
   return oldSize;
}