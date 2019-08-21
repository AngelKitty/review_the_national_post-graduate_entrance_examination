/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

//对向量中的第i个词条实施上滤操作，i < _size
template <typename T> Rank PQ_ComplHeap<T>::percolateUp ( Rank i ) {
   while ( ParentValid ( i ) ) { //只要i有父亲（尚未抵达堆顶），则
      Rank j = Parent ( i ); //将i之父记作j
      if ( lt ( _elem[i], _elem[j] ) ) break; //一旦当前父子不再逆序，上滤旋即完成
      swap ( _elem[i], _elem[j] ); i = j; //否则，父子交换位置，并继续考查上一层
   } //while
   return i; //返回上滤最终抵达的位置
}
