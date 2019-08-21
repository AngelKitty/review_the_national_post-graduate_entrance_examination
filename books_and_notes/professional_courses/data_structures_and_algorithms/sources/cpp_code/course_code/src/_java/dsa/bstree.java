/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于链表式BST实现的词典结构
 * 基于BinTree进行扩充
 */

package dsa;

public class BSTree extends BinTree_LinkedList implements Dictionary {
   /**************************** 实例变量 ****************************/
   protected Comparator C;//比较器
   protected BinTreePosition lastV;//最后操作的节点，以便AVL树、伸展树重平衡

   /**************************** 构造方法 ****************************/
   public BSTree()
   { this(null, new ComparatorDefault()); }

   public BSTree(BinTreePosition r)
   { this(r, new ComparatorDefault()); }

   public BSTree(BinTreePosition r, Comparator c)
   {  root = r;   C = c; }

   /**************************** 词典方法 ****************************/
   //若词典中存在以key为关键码的条目，则返回其中的一个条目；否则，返回null
   public Entry find(Object key) {
      if (isEmpty()) return null;
      BSTreeNode u = binSearch((BSTreeNode)root, key, C);
      return (0 == C.compare(key, u.getKey())) ? (Entry)u.getElem() : null;
   }

   //返回由关键码为key的条目组成的迭代器
   public Iterator findAll(Object key) {
      List s = new List_DLNode();
      finAllNodes((BSTreeNode)root, key, s, C);
      return s.elements();
   }

   //插入条目(key, value)，并返回该条目
   //lastV指示被插入的节点
   public Entry insert(Object key, Object value) {
      Entry e = new EntryDefault(key, value);//创建新的元素
      if (isEmpty()) {//插入根节点的情况
         lastV = root = new BSTreeNode(e, null, true, null, null);//插入新节点
      } else {//插入一般节点的情况
         BSTreeNode p = (BSTreeNode)root;//从根节点开始，查找可插入位置
         boolean asLeftChild;//表示新节点是否作为p的左孩子插入
         while(true) {//不断地
            p = binSearch(p, key, C);//查找关键码为key的节点，直至
            if (C.compare(key, p.getKey()) < 0)//查找失败于无左孩子节点，或
               { asLeftChild = true; break; }
            else if (C.compare(key, p.getKey()) > 0)//查找失败无右孩子节点，或
               { asLeftChild = false; break; }
            else if (!p.hasLChild())//查找成功，且可作为左孩子插入，或
               { asLeftChild = true; break; }
            else if (!p.hasRChild())//查找成功，且可作为右孩子插入，或
               { asLeftChild = false; break; }
            else//否则
               p = (BSTreeNode)p.getLChild();//在左子树中继续查找（当然，在右子树中查找亦可）
         }//至此，新节点可以作为p的孩子插入，插入的方向由childType确定
         lastV = new BSTreeNode(e, p, asLeftChild, null, null);//插入新节点
      }//else
      return e;
   }

   //若词典中存在以key为关键码的条目，则摘除这样的一个节点，并返回其中存放的条目；否则，返回null
   //lastV指示被删除节点的父亲
   public Entry remove(Object key) {
      if (isEmpty()) return null;//空树
      BinTreePosition v = binSearch((BSTreeNode)root, key, C);//查找
      if (0 != C.compare(key, ((BSTreeNode)v).getKey())) return null;//若查找失败，则返回null
      //至此查找必成功，v为待删除节点
      if (v.hasLChild()) {//若v的左子树非空，则
         BinTreePosition w = v.getPrev();//在v的左子树中找出其直接前驱w
         w.setElem(v.setElem(w.getElem()));//交换v和u的数据对象
         v = w;//这样，相当于删除w
      }
      //至此，v至多只有一个孩子
      //下面，删除v，代之以其孩子
      lastV = v.getParent();//取待删除节点v的父亲
      BinTreePosition u = v.hasLChild() ? v.getLChild() : v.getRChild();//取v的孩子u
      if (null == lastV)//若v恰为树根
         {  if (null != u) u.secede(); root = u; }//将u作为树根
      else {//否则
         if (v.isLChild())//若v是p的左孩子，则
            { v.secede();  lastV.attachL(u); }//摘出v，将u作为p的左孩子
         else//否则
            { v.secede();  lastV.attachR(u); }//摘出v，将u作为p的右孩子
      }
      return (Entry) v.getElem();//返回被删除节点中存放的的元素
   }

   //返回词典中所有条目的一个迭代器
   public Iterator entries() {
      List list = new List_DLNode();
      concatenate(list, (BSTreeNode)root);
      return list.elements();
   }

   /**************************** 辅助方法 ****************************/
   //在以v为根的子树中查找关键码为key的节点（假设该子树不为空）
   //  若找到，则返回该节点
   //  否则，返回被访问的最后一个节点
   //为了确定是否成功，上层方法需要再检查一次返回节点的关键码
   protected static BSTreeNode binSearch(BSTreeNode v, Object key, Comparator c) {
      BSTreeNode  u = v;//当前节点
      while (true) {//不断地
         int comp = c.compare(key, u.getKey());//将当前节点与目标关键码做比较
         if (comp < 0)//若目标关键码更小，则
            if (u.hasLChild())//若u有左孩子
               u = (BSTreeNode)u.getLChild();//递归查找左子树，或
            else
               return u;//终止于无左孩子节点
         else if (comp > 0)//若目标关键码更大，则
            if (u.hasRChild())//u有右孩子
               u = (BSTreeNode)u.getRChild();//递归查找右子树，或
            else
               return u;//终止于无右孩子节点
         else
            return u;//查找命中
      }
   }

   //在以v为根节点的（子）树中，递归地找出关键码为key的所有节点
   //这些节点被组织为一个列表（借此可以生成一个迭代器）
   protected static void finAllNodes(BSTreeNode v, Object k, List s, Comparator c) {
      if (null == v) return;//递归基：空树
      int comp = c.compare(k, v.getKey());
      if (0 >= comp) finAllNodes((BSTreeNode)v.getLChild(), k, s, c);//查找左子树
      if (0 == comp) s.insertLast(v);//命中
      if (0 <= comp) finAllNodes((BSTreeNode)v.getRChild(), k, s, c);//查找右子树
   }

   //将v的所有后代节点（中存放的条目）组织为一个列表（借此可以生成一个迭代器）
   protected static void concatenate(List list, BSTreeNode v) {
      if (null == v) return;
      concatenate(list, (BSTreeNode) v.getLChild());
      list.insertLast(v.getElem());
      concatenate(list, (BSTreeNode) v.getRChild());
   }
}