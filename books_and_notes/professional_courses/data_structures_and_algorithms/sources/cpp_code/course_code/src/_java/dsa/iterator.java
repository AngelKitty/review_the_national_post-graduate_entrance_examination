/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 迭代器ADT接口
 */

package dsa;

public interface Iterator {
   boolean  hasNext();//检查迭代器中是否还有剩余的元素
   Object   getNext();//返回迭代器中的下一元素
}