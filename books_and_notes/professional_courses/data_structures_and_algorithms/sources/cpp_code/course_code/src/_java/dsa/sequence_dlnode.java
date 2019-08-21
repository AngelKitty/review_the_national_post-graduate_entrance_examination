/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于双向链表实现序列
 */

package dsa;

public class Sequence_DLNode extends List_DLNode implements Sequence {
   //检查秩r是否在[0, n)之间
   protected void checkRank(int r, int n)
   throws ExceptionBoundaryViolation {
      if (r < 0 || r >= n)
         throw new ExceptionBoundaryViolation("意外：非法的秩" + r + "，应该属于[0, " + n + ")");
   }

   //若0 <= r < getSize()，则返回秩为r的元素所在的位置；否则，报错--O(n)
   public Position rank2Pos(int r) throws ExceptionBoundaryViolation {
      DLNode node;
      checkRank(r, getSize());
      if (r <= getSize() / 2) { //若秩较小，则
         node = header.getNext( );//从前端开始
         for (int i = 0; i < r; i++) node = node.getNext(); //逐一扫描
      } else {//若秩较大，则
         node = trailer.getPrev();//从后端开始
         for (int i = 1; i < getSize() - r; i++)   node = node.getPrev(); //逐一扫描
      }
      return node;
   }

   //若p是序列中的合法位置，则返回存放于p处的元素的秩；否则，报错--O(n)
   public int pos2Rank(Position p) throws ExceptionPositionInvalid {
      DLNode   node = header.getNext();
      int      r = 0;
      while (node != trailer) {
         if (node == p) return(r);
         node = node.getNext();  r++;
      }
      throw new ExceptionPositionInvalid("意外：作为参数的位置不属于序列");
   }

   //取秩为r的元素--O(n)
   public Object getAtRank(int r) throws ExceptionBoundaryViolation {
      checkRank(r, getSize());
      return rank2Pos(r).getElem();
   }

   //将秩为r的元素替换为obj--O(n)
   public Object replaceAtRank(int r, Object obj) throws ExceptionBoundaryViolation {
      checkRank(r, getSize());
      return replace(rank2Pos(r), obj);
   }

   //插入obj，作为秩为r的元素--O(n)；返回该元素
   public Object insertAtRank(int r, Object obj)   throws ExceptionBoundaryViolation {
      checkRank(r, getSize() + 1);
      if (getSize() == r)  insertLast(obj);
      else                       insertBefore(rank2Pos(r), obj);
      return obj;
   }

   //删除秩为r的元素--O(n)
   public Object removeAtRank(int r) throws ExceptionBoundaryViolation {
      checkRank(r, getSize());
      return remove(rank2Pos(r));
   }
}