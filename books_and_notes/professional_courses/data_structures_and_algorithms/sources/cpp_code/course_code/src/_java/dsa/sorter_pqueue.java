/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于优先队列的排序器
 */

package dsa;

public class Sorter_PQueue implements Sorter {
   private Comparator C;

   public Sorter_PQueue()
   { this(new ComparatorDefault()); }

   public Sorter_PQueue(Comparator comp)
   { C = comp; }

   public void sort(Sequence S) {
      Sequence T = new Sequence_DLNode();//为批处理建立优先队列而准备的序列
      while (!S.isEmpty()) {//构建序列T
         Object e = S.removeFirst();//逐一取出S中各元素
         T.insertLast(new EntryDefault(e, e));//用节点元素本身作为关键码
      }
      //    PQueue pq = new PQueue_UnsortedList(C, T);
      //    PQueue pq = new PQueue_SortedList(C, T);
      PQueue pq = new PQueue_Heap(C, T);
      while(!pq.isEmpty()) {//从优先队列中不断地
         S.insertLast(pq.delMin().getValue());//取出最小元素，插至序列末尾
         System.out.println("\t:\t" + S.last().getElem());
      }
   }
}