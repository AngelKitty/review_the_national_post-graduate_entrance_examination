/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> bool Skiplist<K, V>::remove ( K k ) { //跳转表词条删除算法
   if ( empty() ) return false; //空表情况
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层Quadlist的
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点开始
   if ( !skipSearch ( qlist, p, k ) ) return false; //目标词条不存在，直接返回
   do { //若目标词条存在，则逐层拆除与之对应的塔
      QuadlistNode<Entry<K, V>>* lower = p->below; //记住下一层节点，并
      qlist->data->remove ( p ); //删除当前层节点，再
      p = lower; qlist = qlist->succ; //转入下一层
   } while ( qlist->succ ); //如上不断重复，直到塔基
   while ( !empty() && first()->data->empty() ) //逐一地
      List::remove ( first() ); //清除已可能不含词条的顶层Quadlist
   return true; //删除操作成功完成
}