/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 快速排序算法
 * 针对向量和列表两种情况，createPivot()方法的具体实现不同
 * 这里只针对基于向量的序列，给出算法实现
 */

package dsa;

public class Sorter_Quicksort implements Sorter {
   private Comparator C;

   public Sorter_Quicksort()
   {  this(new ComparatorDefault()); }

   public Sorter_Quicksort(Comparator comp)
   {  C = comp; }

   public void sort(Sequence s)//入口方法
   {  qsort(s, 0, s.getSize() - 1); }

   public void qsort(Sequence S, int lo, int hi) {//Quicksort
      if (lo >= hi)  return;
      int mi = createPivot(S, lo, hi);
      qsort(S, lo, mi - 1);
      qsort(S, mi + 1, hi);
   }

   public int createPivot(Sequence S, int lo, int hi) {//确定轴点
      while (lo < hi) {
         while ((lo < hi) && (C.compare(S.getAtRank(lo), S.getAtRank(hi)) <= 0)) hi--;
         swap(S, lo, hi);
         while ((lo < hi) && (C.compare(S.getAtRank(lo), S.getAtRank(hi)) <= 0)) lo++;
         swap(S, lo, hi);
      }
      return lo;
   }

   private void swap(Sequence S, int i, int j) {//交换S[i]和S[j]
      Object temp = S.getAtRank(i);
      S.replaceAtRank(i, S.getAtRank(j));
      S.replaceAtRank(j, temp);
   }
}