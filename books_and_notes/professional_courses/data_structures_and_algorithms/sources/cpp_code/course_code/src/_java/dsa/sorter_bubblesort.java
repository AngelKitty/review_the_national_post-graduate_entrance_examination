/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * ∆≈›≈≈–ÚÀ„∑®
 */

package dsa;

public class Sorter_Bubblesort implements Sorter {
   private Comparator C;

   public Sorter_Bubblesort()
   {  this(new ComparatorDefault()); }

   public Sorter_Bubblesort(Comparator comp)
   {  C = comp; }

   public void sort(Sequence S) {
      int n = S.getSize();
      for (int i = 0; i < n; i++)
         for (int j = 0; j < n - i - 1; j++)
            if (0 < C.compare(S.getAtRank(j), S.getAtRank(j + 1))) {
               Object temp = S.getAtRank(j);
               S.replaceAtRank(j, S.getAtRank(j + 1));
               S.replaceAtRank(j + 1, temp);
            }
   }
}