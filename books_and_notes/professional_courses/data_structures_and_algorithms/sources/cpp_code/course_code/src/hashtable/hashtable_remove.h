/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> bool Hashtable<K, V>::remove ( K k ) { //散列表词条删除算法
   int r = probe4Hit ( k ); if ( !ht[r] ) return false; //对应词条不存在时，无法删除
   release ( ht[r] ); ht[r] = NULL; markAsRemoved ( r ); N--; return true;
   //否则释放桶中词条，设置懒惰删除标记，并更新词条总数
}