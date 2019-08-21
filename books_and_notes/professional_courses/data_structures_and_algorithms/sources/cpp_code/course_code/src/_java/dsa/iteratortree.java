/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/**
 * 基于列表实现的树迭代器
 */

package dsa;

public class IteratorTree implements Iterator {
   private List list;//列表
   private Position nextPosition;//当前（下一个）元素的位置

   //默认构造方法
   public IteratorTree() { list = null; }

   //前序遍历
   public void elementsPreorderIterator(TreeLinkedList T) {
      if (null == T) return;//递归基
      list.insertLast(T);//首先输出当前节点
      TreeLinkedList subtree = T.getFirstChild();//从当前节点的长子开始
      while (null != subtree) {//依次对当前节点的各个孩子
         this.elementsPreorderIterator(subtree);//做前序遍历
         subtree = subtree.getNextSibling();
      }
   }

   //后序遍历
   public void elementsPostorderIterator(TreeLinkedList T) {
      if (null == T) return;//递归基
      TreeLinkedList subtree = T.getFirstChild();//从当前节点的长子开始
      while (null != subtree) {//依次对当前节点的各个孩子
         this.elementsPostorderIterator(subtree);//做后序遍历
         subtree = subtree.getNextSibling();
      }
      list.insertLast(T);//当所有后代都访问过后，最后才访问当前节点
   }

   //层次遍历
   public void levelTraversalIterator(TreeLinkedList T) {
      if (null == T) return;
      Queue_List  Q = new Queue_List();//空队
      Q.enqueue(T);//根节点入队
      while (!Q.isEmpty()) {//在队列重新变空之前
         TreeLinkedList tree = (TreeLinkedList) (Q.dequeue());//取出队列首节点
         list.insertLast(tree);//将新出队的节点接入迭代器中
         TreeLinkedList subtree = tree.getFirstChild();//从tree的第一个孩子起
         while (null != subtree) {//依次找出所有孩子，并
            Q.enqueue(subtree);//将其加至队列中
            subtree = subtree.getNextSibling();
         }
      }
   }

   //检查迭代器中是否还有剩余的元素
   public boolean hasNext() { return (null != nextPosition); }

   //返回迭代器中的下一元素
   public Object getNext() throws ExceptionNoSuchElement {
      if (!hasNext()) throw new ExceptionNoSuchElement("No next position");
      Position currentPosition = nextPosition;
      if (currentPosition == list.last())//若已到达尾元素，则
         nextPosition = null;//不再有下一元素
      else//否则
         nextPosition = list.getNext(currentPosition);//转向下一元素
      return currentPosition.getElem();
   }
}