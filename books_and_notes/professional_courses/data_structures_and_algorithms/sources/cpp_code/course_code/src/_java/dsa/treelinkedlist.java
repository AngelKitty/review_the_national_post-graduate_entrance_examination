/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于链表实现树结构
 */

package dsa;

public class TreeLinkedList implements Tree {
   private Object element;//树根节点
   private TreeLinkedList parent, firstChild, nextSibling;//父亲、长子及最大的弟弟

   //（单节点树）构造方法
   public TreeLinkedList()
   {  this(null, null, null, null); }

   //构造方法
   public TreeLinkedList(Object e, TreeLinkedList p, TreeLinkedList c, TreeLinkedList s) {
      element = e;
      parent = p;
      firstChild = c;
      nextSibling = s;
   }

   /*---------- Tree接口中各方法的实现 ----------*/
   //返回当前节点中存放的对象
   public Object getElem()
   { return element; }


   //将对象obj存入当前节点，并返回此前的内容
   public Object setElem(Object obj)
   {  Object bak = element;   element = obj; return bak; }

   //返回当前节点的父节点；对于根节点，返回null
   public TreeLinkedList getParent()
   { return parent; }

   //返回当前节点的长子；若没有孩子，则返回null
   public TreeLinkedList getFirstChild()
   { return firstChild; }

   //返回当前节点的最大弟弟；若没有弟弟，则返回null
   public TreeLinkedList getNextSibling()
   { return nextSibling; }

   //返回当前节点后代元素的数目，即以当前节点为根的子树的规模
   public int getSize() {
      int   size = 1;//当前节点也是自己的后代
      TreeLinkedList subtree = firstChild;//从长子开始
      while (null != subtree) {//依次
         size += subtree.getSize();//累加
         subtree = subtree.getNextSibling();//所有孩子的后代数目
      }
      return size;//即可得到当前节点的后代总数
   }

   //返回当前节点的高度
   public int getHeight() {
      int   height = -1;
      TreeLinkedList subtree = firstChild;//从长子开始
      while (null != subtree) {//依次
         height = Math.max(height, subtree.getHeight());//在所有孩子中取最大高度
         subtree = subtree.getNextSibling();
      }
      return height + 1; //即可得到当前节点的高度
   }

   //返回当前节点的深度
   public int getDepth() {
      int   depth = 0;
      TreeLinkedList p = parent;//从父亲开始
      while (null != p) {//依次
         depth++; p = p.getParent();//访问各个真祖先
      }
      return depth;//真祖先的数目，即为当前节点的深度
   }
}
