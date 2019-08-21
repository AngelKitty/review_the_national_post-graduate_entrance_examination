/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 归并排序算法
 * 针对向量和列表两种情况，merge()方法的具体实现不同
 * 这里只针对基于列表的序列，给出算法实现
 */

package dsa;

public class Sorter_Mergesort implements Sorter {
   protected Comparator C;

   public Sorter_Mergesort()
   {  this(new ComparatorDefault()); }

   public Sorter_Mergesort(Comparator comp)
   {  C = comp; }

   public void sort(Sequence S) {//Mergesort
      int n = S.getSize();
      if (1 >= n)   return; //递归基
      Sequence S1 = new Sequence_DLNode();
      Sequence S2 = new Sequence_DLNode();
      while (!S.isEmpty()) {//将S均匀地分成两个子序列S1和S2
         S1.insertLast(S.remove(S.first()));
         if (!S.isEmpty()) S2.insertLast(S.remove(S.first()));
      }
      sort(S1);   sort(S2);
      merge(S, S1, S2);
   }

   public void merge(Sequence S, Sequence S1, Sequence S2) {//有序列表的归并算法
      while (!S1.isEmpty() || !S2.isEmpty()) {
         Object e;
         //在两个子列表变为空之前，不断地摘出两个首元素中的小者e
         if (S1.isEmpty())
            e = S2.remove(S2.first());
         else if (S2.isEmpty())
            e = S1.remove(S1.first());
         else if (0 < C.compare(S1.first().getElem(), S2.first().getElem()))
            e = S2.remove(S2.first());
         else
            e = S1.remove(S1.first());
         //将该元素插至S的尾部
         S.insertLast(e);
      }//while
   }
}