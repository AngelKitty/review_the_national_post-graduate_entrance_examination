/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于列表实现（无序）词典结构
 */

package  dsa;

public class Dictionary_DLNode implements Dictionary {
   private List L;//存放条目的列表
   private EqualityTester T;//判等器

   //构造方法
   public Dictionary_DLNode()
   {  this(new EqualityTesterDefault()); }

   //默认构造方法
   public Dictionary_DLNode(EqualityTester t)
   {  L = new List_DLNode();  T = t; }

   /***************************** ADT方法 *****************************/
   //查询词典结构当前的规模
   public int getSize()
   { return L.getSize(); }

   //判断词典结构是否为空
   public boolean isEmpty()
   { return L.isEmpty(); }

   //若词典中存在以key为关键码的条目，则返回其中的一个条目；否则，返回null
   public Entry find(Object key) {
      Iterator P = L.positions();
      while (P.hasNext()) {
         Position pos = (Position)P.getNext();
         Entry entry = (EntryDefault) pos.getElem();
         if (T.isEqualTo(entry.getKey(), key))  return entry;
      }
      return null;
   }

   //返回由关键码为key的条目组成的迭代器
   public Iterator findAll(Object key) {
      List  list = new List_DLNode();
      Iterator P = L.positions();
      while (P.hasNext()) {
         Position pos = (Position)P.getNext();
         Entry entry = (EntryDefault) pos.getElem();
         if (T.isEqualTo(entry.getKey(), key))
            list.insertLast(entry);
      }
      return new IteratorElement(list);
   }

   //插入条目(key, value)，并返回该条目
   public Entry insert(Object key, Object value) {
      Entry entry = new EntryDefault(key, value);//创建新条目
      L.insertFirst(entry);//将新条目插至表首，并
      return entry;//返回null标志
   }

   //若词典中存在以key为关键码的条目，则将摘除其中的一个并返回；否则，返回null
   public Entry remove(Object key) {
      Iterator P = L.positions();
      while (P.hasNext()) {//逐一对比
         Position pos = (Position)P.getNext();//各个位置
         Entry entry = (EntryDefault) pos.getElem();//处的条目
         if (T.isEqualTo(entry.getKey(), key)) {//若发现key已出现在某个条目中，则
            Entry oldEntry = entry;//先保留该条目
            L.remove(pos);//删除该条目
            return oldEntry;//最后返回原先的条目
         }
      }//若此循环结束，说明key尚未在词典中出现，因此
      return null;//返回null标志
   }

   //返回词典中所有条目的一个迭代器
   public Iterator entries()
   {  return new IteratorElement(L); }//直接利用List接口的方法生成元素迭代器
}