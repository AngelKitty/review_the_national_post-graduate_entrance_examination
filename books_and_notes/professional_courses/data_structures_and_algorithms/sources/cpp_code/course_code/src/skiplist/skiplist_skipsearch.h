/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * Skiplist词条查找算法（供内部调用）
 * 入口：qlist为顶层列表，p为qlist的首节点
 * 出口：若成功，p为命中关键码所属塔的顶部节点，qlist为p所属列表
 *       否则，p为所属塔的基座，该塔对应于不大于k的最大且最靠右关键码，qlist为空
 * 约定：多个词条命中时，沿四联表取最靠后者
 ******************************************************************************************/
template <typename K, typename V> bool Skiplist<K, V>::skipSearch (
   ListNode<Quadlist<Entry<K, V>>*>* &qlist, //从指定层qlist的
   QuadlistNode<Entry<K, V>>* &p, //首节点p出发
   K& k ) { //向右、向下查找目标关键码k
   while ( true ) { //在每一层
      while ( p->succ && ( p->entry.key <= k ) ) //从前向后查找
         p = p->succ; //直到出现更大的key或溢出至trailer
      p = p->pred; //此时倒回一步，即可判断是否
      if ( p->pred && ( k == p->entry.key ) ) return true; //命中
      qlist = qlist->succ; //否则转入下一层
      if ( !qlist->succ ) return false; //若已到穿透底层，则意味着失败
      p = ( p->pred ) ? p->below : qlist->data->first(); //否则转至当前塔的下一节点
   }  //课后：通过实验统计，验证关于平均查找长度的结论
}
