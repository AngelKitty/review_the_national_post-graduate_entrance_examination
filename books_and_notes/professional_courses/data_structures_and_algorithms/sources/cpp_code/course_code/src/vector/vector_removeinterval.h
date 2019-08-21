/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> int Vector<T>::remove ( Rank lo, Rank hi ) { //删除区间[lo, hi)
   if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
   while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
   _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
   shrink(); //若有必要，则缩容
   return hi - lo; //返回被删除元素的数目
}