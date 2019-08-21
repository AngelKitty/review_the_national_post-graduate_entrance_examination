/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 条目接口
 */

package  dsa;

public interface Entry {
   //取条目的关键码
   public Object getKey();
   //修改条目的关键码，返回此前存放的关键码
   public Object setKey(Object k);

   //取条目的数据对象
   public Object getValue();
   //修改条目的数据对象，返回此前存放的数据对象
   public Object setValue(Object v);
}