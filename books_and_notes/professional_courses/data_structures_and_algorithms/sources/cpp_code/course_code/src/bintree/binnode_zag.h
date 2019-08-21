/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> BinNodePosi(T) BinNode<T>::zag() { //ÄæÊ±ÕëÐý×ª
   BinNodePosi(T) rChild = rc;
   rChild->parent = this->parent;
   if ( rChild->parent )
      ( ( this == rChild->parent->lc ) ? rChild->parent->lc : rChild->parent->rc ) = rChild;
   rc = rChild->lc; if ( rc ) rc->parent = this;
   rChild->lc = this; this->parent = rChild;
   return rChild;
}
