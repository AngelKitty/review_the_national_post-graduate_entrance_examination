/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> bool RedBlack<T>::remove ( const T& e ) { //从红黑树中删除关键码e
   BinNodePosi(T) & x = search ( e ); if ( !x ) return false; //确认目标存在（留意_hot的设置）
   BinNodePosi(T) r = removeAt ( x, _hot ); if ( ! ( --_size ) ) return true; //实施删除
// assert: _hot某一孩子刚被删除，且被r所指节点（可能是NULL）接替。以下检查是否失衡，并做必要调整
   if ( ! _hot ) //若刚被删除的是根节点，则将其置黑，并更新黑高度
      { _root->color = RB_BLACK; updateHeight ( _root ); return true; }
// assert: 以下，原x（现r）必非根，_hot必非空
   if ( BlackHeightUpdated ( *_hot ) ) return true; //若所有祖先的黑深度依然平衡，则无需调整
   if ( IsRed ( r ) ) //否则，若r为红，则只需令其转黑
      { r->color = RB_BLACK; r->height++; return true; }
// assert: 以下，原x（现r）均为黑色
   //*DSA*/printBinTree(_hot, 0, 0);
   solveDoubleBlack ( r ); return true; //经双黑调整后返回
} //若目标节点存在且被删除，返回true；否则返回false
