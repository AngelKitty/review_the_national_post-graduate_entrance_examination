/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * AVL树
 * 基于BSTree的扩充
 */

package dsa;

public class AVLTree extends BSTree implements Dictionary {
   /**************************** 构造方法 ****************************/
   public AVLTree() { super(); }
   public AVLTree(BinTreePosition r) { super(r); }
   public AVLTree(BinTreePosition r, Comparator c) { super(r, c); }

   /**************************** 词典方法（覆盖父类BSTree） ****************************/
   //插入条目(key, value)，并返回该条目
   public Entry insert(Object key, Object value) {
      Entry e = super.insert(key, value);//调用父类方法完成插入
      root = rebalance(lastV.getParent(), root);//从插入节点的父亲开始重新平衡化
      return e;
   }

   //若词典中存在以key为关键码的条目，则将摘除其中的一个并返回；否则，返回null
   public Entry remove(Object key) {
      Entry e = super.remove(key);//调用父类方法完成删除
      if (null != e) root = rebalance(lastV, root);//从删除节点的父亲开始重新平衡化
      return e;
   }

   /**************************** 辅助方法 ****************************/
   //从节点z开始，自上而下重新平衡化
   //返回后，root仍为平衡后的（整棵）树的根节点
   protected static BinTreePosition rebalance(BinTreePosition z, BinTreePosition r) {
      if (null == z) return r;
      while (true) {//从z开始，向上逐一检查z的祖先
         if (!isBalanced(z))  rotate(z);//若z节点失去平衡，则通过旋转使之重新平衡
         if (!z.hasParent())  return z;
         z = z.getParent();//继续检查其父亲
      }//while
   }

   //判断节点v是否平衡
   protected static boolean isBalanced(BinTreePosition v) {
      if (null == v) return true;
      int lH = (v.hasLChild()) ? (v.getLChild().getHeight()) : -1;
      int rH = (v.hasRChild()) ? (v.getRChild().getHeight()) : -1;
      int deltaH = lH - rH;
      return (-1 <= deltaH) && (deltaH <= 1);
   }

   //通过旋转，使节点z的平衡因子的绝对值不超过1（支持AVL树）
   //返回新的子树根
   public static BinTreePosition rotate(BinTreePosition z) {
      BinTreePosition   y = tallerChild(z);//取y为z更高的孩子
      BinTreePosition   x = tallerChild(y);//取x为y更高的孩子
      boolean cType = z.isLChild();//记录：z是否左孩子
      BinTreePosition   p = z.getParent();//p为z的父亲
      BinTreePosition   a, b, c;//自左向右，三个节点
      BinTreePosition   t0, t1, t2, t3;//自左向右，四棵子树
      /******** 以下分四种情况 ********/
      if (y.isLChild()) {//若y是左孩子，则
         c = z;   t3 = z.getRChild();
         if (x.isLChild()) {//若x是左孩子
            b = y;   t2 = y.getRChild();
            a = x;   t1 = x.getRChild();  t0 = (BSTreeNode)x.getLChild();
         } else {//若x是右孩子
            a = y;   t0 = y.getLChild();
            b = x;   t1 = x.getLChild();  t2 = (BSTreeNode)x.getRChild();
         }
      } else {//若y是右孩子，则
         a = z;   t0 = z.getLChild();
         if (x.isRChild()) {//若x是右孩子
            b = y;   t1 = y.getLChild();
            c = x;   t2 = x.getLChild();  t3 = (BSTreeNode)x.getRChild();
         } else {//若x是左孩子
            c = y;   t3 = y.getRChild();
            b = x;   t1 = x.getLChild();  t2 = (BSTreeNode)x.getRChild();
         }
      }
      //摘下三个节点
      z.secede();
      y.secede();
      x.secede();
      //摘下四棵子树
      if (null != t0) t0.secede();
      if (null != t1) t1.secede();
      if (null != t2) t2.secede();
      if (null != t3) t3.secede();
      //重新链接
      a.attachL(t0); a.attachR(t1); b.attachL(a);
      c.attachL(t2); c.attachR(t3); b.attachR(c);
      //子树重新接入原树
      if (null != p)
         if (cType)  p.attachL(b);
         else           p.attachR(b);
      return b;//返回新的子树根
   }//rotate

   //返回节点p的孩子中的更高者
   protected static BinTreePosition tallerChild(BinTreePosition v) {
      int   lH = v.hasLChild() ? v.getLChild().getHeight() : -1;
      int   rH = v.hasRChild() ? v.getRChild().getHeight() : -1;
      if (lH > rH)   return v.getLChild();
      if (lH < rH)   return v.getRChild();
      if (v.isLChild()) return v.getLChild();
      else                    return v.getRChild();
   }

   //返回节点p的孩子中的更矮者
   protected static BinTreePosition shorterChild(BinTreePosition v) {
      int   lH = v.hasLChild() ? v.getLChild().getHeight() : -1;
      int   rH = v.hasRChild() ? v.getRChild().getHeight() : -1;
      if (lH > rH)   return v.getRChild();
      if (lH < rH)   return v.getLChild();
      if (v.isLChild()) return v.getRChild();
      else                    return v.getLChild();
   }
}
