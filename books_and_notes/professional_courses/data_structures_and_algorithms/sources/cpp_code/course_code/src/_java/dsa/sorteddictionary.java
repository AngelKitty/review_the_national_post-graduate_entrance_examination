/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 有序词典接口
 */

package dsa;

public interface SortedDictionary extends Dictionary {
   //若词典非空，则返回其中关键码最小的条目；否则，返回null
   public Entry first();

   //若词典非空，则返回其中关键码最大的条目；否则，返回null
   public Entry last();

   //返回由关键码不小于key的条目依非降序组成的迭代器
   public Iterator successors(Object key);

   //返回由关键码不大于key的条目依非升序组成的迭代器
   public Iterator predecessors(Object key);
}