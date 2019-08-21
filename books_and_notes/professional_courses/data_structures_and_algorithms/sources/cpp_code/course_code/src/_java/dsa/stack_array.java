/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 借助定长数组实现Stack接口
 */

package dsa;

public class Stack_Array implements Stack {
   public static final int CAPACITY = 1024;//数组的默认容量
   protected int capacity;//数组的实际容量
   protected Object[] S;//对象数组
   protected int top = -1;//栈顶元素的位置

   //按默认容量创建栈对象
   public Stack_Array()
   {  this(CAPACITY);   }

   //按指定容量创建栈对象
   public Stack_Array(int cap) {
      capacity = cap;
      S = new Object[capacity];
   }

   //获取栈当前的规模
   public int getSize()
   {  return (top + 1); }

   //测试栈是否为空
   public boolean isEmpty()
   {  return (top < 0); }

   //入栈
   public void push(Object obj) throws ExceptionStackFull {
      if (getSize() == capacity)
         throw new ExceptionStackFull("意外：栈溢出");
      S[++top] = obj;
   }

   //取栈顶元素
   public Object top() throws ExceptionStackEmpty {
      if (isEmpty())
         throw new ExceptionStackEmpty("意外：栈空");
      return S[top];
   }

   //出栈
   public Object pop() throws ExceptionStackEmpty {
      Object elem;
      if (isEmpty())
         throw new ExceptionStackEmpty("意外：栈空");
      elem = S[top];
      S[top--] = null;
      return elem;
   }
}