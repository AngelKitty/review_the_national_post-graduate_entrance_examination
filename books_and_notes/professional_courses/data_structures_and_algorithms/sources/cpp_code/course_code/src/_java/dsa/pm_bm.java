/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 串模式匹配：Boyer-Moore算法
 * 若返回位置i > length(T) - length(P)，则说明失配
 * 否则，i为匹配位置
 */
import dsa.*;
import java.io.*;

public class PM_BM {
   final static int CARD_CHAR_SET = 256;//字符集规模
   //////////////////////////////////////////////////////////////////////////
   // T: 0     1     .     .     .     i     i+1   .     .     .     i+j   .     .     n-1
   //    --------------------|-------------------|------------
   // P:                               0     1     .     .     .     j     .     .
   //                                  |-------------------|
   //////////////////////////////////////////////////////////////////////////
   public static int PM(String T, String P) {
      //预处理
      int[]    BC = BuildBC(P);
      int[]    GS = BuildGS(P);
      //查找匹配
      int i = 0;//模式串相对于文本串的起始位置（初始时与文本串左对齐）
      while (T.length() - P.length() >= i) { //在到达最右端前，不断右移模式串
         int j = P.length() - 1;//从模式串最末尾的字符开始
         while (P.charAt(j) == T.charAt(i + j)) //自右向左比较
            if (0 > --j)   break;
         ShowProgress(T, P, i, j);  System.out.print("\n");
         if (0 > j)//若极大匹配后缀 == 整个模式串（说明已经完全匹配）
            break;//返回匹配位置
         else//否则
            i += MAX(GS[j], j - BC[T.charAt(i+j)]);//在位移量BC和GS之间选择大者，相应地移动模式串
      }
      return(i);
   }

   /*
    * 构造Bad Charactor Shift表BC[]
    *
    *    0                                   BC['X']                                                  m-1
    *    |                                   |                                                           |
    *    ........................X****************************************
    *                                        |<------------ 不含字符'X' ------------>|
    *
    * 复杂度 = O(m + CARD_CHAR_SET)
    ******************************************************************************************/
   protected static int[] BuildBC(String P) {
      //初始化
      int[] BC = new int[CARD_CHAR_SET];//BC[]表
      int   j;
      for (j = 0; j < CARD_CHAR_SET; j++)
         BC[j] = -1;//首先假设该字符没有在P中出现
      //自左向右迭代：更新各字符的BC[]值
      for (j = 0; j < P.length(); j++)
         BC[P.charAt(j)] = j;//P[j]曾出现在位置j——鉴于这里的扫描次序是从左到右（即下标递增），故只要某个字符ch在P中出现过，BC[ch]就会记录下其中的最靠右的出现位置
      System.out.println("-- BC[] Table ---------------");
      for (j = 0; j < CARD_CHAR_SET; j++)
         if (0 <= BC[j])   System.out.print("\t" + (char)j);
      System.out.println();
      for (j = 0; j < CARD_CHAR_SET; j++)
         if (0 <= BC[j])   System.out.print("\t" + BC[j]);
      System.out.println("\n");
      return(BC);
   }

