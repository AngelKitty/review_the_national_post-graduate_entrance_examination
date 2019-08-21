/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes ( ListNodePosi(T) p, int n ) { //p合法，且至少有n-1个真后继节点
   init(); //创建头、尾哨兵节点并做初始化
   while ( n-- ) { insertAsLast ( p->data ); p = p->succ; } //将起自p的n项依次作为末节点插入
}