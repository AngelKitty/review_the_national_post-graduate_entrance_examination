/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../List/List.h" //引入列表
#include "../Entry/Entry.h" //引入词条
#include "Quadlist.h" //引入Quadlist
#include "../Dictionary/Dictionary.h" //引入词典

template <typename K, typename V> //key、value
//符合Dictionary接口的Skiplist模板类（但隐含假设元素之间可比较大小）
class Skiplist : public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*> {
protected:
   bool skipSearch (
      ListNode<Quadlist<Entry<K, V>>*>* &qlist,
      QuadlistNode<Entry<K, V>>* &p,
      K& k );
public:
   int size() const { return empty() ? 0 : last()->data->size(); } //底层Quadlist的规模
   int level() { return List::size(); } //层高 == #Quadlist，不一定要开放
   bool put ( K, V ); //插入（注意与Map有别——Skiplist允许词条重复，故必然成功）
   V* get ( K k ); //读取
   bool remove ( K k ); //删除
};

#include "Skiplist_implementation.h"