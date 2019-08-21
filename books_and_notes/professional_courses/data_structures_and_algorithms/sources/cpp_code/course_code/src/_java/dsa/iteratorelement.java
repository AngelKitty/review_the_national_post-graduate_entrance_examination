/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于列表实现的元素迭代器
 */

package dsa;

public class IteratorElement implements Iterator {
   private List list;//列表
   private Position nextPosition;//当前（下一个）元素的位置

   //默认构造方法
   public IteratorElement() { list = null; }

   //构造方法
   public IteratorElement(List L) {
      list = L;
      if (list.isEmpty())//若列表为空，则
         nextPosition = null;//当前元素置空
      else//否则
         nextPosition = list.first();//从第一个元素开始
   }

   //检查迭代器中是否还有剩余的元素
   public boolean hasNext() { return (null != nextPosition); }

   //返回迭代器中的下一元素
   public Object getNext() throws ExceptionNoSuchElement {
      if (!hasNext()) throw new ExceptionNoSuchElement("意外：没有下一元素");
      Position currentPosition = nextPosition;
      if (currentPosition == list.last())//若已到达尾元素，则
         nextPosition = null;//不再有下一元素
      else//否则
         nextPosition = list.getNext(currentPosition);//转向下一元素
      return currentPosition.getElem();
   }
}