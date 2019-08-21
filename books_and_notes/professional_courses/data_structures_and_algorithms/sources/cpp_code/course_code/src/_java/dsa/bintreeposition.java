/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 二叉树节点ADT接口
 */

package dsa;

public interface BinTreePosition extends Position {
   //判断是否有父亲（为使代码描述简洁）
   public boolean hasParent();
   //返回当前节点的父节点
   public BinTreePosition getParent();
   //设置当前节点的父节点
   public void setParent(BinTreePosition p);

   //判断是否为叶子
   public boolean isLeaf();

   //判断是否为左孩子（为使代码描述简洁）
   public boolean isLChild();

   //判断是否有左孩子（为使代码描述简洁）
   public boolean hasLChild();
   //返回当前节点的左孩子
   public BinTreePosition getLChild();
   //设置当前节点的左孩子（注意：this.lChild和c.parent都不一定为空）
   public void setLChild(BinTreePosition c);

   //判断是否为右孩子（为使代码描述简洁）
   public boolean isRChild();
   //判断是否有右孩子（为使代码描述简洁）
   public boolean hasRChild();
   //返回当前节点的右孩子
   public BinTreePosition getRChild();
   //设置当前节点的右孩子（注意：this.rChild和c.parent都不一定为空）
   public void setRChild(BinTreePosition c);

   //返回当前节点后代元素的数目
   public int getSize();
   //在孩子发生变化后，更新当前节点及其祖先的规模
   public void updateSize();

   //返回当前节点的高度
   public int getHeight();
   //在孩子发生变化后，更新当前节点及其祖先的高度
   public void updateHeight();

   //返回当前节点的深度
   public int getDepth();
   //在父亲发生变化后，更新当前节点及其后代的深度
   public void updateDepth();

   //按照中序遍历的次序，找到当前节点的直接前驱
   public BinTreePosition getPrev();

   //按照中序遍历的次序，找到当前节点的直接后继
   public BinTreePosition getSucc();

   //断绝当前节点与其父亲的父子关系
   //返回当前节点
   public BinTreePosition secede();

   //将节点c作为当前节点的左孩子
   public BinTreePosition attachL(BinTreePosition c);

   //将节点c作为当前节点的右孩子
   public BinTreePosition attachR(BinTreePosition c);

   //前序遍历
   public Iterator elementsPreorder();

   //中序遍历
   public Iterator elementsInorder();

   //后序遍历
   public Iterator elementsPostorder();

   //层次遍历
   public Iterator elementsLevelorder();
}