/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 映射结构接口
 */

package  dsa;

public interface Map {
   //查询映射结构当前的规模
   public int getSize();

   //判断映射结构是否为空
   public boolean isEmpty();

   //若映射中存在以key为关键码的条目，则返回该条目的数据对象；否则，返回null
   public Object get(Object key);

   //若映射中不存在以key为关键码的条目，则插入条目(key, value)并返回null
   //否则，将已有条目的数据对象替换为value，并返回原先的数据对象
   public Object put(Object key, Object value);

   //若映射中存在以key为关键码的条目，则删除之并返回其数据对象；否则，返回null
   public Object remove(Object key);

   //返回映射中所有条目的一个迭代器
   public Iterator entries();
}