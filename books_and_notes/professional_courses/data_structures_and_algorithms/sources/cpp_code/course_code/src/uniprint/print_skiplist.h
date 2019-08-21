/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * Skiplist
 ******************************************************************************************/

template <typename K, typename V> //e、value
void UniPrint::p ( Skiplist<K, V>& s ) { //引用
   printf ( "%s[%d]*%d*%d:\n", typeid ( s ).name(), &s, s.level(), s.size() ); //基本信息
   s.traverse ( print ); //通过print()遍历输出所有元素
   printf ( "\n" );
}
