/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 二叉树接口
 */

package dsa;

public interface BinTree {
   //返回树根
   public BinTreePosition getRoot();

   //判断是否树空
   public boolean isEmpty();

   //返回树的规模（即树根的后代数目）
   public int getSize();

   //返回树（根）的高度
   public int getHeight();

   //前序遍历
   public Iterator elementsPreorder();

   //中序遍历
   public Iterator elementsInorder();

   //后序遍历
   public Iterator elementsPostorder();

   //层次遍历
   public Iterator elementsLevelorder();
}