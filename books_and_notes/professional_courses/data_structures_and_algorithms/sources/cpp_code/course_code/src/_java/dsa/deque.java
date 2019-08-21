/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 双端队列接口
 */

package dsa;

public interface Deque {
   public int getSize();//返回队列中元素数目
   public boolean isEmpty();//判断队列是否为空
   public Object first() throws ExceptionQueueEmpty;//取首元素（但不删除）
   public Object last() throws ExceptionQueueEmpty;//取末元素（但不删除）
   public void insertFirst(Object obj);//将新元素作为首元素插入
   public void insertLast(Object obj);//将新元素作为末元素插入
   public Object removeFirst() throws ExceptionQueueEmpty;//删除首元素
   public Object removeLast() throws ExceptionQueueEmpty;//删除末元素
   public void Traversal();//遍历
}