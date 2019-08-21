/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 树ADT接口
 */

package dsa;

public interface Tree {
   //返回当前节点中存放的对象
   public Object getElem();
   //将对象obj存入当前节点，并返回此前的内容
   public Object setElem(Object obj);

   //返回当前节点的父节点
   public TreeLinkedList getParent();

   //返回当前节点的长子
   public TreeLinkedList getFirstChild();

   //返回当前节点的最大弟弟
   public TreeLinkedList getNextSibling();

   //返回当前节点后代元素的数目，即以当前节点为根的子树的规模
   public int getSize();

   //返回当前节点的高度
   public int getHeight();

   //返回当前节点的深度
   public int getDepth();
}