/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 列表ADT接口
 */

package dsa;

public interface List {
   //查询列表当前的规模
   public int getSize();

   //判断列表是否为空
   public boolean isEmpty();

   //返回第一个元素（的位置）
   public Position first();

   //返回最后一个元素（的位置）
   public Position last();

   //返回紧接给定位置之后的元素（的位置）
   public Position getNext(Position p)
   throws ExceptionPositionInvalid, ExceptionBoundaryViolation;

   //返回紧靠给定位置之前的元素（的位置）
   public Position getPrev(Position p)
   throws ExceptionPositionInvalid, ExceptionBoundaryViolation;

   //将e作为第一个元素插入列表
   public Position insertFirst(Object e);

   //将e作为最后一个元素插入列表
   public Position insertLast(Object e);

   //将e插入至紧接给定位置之后的位置
   public Position insertAfter(Position p, Object e)
   throws ExceptionPositionInvalid;

   //将e插入至紧靠给定位置之前的位置
   public Position insertBefore(Position p, Object e)
   throws ExceptionPositionInvalid;

   //删除给定位置处的元素，并返回之
   public Object remove(Position p)
   throws ExceptionPositionInvalid;

   //删除首元素，并返回之
   public Object removeFirst();

   //删除末元素，并返回之
   public Object removeLast();

   //将处于给定位置的元素替换为新元素，并返回被替换的元素
   public Object replace(Position p, Object e)
   throws ExceptionPositionInvalid;

   //位置迭代器
   public Iterator positions();

   //元素迭代器
   public Iterator elements();
}