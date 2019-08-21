/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../List/List.h" //以列表为基类，派生出栈模板类
template <typename T> class Stack: public List<T> { //将列表的首/末端作为栈顶/底
public: //size()、empty()以及其它开放接口，均可直接沿用
   void push ( T const& e ) { insertAsLast ( e ); } //入栈：等效于将新元素作为列表的首元素插入
   T pop() { return remove ( last() ); } //出栈：等效于删除列表的首元素
   T& top() { return last()->data; } //取顶：直接返回列表的首元素
};
