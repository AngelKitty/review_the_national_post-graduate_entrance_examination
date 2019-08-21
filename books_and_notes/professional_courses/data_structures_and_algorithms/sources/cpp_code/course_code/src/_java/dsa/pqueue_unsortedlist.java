/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于无序列表实现的优先队列
 */

package dsa;

public class PQueue_UnsortedList implements PQueue {
   private List L;
   private Comparator C;

   //构造方法（使用默认比较器）
   public PQueue_UnsortedList()
   { this(new ComparatorDefault(), null); }

   //构造方法（使用指定比较器）
   public PQueue_UnsortedList(Comparator c)
   { this(c, null); }

   //构造方法（使用指定初始元素）
   public PQueue_UnsortedList(Sequence s)
   { this(new ComparatorDefault(), s); }

   //构造方法（使用指定比较器和初始元素）
   public PQueue_UnsortedList(Comparator c, Sequence s) {
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
      Position minPos = L.first();
      Position curPos = L.getNext(minPos);
      while (null != curPos)//依次检查所有位置，找出最小条目
         if (0 < C.compare(minPos.getElem(), curPos.getElem()))
            minPos = curPos;
      return (Entry) minPos.getElem();
   }

   //将对象obj与关键码k合成一个条目，将其插入Q中，并返回该条目
   public Entry insert(Object key, Object obj) throws ExceptionKeyInvalid {
      Entry entry = new EntryDefault(key, obj);//创建一个新条目
      L.insertLast(entry);//接至列表末尾
      return(entry);
   }

   //若Q非空，则从其中摘除关键码最小的条目，并返回该条目；否则，报错
   public Entry delMin() throws ExceptionPQueueEmpty {
      if (L.isEmpty())
         throw new ExceptionPQueueEmpty("意外：优先队列空");
      Position minPos = L.first();
      Iterator it = L.positions();
      while (it.hasNext()) {//依次检查所有位置，找出最小条目
         Position curPos = (Position) (it.getNext());
         //       System.out.println("\t" + ((Entry)(curPos.getElem())).getKey());
         if (0 <  C.compare(
            ((Entry)(minPos.getElem())).getKey(),
            ((Entry)(curPos.getElem())).getKey())
            )
            minPos = curPos;
      }
      return (Entry) L.remove(minPos);
   }
}