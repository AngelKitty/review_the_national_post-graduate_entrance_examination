/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 伸展树
 * 基于BSTree的扩充
 */

package dsa;

public class SplayTree extends BSTree implements Dictionary {
   /**************************** 构造方法 ****************************/
   public SplayTree() { super(); }
   public SplayTree(BinTreePosition r) { super(r); }
   public SplayTree(BinTreePosition r, Comparator c) { super(r, c); }

   /**************************** 词典方法（覆盖父类BSTree） ****************************/
   //若词典中存在以key为关键码的条目，则返回其中的一个条目；否则，返回null
   public Entry find(Object key) {
      if (isEmpty()) return null;
      BSTreeNode u = binSearch((BSTreeNode)root, key, C);
      root = moveToRoot(u);
      return (0 == C.compare(key, u.getKey())) ? (Entry)u.getElem() : null;
   }

   //插入条目(key, value)，并返回该条目
   public Entry insert(Object key, Object value) {
      Entry e = super.insert(key, value);//调用父类方法完成插入
      root = moveToRoot(lastV);//重新平衡化
      return e;
   }

   //若词典中存在以key为关键码的条目，则将摘除其中的一个并返回；否则，返回null
   public Entry remove(Object key) {
      Entry e = super.remove(key);//调用父类方法完成删除
      if (null != e && null != lastV)  root = moveToRoot(lastV);//重新平衡化
      return e;
   }

   /**************************** 辅助方法 ****************************/
   //从节点z开始，自上而下重新平衡化
   protected static BinTreePosition moveToRoot(BinTreePosition z) {
      while (z.hasParent())
         if (!z.getParent().hasParent())
            if (z.isLChild()) z = zig(z);
            else                    z = zag(z);
         else if (z.isLChild())
            if (z.getParent().isLChild()) z = zigzig(z);
            else                                      z = zigzag(z);
         else if (z.getParent().isLChild()) z = zagzig(z);
         else                                      z = zagzag(z);
      return z;
   }

   //v为左孩子
   //顺时针旋转v，使之上升一层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zig(BinTreePosition v) {
      if (null != v && v.isLChild()) {//v必须有父亲，而且必须是左孩子
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         boolean asLChild = p.isLChild();//父亲是否祖父的左孩子
         v.secede();//摘出v（于是p的左孩子为空）
         BinTreePosition c = v.getRChild();//将v的右孩子
         if (null != c) p.attachL(c.secede());//作为p的左孩子
         p.secede();//摘出父亲
         v.attachR(p);//将p作为v的右孩子
         if (null != g)//若祖父存在，则将v作为其孩子
            if (asLChild)  g.attachL(v);
            else              g.attachR(v);
      }
      return v;
   }

   //v为右孩子
   //逆时针旋转v，使之上升一层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zag(BinTreePosition v) {
      if (null != v && v.isRChild()) {//v必须有父亲，而且必须是右孩子
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         boolean asLChild = p.isLChild();//父亲是否祖父的左孩子
         v.secede();//摘出v（于是p的左孩子为空）
         BinTreePosition c = v.getLChild();//将v的左孩子
         if (null != c) p.attachR(c.secede());//作为p的右孩子
         p.secede();//摘出父亲
         v.attachL(p);//将p作为v的左孩子
         if (null != g)//若祖父存在，则将v作为其孩子
            if (asLChild)  g.attachL(v);
            else              g.attachR(v);
      }
      return v;
   }

   //v为左孩子，父亲为左孩子
   //顺时针旋转v，使之上升两层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zigzig(BinTreePosition v) {
      if (null != v && v.isLChild() && v.hasParent() && v.getParent().isLChild()) {
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         BinTreePosition s = g.getParent();//曾祖父
         boolean asLChild = g.isLChild();//祖父是否曾祖父的左孩子
         g.secede();
         p.secede();
         v.secede();
         BinTreePosition c;//临时变量，辅助孩子的移动
         c = p.getRChild(); if (null != c)   g.attachL(c.secede());//p的右孩子作为g的左孩子
         c = v.getRChild(); if (null != c)   p.attachL(c.secede());//v的右孩子作为p的左孩子
         p.attachR(g);//g作为p的右孩子
         v.attachR(p);//p作为v的右孩子
         if (null != s)//若曾祖父存在，则将v作为其孩子
            if (asLChild)  s.attachL(v);
            else              s.attachR(v);
      }
      return v;
   }

   //v为右孩子，父亲为右孩子
   //顺时针旋转v，使之上升两层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zagzag(BinTreePosition v) {
      if (null != v && v.isRChild() && v.hasParent() && v.getParent().isRChild()) {
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         BinTreePosition s = g.getParent();//曾祖父
         boolean asLChild = g.isLChild();//祖父是否曾祖父的左孩子
         g.secede();
         p.secede();
         v.secede();
         BinTreePosition c;//临时变量，辅助孩子的移动
         c = p.getLChild(); if (null != c)   g.attachR(c.secede());//p的左孩子作为g的右孩子
         c = v.getLChild(); if (null != c)   p.attachR(c.secede());//v的左孩子作为p的右孩子
         p.attachL(g);//g作为p的左孩子
         v.attachL(p);//p作为v的左孩子
         if (null != s)//若曾祖父存在，则将v作为其孩子
            if (asLChild)  s.attachL(v);
            else              s.attachR(v);
      }
      return v;
   }

   //v为左孩子，父亲为右孩子
   //顺时针旋转v，使之上升两层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zigzag(BinTreePosition v) {
      if (null != v && v.isLChild() && v.hasParent() && v.getParent().isRChild()) {
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         BinTreePosition s = g.getParent();//曾祖父
         boolean asLChild = g.isLChild();//祖父是否曾祖父的左孩子
         g.secede();
         p.secede();
         v.secede();
         BinTreePosition c;//临时变量，辅助孩子的移动
         c = v.getLChild(); if (null != c)   g.attachR(c.secede());//v的左孩子作为g的右孩子
         c = v.getRChild(); if (null != c)   p.attachL(c.secede());//v的右孩子作为p的左孩子
         v.attachL(g);//g作为v的左孩子
         v.attachR(p);//p作为v的右孩子
         if (null != s)//若曾祖父存在，则将v作为其孩子
            if (asLChild)  s.attachL(v);
            else              s.attachR(v);
      }
      return v;
   }

   //v为右孩子，父亲为左孩子
   //顺时针旋转v，使之上升两层（伸展树）
   //返回新的子树根
   protected static BinTreePosition zagzig(BinTreePosition v) {
      if (null != v && v.isRChild() && v.hasParent() && v.getParent().isLChild()) {
         BinTreePosition p = v.getParent();//父亲
         BinTreePosition g = p.getParent();//祖父
         BinTreePosition s = g.getParent();//曾祖父
         boolean asLChild = g.isLChild();//祖父是否曾祖父的左孩子
         g.secede();
         p.secede();
         v.secede();
         BinTreePosition c;//临时变量，辅助孩子的移动
         c = v.getRChild(); if (null != c)   g.attachL(c.secede());//v的右孩子作为g的左孩子
         c = v.getLChild(); if (null != c)   p.attachR(c.secede());//v的左孩子作为p的右孩子
         v.attachR(g);//g作为v的右孩子
         v.attachL(p);//p作为v的左孩子
         if (null != s)//若曾祖父存在，则将v作为其孩子
            if (asLChild)  s.attachL(v);
            else              s.attachR(v);
      }
      return v;
   }
}