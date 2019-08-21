/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于可扩充数组的向量实现
 */

package dsa;

public class Vector_ExtArray implements Vector {
   private int N = 8;//数组的容量，可不断增加
   private int n;//向量的实际规模
   private Object A[];//对象数组

   //构造函数
   public Vector_ExtArray()   {  A = new Object[N];   n = 0; }

   //返回向量中元素数目
   public int getSize() { return n; }

   //判断向量是否为空
   public boolean isEmpty() { return (0 == n) ? true : false; }

   //取秩为r的元素
   public Object getAtRank(int r)
   throws ExceptionBoundaryViolation {
      if (0 > r || r >= n) throw new ExceptionBoundaryViolation("意外：秩越界");
      return A[r];
   }

   //将秩为r的元素替换为obj
   public Object replaceAtRank(int r, Object obj)
   throws ExceptionBoundaryViolation {
      if (0 > r || r >= n) throw new ExceptionBoundaryViolation("意外：秩越界");
      Object bak = A[r];
      A[r] = obj;
      return bak;
   }

   //插入obj，作为秩为r的元素；并返回该元素
   public Object insertAtRank(int r, Object obj)
   throws ExceptionBoundaryViolation {
      if (0 > r || r > n)  throw new ExceptionBoundaryViolation("意外：秩越界");
      if (N <= n) {//空间溢出的处理
         N *= 2;
         Object   B[] = new Object[N];//开辟一个容量加倍的数组
         for (int i = 0; i < n; i++) B[i] = A[i]; //A[]中内容复制至B[]
         A = B;//用B替换A（原A[]将被自动回收）
      }
      for (int i = n; i > r; i--) A[i] = A[i-1]; //后续元素顺次后移
      A[r] = obj;//插入
      n++;//更新当前规模
      return obj;
   }

   //删除秩为r的元素
   public Object removeAtRank(int r)
   throws ExceptionBoundaryViolation {
      if (0 > r || r >= n) throw new ExceptionBoundaryViolation("意外：秩越界");
      Object bak = A[r];
      for (int i = r; i < n - 1; i++)  A[i] = A[i+1]; //后续元素顺次前移
      n--;//更新当前规模
      return bak;
   }
}