/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于有序（非升）列表实现的优先队列
 */

package dsa;

public class PQueue_SortedList implements PQueue {
   private List L;
   private Comparator C;

   //构造方法（使用默认比较器）
   public PQueue_SortedList()
   { this(new ComparatorDefault(), null); }

   //构造方法（使用指定比较器）
   public PQueue_SortedList(Comparator c)
   { this(c, null); }

   //构造方法（使用指定初始元素）
   public PQueue_SortedList(Sequence s)
   { this(new ComparatorDefault(), s); }

   //构造方法（使用指定比较器和初始元素）
   public PQueue_SortedList(Comparator c, Sequence s) {
      L = new List_DLNode();
      C = c;
      if (null != s)
         while (!s.isEmpty()) {
            Entry e = (Entry)s.removeFirst();
            insert(e.getKey(), e.getValue());
         }
   }

   //统计优先队列的规模
   public int getSize()
   { return L.getSize(); }

   //判断优先队列是否为空
   public boolean isEmpty()
   { return L.isEmpty(); }

   //若Q非空，则返回其中的最小条目（并不删除）;否则，报错
   public Entry getMin() throws ExceptionPQueueEmpty {
      if (L.isEmpty())
         throw new ExceptionPQueueEmpty("意外：优先队列空");
      return (Entry) L.last();
   }

   //将对象obj与关键码k合成一个条目，将其插入Q中，并返回该条目
   public Entry insert(Object key, Object obj) throws ExceptionKeyInvalid {
      Entry entry = new EntryDefault(key, obj);//创建一个新条目
      if (L.isEmpty()//若优先队列为空
      || (0 > C.compare(((Entry) (L.first().getElem())).getKey(), entry.getKey())))
         //或新条目是当前最大者
         L.insertFirst(entry);//则直接插入至表头
      else {//否则
         Position curPos = L.last();//从尾条目开始
         while (0 > C.compare(((Entry)(curPos.getElem())).getKey(), entry.getKey()))
            curPos = L.getPrev(curPos);//不断前移，直到第一个不小于entry的条目
         L.insertAfter(curPos, entry);//紧接该条目之后插入entry
      }
      return(entry);
   }

   //若Q非空，则从其中摘除关键码最小的条目，并返回该条目；否则，报错
   public Entry delMin() throws ExceptionPQueueEmpty {
      if (L.isEmpty())
         throw new ExceptionPQueueEmpty("意外：优先队列空");
      return (Entry) L.remove(L.last());
   }
}