/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于链表实现的BST节点类
 */

package dsa;

public class BSTreeNode extends BinTreeNode implements BinTreePosition, Entry {
   /**************************** 构造方法 ****************************/
   public BSTreeNode()
   { super(); }

   public BSTreeNode(
      Object e,//节点内容
      BinTreePosition p,//父节点
      boolean asLChild,//是否作为父节点的左孩子
      BinTreePosition l,//左孩子
      BinTreePosition r)//右孩子
   {  super(e, p, asLChild, l, r); }

   /**************************** 实现Entry接口的方法 ****************************/
   //返回当前节点的关键码
   public Object getKey()
   { return ((Entry)getElem()).getKey(); }

   //修改条目的关键码，返回此前存放的关键码
   public Object setKey(Object k)
   { return ((Entry)getElem()).setKey(k); }

   //取条目的数据对象
   public Object getValue()
   { return ((Entry)getElem()).getValue(); }

   //修改条目的数据对象，返回此前存放的数据对象
   public Object setValue(Object v)
   { return ((Entry)getElem()).setValue(v); }
}