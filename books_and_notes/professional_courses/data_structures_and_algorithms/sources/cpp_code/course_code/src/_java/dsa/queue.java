/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 队列接口
 */

package dsa;

public interface Queue {
   public int getSize();//返回队列中元素数目
   public boolean isEmpty();//判断队列是否为空
   public Object front()//取队首元素（但不删除）
   throws ExceptionQueueEmpty;
   public void enqueue (Object obj)
   throws ExceptionQueueFull;//入队
   public Object dequeue()//出队
   throws ExceptionQueueEmpty;
   public void Traversal();//遍历
}