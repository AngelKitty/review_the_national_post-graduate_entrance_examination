/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> V* Hashtable<K, V>::get ( K k ) //散列表词条查找算法
{  int r = probe4Hit ( k ); return ht[r] ? & ( ht[r]->value ) : NULL;  } //禁止词条的key值雷同