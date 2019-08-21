/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 位置ADT接口
 */

package dsa;

public interface Position {
   public Object  getElem();//返回存放于该位置的元素
   public Object setElem(Object e);//将给定元素存放至该位置，返回此前存放的元素
}