   /*
    * 计算P的各前缀与P的各后缀的最大匹配长度
    * 对于P的每一前缀P[0..j]，SS[j] = max{s | P[j-s+1..j] = P[m-s..m-1]}
    *
    *    0                                   m-SS[j]                          m-1
    *    |                                   |                                   |
    *    ........................*************************
    *                                        |                                   |
    *                                        <-------- SS[j] -------->
    *                                        |                                   |
    *                      ............*************************..................
    *                      |                 |                                   |                          |
    *                      0                 j-SS[j]+1                        j                          m-1
    *
    * 复杂度 = O(m)
    ******************************************************************************************/
   protected static int[] ComputeSuffixSize(String P) {
      int      m = P.length();
      int[] SS = new int[m];//Suffix Size Table
      int      s, t;//子串P[s+1, ..., t]与后缀P[m+s-t, ..., m-1]匹配
      int      j;//当前字符的位置
      // 对最后一个字符而言，与之匹配的最长后缀就是整个P串，故...
      SS[m-1]  =  m;
      // 从倒数第二个字符起，自右向左扫描P，依次计算出SS[]其余各项
      s  =  m - 1;  t = m - 2;
      for (j = m - 2; j >= 0; j--) {
         if ((j > s) && (j - s > SS[(m-1-t)+j]))
            SS[j] =  SS[(m-1-t)+j];
         else {
            t = j;//与后缀匹配之子串的终点，就是当前字符
            s = MIN(s, j);//与后缀匹配之子串的起点
            while ((0 <= s) && (P.charAt(s) == P.charAt((m - 1 - t) + s)))
               s--;//似乎是二重循环，难道复杂度是平方量级？
            SS[j] = t - s; //与后缀匹配之最长子串的长度
         }
      }
      System.out.println("-- SS[] Table -------");
      for (j = 0; j < m; j++)  System.out.print("\t" + P.charAt(j));    System.out.println();
      for (j = 0; j < m; j++)  System.out.print("\t" + SS[j]);             System.out.println("\n");
      return(SS);
   }

   /*
    * 构造Good Suffix Shift表GS[]
    * 复杂度 = O(m)
    ******************************************************************************************/
   protected static int[] BuildGS(String P) {
      int      m = P.length();
      int[] SS = ComputeSuffixSize(P);//计算各字符对应的最长匹配后缀长度
      int[] GS = new int[m];//Good Suffix Index
      int      j;
      for (j = 0; j < m; j++) GS[j] = m;
      /*
       *                      i < m-j-1（失配位置）
       *                      |
       *    0                 |              m-j-1                         m-1
       *    |                 |              |                                |
       *    ............A#########***********************
       *                      |              |                                |
       *                      |              <---- Suffix Size ----><------ GS Shift ------>
       *                      |              <---- SS[j] = j+1 ----><-------- m-j-1 ------->
       *                      |              |                                |                                   |
       *                                     ***********************........................
       *                                     |                                |                                   |
       *                                     0                                j                                   m-1
       *                                     <--<--<--<--<--<--< 自右向扫描 <--<--<--<--<--<
       *
       ******************************************************************************************/
      int   i = 0;
      for (j = m - 1; j >= -1; j--) //提问：反过来（自左向右）扫描可以吗？为什么？
         if (-1 == j || j + 1 == SS[j]) //若定义SS[-1] = 0，则可统一为：if (j+1 == SS[j])
            for (; i < m - j - 1; i++) //似乎是二重循环，难道复杂度是平方量级？
               if (GS[i] == m)
                  GS[i] = m - j - 1;
      /*
       *                        m-SS[j]-1（失配位置）
       *                        |
       *    0                   |m-SS[j]              m-1
       *    |                   ||                    |
       *    ....................A**********************
       *                        ||                    |
       *                        |<--- Suffix Size ----><-- GS Shift ->
       *                        |<------- SS[j] ------><--- m-j-1 --->
       *                        ||                    |              |
       *                   .....B**********************...............
       *                   |     |                    |              |
       *                   0     j-SS[j]+1            j              m-1
       *                   >-->-->-->-->--> 自左向右扫描 --->-->-->-->
       *
       ******************************************************************************************/
      for (j = 0; j < m - 1; j++) //提问：反过来（自右向左）扫描可以吗？为什么？
         GS[m-SS[j] - 1] = m - j - 1;
      System.out.println("-- GS[] Table ---------------");
      for (j = 0; j < m; j++)  System.out.print("\t" + P.charAt(j));    System.out.println();
      for (j = 0; j < m; j++)  System.out.print("\t" + GS[j]);             System.out.println("\n");
      return(GS);
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

   protected static int MAX(int a, int b)
   {  return (a > b) ? a : b; }

   protected static int MIN(int a, int b)
   {  return (a < b) ? a : b; }
}
