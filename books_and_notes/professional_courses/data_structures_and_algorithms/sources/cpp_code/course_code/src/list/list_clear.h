/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> int List<T>::clear() { //清空列表
   int oldSize = _size;
   while ( 0 < _size ) remove ( header->succ ); //反复删除首节点，直至列表变空
   return oldSize;
}