/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于秩实现的完全二叉树节点
 */

package dsa;

public class ComplBinTreeNode_Rank extends BinTreeNode implements BinTreePosition {
   private Vector T;//所属的树
   private int rank;//在所属树中的秩
   private Object element;//存放的对象

   //构造函数
   public ComplBinTreeNode_Rank (Vector t, Object obj) {
      element = obj;
      T = t;
      rank = T.getSize();
      T.insertAtRank(rank, this);
   }

   //返回当前节点中存放的对象
   public Object getElem()
   { return element; }
   //将对象obj存入当前节点，并返回此前的内容
   public Object setElem(Object obj)
   { Object bak = element;  element = obj;  return bak; }

   //判断是否有父亲（为使代码描述简洁）
   public boolean hasParent()
   { return (0 != rank) ? true : false; }
   //返回当前节点的父节点
   public BinTreePosition getParent()
   { return hasParent() ? (BinTreePosition) T.getAtRank((rank - 1) / 2) : null; }

   //判断是否有左孩子（为使代码描述简洁）
   public boolean hasLChild()
   { return (1 + rank * 2 < T.getSize()) ? true : false; }
   //返回当前节点的左孩子
   public BinTreePosition getLChild()
   { return hasLChild() ? (BinTreePosition) (T.getAtRank(1 + rank * 2)) : null; }

   //判断是否有右孩子（为使代码描述简洁）
   public boolean hasRChild()
   { return (2 + rank * 2 < T.getSize()) ? true : false; }
   //返回当前节点的右孩子
   public BinTreePosition getRChild()
   { return hasRChild() ? (BinTreePosition) (T.getAtRank(2 + rank * 2)) : null; }

   //返回当前节点后代元素的数目
   public int getSize() {
      int   size = 1;
      if (hasLChild())  size += getLChild().getSize();
      if (hasRChild())  size += getRChild().getSize();
      return size;
   }

   //返回当前节点的高度
   public int getHeight()  {
      int hL = hasLChild() ? getLChild().getHeight() : -1;
      int hR = hasRChild() ? getRChild().getHeight() : -1;
      return 1 + Math.max(hL, hR);
   }

   //返回当前节点的深度
   public int getDepth()   {
      return hasParent() ? 1 + getParent().getDepth() : 0;
   }
}
