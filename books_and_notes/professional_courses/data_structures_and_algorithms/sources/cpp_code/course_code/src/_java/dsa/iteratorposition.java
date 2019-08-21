/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于列表实现的位置迭代器
 */

package dsa;

public class IteratorPosition implements Iterator {
   private List list;//列表
   private Position nextPosition;//当前（下一个）位置

   //默认构造方法
   public IteratorPosition() { list = null; }

   //构造方法
   public IteratorPosition(List L) {
      list = L;
      if (list.isEmpty())//若列表为空，则
         nextPosition = null;//当前位置置空
      else//否则
         nextPosition = list.first();//从第一个位置开始
   }

   //检查迭代器中是否还有剩余的位置
   public boolean hasNext() { return (nextPosition != null); }

   //返回迭代器中的下一位置
   public Object getNext() throws ExceptionNoSuchElement {
      if (!hasNext()) throw new ExceptionNoSuchElement("意外：没有下一位置");
      Position currentPosition = nextPosition;
      if (currentPosition == list.last())//若已到达尾位置，则
         nextPosition = null;//不再有下一个位置
      else//否则
         nextPosition = list.getNext(currentPosition);//转向下一位置
      return currentPosition;
   }
}