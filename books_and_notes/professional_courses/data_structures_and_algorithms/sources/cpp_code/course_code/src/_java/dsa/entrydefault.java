/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 默认条目
 */

package  dsa;

public class EntryDefault implements Entry {
   protected Object key;
   protected Object value;

   /**************************** 构造函数 ****************************/
   public EntryDefault(Object k, Object v)
   { key = k;  value = v; }

   /**************************** Entry接口方法 ****************************/
   //取条目的关键码
   public Object getKey()
   { return key; }
   //修改条目的关键码，返回此前存放的关键码
   public Object setKey(Object k)
   { Object oldK = key; key = k; return oldK; }

   //取条目的数据对象
   public Object getValue()
   { return value; }
   //修改条目的数据对象，返回此前存放的数据对象
   public Object setValue(Object v)
   { Object oldV = value;  value = v;  return oldV; }
}