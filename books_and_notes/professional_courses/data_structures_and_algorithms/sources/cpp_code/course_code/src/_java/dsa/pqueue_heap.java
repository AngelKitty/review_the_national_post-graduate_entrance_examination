/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 利用堆实现优先队列
 */

package dsa;

public class PQueue_Heap implements PQueue {
   private ComplBinTree H;//完全二叉树形式的堆
   private Comparator comp;//比较器

   //构造方法
   public PQueue_Heap()
   { this(new ComparatorDefault(), null); }

   //构造方法：默认的空优先队列
   public PQueue_Heap(Comparator c)
   { this(c, null); }

   //构造方法：根据某一序列直接批量式构造堆算法，S中元素都是形如(key, value)的条目
   public PQueue_Heap(Sequence S)
   { this(new ComparatorDefault(), S); }

   //构造方法：根据某一序列直接批量式构造堆算法，s中元素都是形如(key, value)的条目
   public PQueue_Heap(Comparator c, Sequence s) {
      comp = c;
      H = new ComplBinTree_Vector(s);
      if (!H.isEmpty()) {
         for (int i = H.getSize() / 2 - 1; i >= 0; i--) //自底而上
            percolateDown(H.posOfNode(i));//逐节点进行下滤
      }
   }

   /*-------- PQueue接口中定义的方法 --------*/
   //统计优先队列的规模
   public int getSize()
   { return H.getSize(); }

   //判断优先队列是否为空
   public boolean isEmpty()
   { return H.isEmpty(); }

   //若Q非空，则返回其中的最小条目（并不删除）;否则，报错
   public Entry getMin() throws ExceptionPQueueEmpty {
      if (isEmpty()) throw new ExceptionPQueueEmpty("意外：优先队列为空");
      return (Entry) H.getRoot().getElem();
   }

   //将对象obj与关键码k合成一个条目，将其插入Q中，并返回该条目
   public Entry insert(Object key, Object obj) throws ExceptionKeyInvalid  {
      checkKey(key);
      Entry entry = new EntryDefault(key, obj);
      percolateUp(H.addLast(entry));
      return entry;
   }

   //若Q非空，则从其中摘除关键码最小的条目，并返回该条目；否则，报错
   public Entry delMin() throws ExceptionPQueueEmpty  {
      if (isEmpty()) throw new ExceptionPQueueEmpty("意外：优先队列为空");
      Entry min = (Entry)H.getRoot().getElem();//保留堆顶
      if (1 == getSize())//若只剩下最后一个条目
         H.delLast();//直接摘除之
      else {//否则
         H.getRoot().setElem(((ComplBinTreeNode_Rank)H.delLast()).getElem());
         //取出最后一个条目，植入堆顶
         percolateDown(H.getRoot());
      }
      return min;//返回原堆顶
   }

   /*-------- 辅助方法 --------*/
   //检查关键码的可比较性
   protected void checkKey(Object key) throws ExceptionKeyInvalid {
      try {
         comp.compare(key, key);
      } catch (Exception e) {
         throw new ExceptionKeyInvalid("无法比较关键码");
      }
   }

   //返回节点v（中所存条目）的关键码
   protected Object key(BinTreePosition v) {
      return ((Entry)(v.getElem())).getKey();
   }

   /*-------- 算法方法 --------*/
   //交换父子节点（中所存放的内容）
   protected void swapParentChild(BinTreePosition u, BinTreePosition v) {
      Object temp = u.getElem();
      u.setElem(v.getElem());
      v.setElem(temp);
   }

   //上滤算法
   protected void percolateUp(BinTreePosition v) {
      BinTreePosition root = H.getRoot();//记录根节点
      while (v != H.getRoot()) {//不断地
         BinTreePosition p = v.getParent();//取当前节点的父亲
         if (0 >= comp.compare(key(p), key(v))) break;//除非父亲比孩子小
         swapParentChild(p, v);//否则，交换父子次序
         v = p;//继续考察新的父节点（即原先的孩子）
      }
   }

   //下滤算法
   protected void percolateDown(BinTreePosition v) {
      while (v.hasLChild()) {//直到v成为叶子
         BinTreePosition smallerChild = v.getLChild();//首先假设左孩子的（关键码）更小
         if (v.hasRChild() && 0 < comp.compare(key(v.getLChild()), key(v.getRChild())))
            smallerChild = v.getRChild();//若右孩子存在且更小，则将右孩子作为进一步比较的对象
         if (0 <= comp.compare(key(smallerChild), key(v))) break;//若两个孩子都不比v更小，则下滤完成
         swapParentChild(v, smallerChild);//否则，将其与更小的孩子交换
         v = smallerChild;//并继续考察这个孩子
      }
   }
}