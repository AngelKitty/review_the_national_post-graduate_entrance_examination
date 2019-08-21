/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> bool Hashtable<K, V>::put ( K k, V v ) { //散列表词条插入
   if ( ht[probe4Hit ( k ) ] ) return false; //雷同元素不必重复插入
   int r = probe4Free ( k ); //为新词条找个空桶（只要装填因子控制得当，必然成功）
   ht[r] = new Entry<K, V> ( k, v ); ++N; //插入（注意：懒惰删除标记无需复位）
   if ( N * 2 > M ) rehash(); //装填因子高于50%后重散列
   return true;
}