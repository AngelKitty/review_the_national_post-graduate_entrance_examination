/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> struct Crc { //函数对象：累计T类对象的特征（比如总和），以便校验对象集合
   T& c;
   Crc ( T& crc ) : c ( crc ) {}
   virtual void operator() ( T& e ) { c += e; } //假设T可直接相加
};