/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 串模式匹配：KMP算法
 * 若返回位置i > length(T) - length(P)，则说明失配
 * 否则，i为匹配位置
 */
import dsa.*;
import java.io.*;

public class PM_KMP {
   //////////////////////////////////////////////////////////////////////////
   // T: 0     1     .     .     .     i     i+1   .     .     .     i+k   .     .     n-1
   //    --------------------|-------------------|------------
   // P:                               j     j+1   .     .     .     j+k   .     .
   //                                  |-------------------|
   //////////////////////////////////////////////////////////////////////////
   public static int PM(String T, String P) {//KMP算法
      int[]    next = BuildNextImproved(P);//构造next[]表
      int      i = 0;//文本串指针
      int      j = 0;//模式串指针
      while(j < P.length() && i < T.length()) { //自左向右逐个比较字符
         ShowProgress(T, P, i - j, j);      ShowNextTable(next, i - j, P.length());  System.out.println();
         if (0 > j || T.charAt(i) == P.charAt(j)) { //若匹配，或P已移出最左侧（提问：这两个条件能否交换次序？）
            i++;  j++;//则转到下一对字符
         } else//否则
            j = next[j];//模式串右移（注意：文本串不用回退）
      }//while
      return(i - j);
   }

   protected static int[] BuildNext(String   P) {//建立模式串P的next[]表
      int[]    next = new int[P.length()];//next[]表
      int      j = 0;//“主”串指针
      int      t = next[0] = -1;//“模式”串指针
      while (j < P.length() - 1)
         if (0 > t || P.charAt(j) == P.charAt(t)) {//匹配
            j++;  t++;
            next[j] = t;//此句可以改进...
         } else//失配
            t = next[t];
      for (j = 0; j < P.length(); j++)  System.out.print("\t" + P.charAt(j)); System.out.print("\n");
      ShowNextTable(next, 0, P.length());
      return(next);
   }

   protected static int[] BuildNextImproved(String P) {//建立模式串P的next[]表（改进版本）
      int[]    next = new int[P.length()];;//next[]表
      int      j = 0;//“主”串指针
      int      t = next[0] = -1;//“模式”串指针
      while (j < P.length() - 1)
         if (0 > t || P.charAt(j) == P.charAt(t)) {//匹配
            j++;  t++;
            next[j] = (P.charAt(j) != P.charAt(t)) ? t : next[t];//注意此句与未改进之前的区别
         } else//失配
            t = next[t];
      for (j = 0; j < P.length(); j++)  System.out.print("\t" + P.charAt(j)); System.out.print("\n");
      ShowNextTable(next, 0, P.length());
      return(next);
   }


   protected static void ShowNextTable(//显示next[]表，供演示分析
      int[] N,
      int      offset,
      int      length) {
      int      i;
      for (i = 0; i < offset; i++)   System.out.print("\t");
      for (i = 0; i < length; i++)   System.out.print("\t" + N[i]);  System.out.print("\n\n");
   }

   protected static void ShowProgress(//动态显示匹配进展
      String   T,//文本串
      String   P,//模式串
      int         i,//模式串相对于文本串的起始位置
      int         j) { //模式串的当前字符
      int         t;
      System.out.println("-------------------------------------------");
      for (t = 0; t < T.length(); t++)  System.out.print("\t" + T.charAt(t)); System.out.print("\n");
      if (0 <= i + j) {
         for (t = 0; t < i + j; t++)  System.out.print("\t");
         System.out.print("\t|");
      }
      System.out.println();
      for (t = 0; t < i; t++) System.out.print("\t");
      for (t = 0; t < P.length(); t++)  System.out.print("\t" + P.charAt(t)); System.out.print("\n");
      System.out.println();
   }
}
