/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 借助定长循环数组实现Queue接口
 */

package dsa;

public class Queue_Array implements Queue {
   public static final int CAPACITY = 1000;//数组的默认容量
   protected int capacity;//数组的实际容量
   protected Object[] Q;//对象数组
   protected int f = 0;//队首元素的位置
   protected int r = 0; //队尾元素的位置

   //构造方法（空队列）
   public Queue_Array()
   {  this(CAPACITY);   }

   //按指定容量创建对象
   public Queue_Array(int cap)
   {  capacity = cap;   Q = new Object[capacity];  }

   //查询当前队列的规模
   public int getSize()
   {  return (capacity - f + r) % capacity;   }

   //判断队列是否为空
   public boolean isEmpty()
   {  return (f == r);  }

   //入队
   public void enqueue(Object obj) throws ExceptionQueueFull {
      if (getSize() == capacity - 1)
         throw new ExceptionQueueFull("Queue overflow.");
      Q[r] = obj;
      r = (r + 1) % capacity;
   }

   //出队
   public Object dequeue() {
      Object elem;
      if (isEmpty())
         throw new ExceptionQueueEmpty("意外：队列空");
      elem = Q[f];
      Q[f] = null;
      f = (f + 1) % capacity;
      return elem;
   }

   //取（并不删除）队首元素
   public Object front() throws ExceptionQueueEmpty {
      if (isEmpty())
         throw new ExceptionQueueEmpty("意外：队列空");
      return Q[f];
   }

   //遍历（不属于ADT）
   public void Traversal() {
      for (int i = f; i < r; i++)
         System.out.print(Q[i] + " ");
      System.out.println();
   }
}