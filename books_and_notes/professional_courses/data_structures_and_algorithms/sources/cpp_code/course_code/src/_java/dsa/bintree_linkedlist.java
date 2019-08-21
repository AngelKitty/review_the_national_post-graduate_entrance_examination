/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于链表实现二叉树
 */

package dsa;

public class BinTree_LinkedList implements BinTree {
   protected BinTreePosition root;//根节点

   /**************************** 构造函数 ****************************/
   public BinTree_LinkedList()
   { this(null); }

   public BinTree_LinkedList(BinTreePosition r)
   { root = r; }

   /**************************** BinaryTree接口方法 ****************************/
   //返回树根
   public BinTreePosition getRoot()
   { return root; }

   //判断是否树空
   public boolean isEmpty()
   { return null == root; }

   //返回树的规模（即树根的后代数目）
   public int getSize()
   { return isEmpty() ? 0 : root.getSize(); }

   //返回树（根）的高度
   public int getHeight()
   {return isEmpty() ? -1 : root.getHeight(); }

   //前序遍历
   public Iterator elementsPreorder()
   { return root.elementsPreorder(); }

   //中序遍历
   public Iterator elementsInorder()
   { return root.elementsInorder(); }

   //后序遍历
   public Iterator elementsPostorder()
   { return root.elementsPostorder(); }

   //层次遍历
   public Iterator elementsLevelorder()
   { return root.elementsLevelorder(); }
}