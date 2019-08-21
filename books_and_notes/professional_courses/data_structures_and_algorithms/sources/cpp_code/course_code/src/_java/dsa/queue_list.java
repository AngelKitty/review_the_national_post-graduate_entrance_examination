/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于单链表实现队列结构
 */

package dsa;

public class Queue_List implements Queue {
   protected Node head;//指向表首元素
   protected Node tail;//指向表末元素
   protected int size;//队列中元素的数目

   //构造方法（空队列）
   public Queue_List()
   {  head = tail = null;  size = 0; }

   //查询当前队列的规模
   public int getSize()
   {  return size; }

   //判断队列是否为空
   public boolean isEmpty()
   {  return (0 == size) ? true : false; }

   //入队
   public void enqueue(Object obj) {
      Node node = new Node();
      node.setElem(obj);
      node.setNext(null);//新节点作为末节点插入
      if (0 == size) head = node;//若此前队列为空，则直接插入
      else                 tail.setNext(node);//否则，将新节点接至队列末端
      tail = node;//更新指向末节点引用
      size++;//更新规模
   }

   //出队
   public Object dequeue() throws ExceptionQueueEmpty {
      if (0 == size)
         throw new ExceptionQueueEmpty("意外：队列空");
      Object obj = head.getElem();
      head = head.getNext();
      size--;
      if (0 == size) tail = null;//若队列已空，须将末节点引用置空
      return obj;
   }

   //取（并不删除）队首元素
   public Object front() throws ExceptionQueueEmpty {
      if (isEmpty())
         throw new ExceptionQueueEmpty("意外：队列空");
      return head.getElem();
   }

   //遍历（不属于ADT）
   public void Traversal() {
      Node  p = head;
      while (null != p) {
         System.out.print(p.getElem() + " ");
         p = p.getNext();
      }
   }
}