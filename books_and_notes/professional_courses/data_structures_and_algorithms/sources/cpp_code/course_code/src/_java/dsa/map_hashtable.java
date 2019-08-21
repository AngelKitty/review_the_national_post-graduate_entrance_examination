/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于散列表实现的映射结构
 * 采用分离链策略解决冲突
 */

package dsa;

public class Map_HashTable implements Map {
   private Map[] A;//桶数组，每个桶本身也是一个（基于列表实现的）映射结构
   private  int N;//散列表长
   private final double maxLemda = 0.75;//装填因子上限
   private int size;//映射结构的规模
   private EqualityTester T;//判等器

   //默认构造方法
   public Map_HashTable()
   { this(0, new EqualityTesterDefault()); }

   //构造方法
   public Map_HashTable(int n, EqualityTester t) {
      T = t;
      N = p(n);//桶数组容量取为不小于n的最小素数
      A = new Map[N];
      for (int i = 0; i < N; i++) A[i] = new Map_DLNode(T);
      size = 0;
   }

   /***************************** 辅助方法 *****************************/
   //散列定址函数（采用模余法）
   private int h(Object key)
   { return key.hashCode() % N; }

   //判断n是否为素数
   private static boolean prime(int n) {
      for (int i = 3; i < 1 + Math.sqrt(n); i++)
         if (n / i* i == n)   return false;
      return true;
   }

   //取不小于n的最小素数
   private static int p(int n) {
      if (3 > n) n = 3;
      n = n | 1;//奇数化
      while (!prime(n)) n += 2;
      return n;
   }

   /***************************** ADT方法 *****************************/
   //查询映射结构当前的规模
   public int getSize()
   { return size; }

   //判断映射结构是否为空
   public boolean isEmpty()
   { return 0 == size; }

   //若M中存在以key为关键码的条目，则返回该条目的数据对象；否则，返回null
   public Object get(Object key)
   { return A[h(key)].get(key); }

   //若M中不存在以key为关键码的条目，则将条目(key, value)加入到M中并返回null
   //否则，将已有条目的数据对象替换为value，并返回原先的数据对象
   public Object put(Object key, Object value) {
      Object oldValue = A[h(key)].put(key, value);
      if (null == oldValue) { //若插入的条目未出现于原散列表中，则
         size++;//更新规模记录
         if (size > N * maxLemda)   rehash();//若装填因子过大，则重散列
      }
      return oldValue;
   }

   //若M中存在以key为关键码的条目，则删除之并返回其数据对象；否则，返回null
   public Object remove(Object key) {
      Object oldValue = A[h(key)].remove(key);
      if (null != oldValue)  size--;
      return oldValue;
   }

   //返回M中所有条目的一个迭代器
   //将各桶对应的映射结构的迭代器串接起来，构成整体的迭代器
   public Iterator entries() {
      List L = new List_DLNode();
      for (int i = 0; i < N; i++) {
         Iterator it = A[i].entries();
         while (it.hasNext()) L.insertLast(it.getNext());
      }
      return new IteratorElement(L);
   }

   //重散列
   private void rehash() {
      Iterator it = this.entries();
      N = p(N << 1);
      A = new Map[N];//桶数组容量至少加倍
      for (int i = 0; i < N; i++) A[i] = new Map_DLNode(T); //为每个桶分配一个子映射
      while (it.hasNext()) {//将其对应的映射结构中的
         Entry e = (Entry)it.getNext();//各条目逐一取出，将其
         Object k = e.getKey();//关键码和
         Object v = e.getValue();//数据对象
         A[h(k)].put(k, v);//整合为新的条目，插入对应的子映射中
      }
   }
}