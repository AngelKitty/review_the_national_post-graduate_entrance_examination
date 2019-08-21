/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （无序）词典结构接口
 */

package  dsa;

public interface Dictionary {
   //查询词典结构当前的规模
   public int getSize();

   //判断词典结构是否为空
   public boolean isEmpty();

   //若词典中存在以key为关键码的条目，则返回其中的一个条目；否则，返回null
   public Entry find(Object key);

   //返回由关键码为key的条目组成的迭代器
   public Iterator findAll(Object key);

   //插入条目(key, value)，并返回该条目
   public Entry insert(Object key, Object value);

   //若词典中存在以key为关键码的条目，则将摘除其中的一个并返回；否则，返回null
   public Entry remove(Object key);

   //返回词典中所有条目的一个迭代器
   public Iterator entries();
}