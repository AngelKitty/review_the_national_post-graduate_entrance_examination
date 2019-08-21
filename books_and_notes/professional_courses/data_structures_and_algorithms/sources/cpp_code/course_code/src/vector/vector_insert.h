/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //将e作为秩为r元素插入
Rank Vector<T>::insert ( Rank r, T const& e ) { //assert: 0 <= r <= size
   expand(); //若有必要，扩容
   for ( int i = _size; i > r; i-- ) _elem[i] = _elem[i-1]; //自后向前，后继元素顺次后移一个单元
   _elem[r] = e; _size++; //置入新元素并更新容量
   return r; //返回秩
}