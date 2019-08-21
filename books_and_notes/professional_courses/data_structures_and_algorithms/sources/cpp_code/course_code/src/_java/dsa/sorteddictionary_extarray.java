/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于有序查找表实现的有序词典
 */

package dsa;

public class SortedDictionary_ExtArray implements SortedDictionary {
   Vector   S;//有序查找表
   Comparator C;//比较器

   //默认构造方法
   public SortedDictionary_ExtArray()
   {  this(new ComparatorDefault()); }

   //构造方法
   public SortedDictionary_ExtArray(Comparator comp)
   {  S = new Vector_ExtArray(); C = comp; }

   /**************************** 辅助方法 ****************************/
   //二分查找
   //返回值可能是命中元素的秩，也可能是key可以插入的秩
   //具体如何，需要进一步检查
   //不变性：若将key按照返回的秩插入有序向量，向量依然有序
   private static int binSearch(Vector s, Comparator c, Object key, int lo, int hi) {
      if (lo > hi) return lo;//递归基，查找失败
      int mi = (lo + hi) >> 1; //取中值
      Entry e = (Entry)s.getAtRank(mi);//居中的条目
      int flag = c.compare(key, e.getKey());//比较关键码
      if (flag < 0) return binSearch(s, c, key, lo, mi - 1); //转向左半区间
      else if (flag > 0) return binSearch(s, c, key, mi + 1, hi); //转向右半区间
      else return mi;//命中
   }

   /**************************** 无序词典ADT方法 ****************************/
   //查询词典结构当前的规模
   public int getSize()
   {  return S.getSize(); }

   //判断词典结构是否为空
   public boolean isEmpty()
   { return S.isEmpty(); }

   //若词典中存在以key为关键码的条目，则返回其中的一个条目；否则，返回null
   public Entry find(Object key) {
      int k = binSearch(S, C, key, 0, S.getSize() - 1); //查找关键码为key的条目
      if (0 > k || k >= S.getSize() || (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey())))
         return null;//若这样的条目不存在，则返回失败标志
      return (Entry) S.getAtRank(k);
   }

   //返回由关键码为key的条目组成的迭代器
   public Iterator findAll(Object key) {
      List L = new List_DLNode();//创建一个链表L
      int k = binSearch(S, C, key, 0, S.getSize() - 1); //查找关键码为key的条目
      if (0 > k || k >= S.getSize() || (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey())))
         return new IteratorElement(L);//若这样的条目不存在，则返回空迭代器
      L.insertFirst(S.getAtRank(k));//将e插入L中
      int lo = k;//从S[k-1]开始
      while (0 <= --lo) {//不断向前搜索
         if (0 != C.compare(key, ((Entry)S.getAtRank(lo)).getKey())) break;//直到第一个不命中的条目
         L.insertFirst(S.getAtRank(lo));//将命中的条目插入L中
      }
      int hi = k;//从S[k+1]开始
      while (++hi < S.getSize()) {//不断向后搜索
         if (0 != C.compare(key, ((Entry)S.getAtRank(hi)).getKey())) break;//直到第一个不命中的条目
         L.insertLast(S.getAtRank(hi));//将命中的条目插入L中
      }
      return new IteratorElement(L);//由L创建迭代器，返回之
   }

   //插入条目(key, value)，并返回该条目
   public Entry insert(Object key, Object value) {
      Entry e = new EntryDefault(key, value);//创建新条目
      //若词典为空，则直接插入新元素
      if (S.isEmpty())  return (Entry) S.insertAtRank(0, e);
      //通过二分查找，确定可插入位置
      //请读者自己检查：即便key在S中为最小或最大，都可以正常插入
      return (Entry) S.insertAtRank(binSearch(S, C, key, 0, S.getSize() - 1), e);
   }

   //若词典中存在以key为关键码的条目，则将摘除其中的一个并返回；否则，返回null
   public Entry remove(Object key) {
      int k = binSearch(S, C, key, 0, S.getSize() - 1); //查找关键码为key的条目
      if (0 > k || k >= S.getSize() || (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey())))
         return null;//若这样的条目不存在，则返回失败标志
      return (Entry) S.removeAtRank(k);
   }

   //返回词典中所有条目的一个迭代器
   public Iterator entries() {
      List L = new List_DLNode();
      for (int i = 0; i < S.getSize(); i++)
         L.insertLast(S.getAtRank(i));
      return new IteratorElement(L);//直接利用List接口的方法生成元素迭代器
   }

   /**************************** 有序词典ADT方法 ****************************/
   //若词典非空，则返回其中关键码最小的条目；否则，返回null
   public Entry first()
   {  return (S.isEmpty()) ? null : (Entry) S.getAtRank(0); }

   //若词典非空，则返回其中关键码最大的条目；否则，返回null
   public Entry last()
   {  return (S.isEmpty()) ? null : (Entry) S.getAtRank(S.getSize() - 1); }

   //返回由关键码不小于key的条目依非降序组成的迭代器
   public Iterator successors(Object key) {
      List L = new List_DLNode();//创建一个链表L
      int k = binSearch(S, C, key, 0, S.getSize() - 1); //查找关键码为key的条目
      if (0 > k || k >= S.getSize() || (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey())))
         return new IteratorElement(L);//若这样的条目不存在，则返回空迭代器
      while (0 <= --k)//从S[k-1]开始向前搜索，直至符合要求的、秩最小的元素
         if (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey()))  break;
      while (S.getSize() > ++k)//将后继的所有元素依次
         L.insertLast(S.getAtRank(k));//插入L中
      return new IteratorElement(L);//由L创建迭代器，返回之
   }

   //返回由关键码不大于key的条目依非升序组成的迭代器
   public Iterator predecessors(Object key) {
      List L = new List_DLNode();//创建一个链表L
      int k = binSearch(S, C, key, 0, S.getSize() - 1); //查找关键码为key的条目
      if (0 > k || k >= S.getSize() || (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey())))
         return new IteratorElement(L);//若这样的条目不存在，则返回空迭代器
      while (S.getSize() > ++k)//从S[k-1]开始向后搜索，直至符合要求的、秩最大的元素
         if (0 != C.compare(key, ((Entry)S.getAtRank(k)).getKey()))  break;
      while (0 <= --k)//将前驱的所有元素依次
         L.insertLast(S.getAtRank(k));//插入L中
      return new IteratorElement(L);//由L创建迭代器，返回之
   }
}