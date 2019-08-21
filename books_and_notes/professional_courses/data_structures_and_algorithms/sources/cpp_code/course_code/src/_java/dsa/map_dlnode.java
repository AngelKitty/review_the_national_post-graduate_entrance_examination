/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于列表实现映射结构
 */

package  dsa;

public class Map_DLNode implements Map {
   private List L;//存放条目的列表
   private EqualityTester T;//判等器

   //构造方法
   public Map_DLNode()
   {  this(new EqualityTesterDefault()); }

   //默认构造方法
   public Map_DLNode(EqualityTester t)
   {  L = new List_DLNode();  T = t; }

   /***************************** ADT方法 *****************************/
   //查询映射结构当前的规模
   public int getSize()
   { return L.getSize(); }

   //判断映射结构是否为空
   public boolean isEmpty()
   { return L.isEmpty(); }

   //若M中存在以key为关键码的条目，则返回该条目的数据对象；否则，返回null
   public Object get(Object key) {
      Iterator P = L.positions();
      while (P.hasNext()) {
         Position pos = (Position)P.getNext();
         Entry entry = (EntryDefault) pos.getElem();
         if (T.isEqualTo(entry.getKey(), key))  return entry.getValue();
      }
      return null;
   }

   //若M中不存在以key为关键码的条目，则将条目(key, value)加入到M中并返回null
   //否则，将已有条目的数据对象替换为value，并返回原先的数据对象
   public Object put(Object key, Object value)  {
      Iterator P = L.positions();
      while (P.hasNext()) {//逐一对比
         Position pos = (Position)P.getNext();//各个位置
         Entry entry = (EntryDefault) pos.getElem();//处的条目
         if (T.isEqualTo(entry.getKey(), key)) {//若发现key已出现在某个条目中，则
            Object oldValue = entry.getValue();//先保留该条目原先的数据对象
            L.replace(pos, new EntryDefault(key, value));//再替之以新数据对象
            return oldValue;//最后返回原先的数据对象。注意：返回null时的歧义
         }
      }//若此循环结束，说明key尚未在M中出现，因此
      L.insertFirst(new EntryDefault(key, value));//将新条目插至表首，并
      return null;//返回null标志
   }

   //若M中存在以key为关键码的条目，则删除之并返回其数据对象；否则，返回null
   public Object remove(Object key) {
      Iterator P = L.positions();
      while (P.hasNext()) {//逐一对比
         Position pos = (Position)P.getNext();//各个位置
         Entry entry = (EntryDefault) pos.getElem();//处的条目
         if (T.isEqualTo(entry.getKey(), key)) {//若发现key已出现在某个条目中，则
            Object oldValue = entry.getValue();//先保留该条目原先的数据对象
            L.remove(pos);//删除该条目
            return oldValue;//最后返回原先的数据对象。注意：返回null时的歧义
         }
      }//若此循环结束，说明key尚未在映射中出现，因此
      return null;//返回null标志
   }

   //返回M中所有条目的一个迭代器
   public Iterator entries()
   {  return new IteratorElement(L); }//直接利用List接口的方法生成元素迭代器
}