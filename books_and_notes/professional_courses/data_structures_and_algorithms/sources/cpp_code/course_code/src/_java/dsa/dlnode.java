/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于位置接口实现的双向链表节点类
 */

package dsa;

public class DLNode implements Position {
   private Object element;//数据对象
   private DLNode prev;//指向前驱节点
   private DLNode next;//指向后继节点

   /**************************** 构造函数 ****************************/
   public DLNode()
   { this(null, null, null); }

   public DLNode(Object e, DLNode p, DLNode n)
   { element = e; prev = p; next = n; }
   //注意三个参数的次序：数据对象、前驱节点、后继节点

   /**************************** Position接口方法 ****************************/
   //返回存放于该位置的元素
   public Object getElem()
   { return element; }

   //将给定元素存放至该位置，返回此前存放的元素
   public Object setElem(Object e)
   { Object oldElem = element;   element = e;   return oldElem; }

   /**************************** 双向链表节点方法 ****************************/
   //找到后继位置
   public DLNode getNext()
   { return next; }

   //找到前驱位置
   public DLNode getPrev()
   { return prev; }

   //修改后继位置
   public void setNext(DLNode newNext)
   { next = newNext; }

   //修改前驱位置
   public void setPrev(DLNode newPrev)
   { prev = newPrev; }
}