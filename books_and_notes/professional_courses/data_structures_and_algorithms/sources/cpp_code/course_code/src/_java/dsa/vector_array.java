/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于数组的向量实现
 */

package dsa;

public class Vector_Array implements Vector {
   private final int N = 1024;//数组的容量
   private int n = 0;//向量的实际规模
   private Object[] A;//对象数组

   //构造函数
   public Vector_Array() {
      A = new Object[N];
      n = 0;
   }

   //返回向量中元素数目
   public int getSize() { return n; }

   //判断向量是否为空
   public boolean isEmpty() { return (0 == n) ? true : false; }

   //取秩为r的元素
   public Object getAtRank(int r)//O(1)
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

   //插入obj，作为秩为r的元素；返回该元素
   public Object insertAtRank(int r, Object obj)
   throws ExceptionBoundaryViolation {
      if (0 > r || r > n)  throw new ExceptionBoundaryViolation("意外：秩越界");
      if (n >= N) throw new ExceptionBoundaryViolation("意外：数组溢出");
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
      for (int i = r; i < n; i++) A[i] = A[i+1]; //后续元素顺次前移
      n--;//更新当前规模
      return bak;
   }
}