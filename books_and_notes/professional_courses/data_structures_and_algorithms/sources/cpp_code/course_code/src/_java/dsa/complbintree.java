/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 完全二叉树接口
 */

package dsa;

public interface ComplBinTree extends BinTree {
   //生成并返回一个存放e的外部节点，该节点成为新的末节点
   public BinTreePosition addLast(Object e);

   //删除末节点，并返回其中存放的内容
   public Object delLast();

   //返回按照层次遍历编号为i的节点的位置，0 <= i < size()
   public BinTreePosition posOfNode(int i);
}