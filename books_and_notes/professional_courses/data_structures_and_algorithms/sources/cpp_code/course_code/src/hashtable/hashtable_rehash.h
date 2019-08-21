/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 重散列算法：装填因子过大时，采取“逐一取出再插入”的朴素策略，对桶数组扩容
 * 不可简单地（通过memcpy()）将原桶数组复制到新桶数组（比如前端），否则存在两个问题：
 * 1）会继承原有冲突；2）可能导致查找链在后端断裂——即便为所有扩充桶设置懒惰删除标志也无济于事
 ******************************************************************************************/
template <typename K, typename V> void Hashtable<K, V>::rehash() {
   int old_capacity = M; Entry<K, V>** old_ht = ht;
   M = primeNLT ( 2 * M, 1048576, "../../_input/prime-1048576-bitmap.txt" ); //容量至少加倍
   N = 0; ht = new Entry<K, V>*[M]; memset ( ht, 0, sizeof ( Entry<K, V>* ) * M ); //新桶数组
   release ( lazyRemoval ); lazyRemoval = new Bitmap ( M ); //新开懒惰删除标记比特图
   //*DSA*/printf("A bucket array has been created with capacity = %d\n\n", M);
   for ( int i = 0; i < old_capacity; i++ ) //扫描原桶数组
      if ( old_ht[i] ) //将非空桶中的词条逐一
         put ( old_ht[i]->key, old_ht[i]->value ); //插入至新的桶数组
   release ( old_ht ); //释放原桶数组——由于其中原先存放的词条均已转移，故只需释放桶数组本身
}
