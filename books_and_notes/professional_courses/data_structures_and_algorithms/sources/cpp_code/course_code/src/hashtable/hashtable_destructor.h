/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> Hashtable<K, V>::~Hashtable() { //析构前释放桶数组及非空词条
   for ( int i = 0; i < M; i++ ) //逐一检查各桶
      if ( ht[i] ) release ( ht[i] ); //释放非空的桶
   release ( ht ); //释放桶数组
   release ( lazyRemoval ); //释放懒惰删除标记
}