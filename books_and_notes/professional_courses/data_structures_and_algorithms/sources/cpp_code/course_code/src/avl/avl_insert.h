/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> BinNodePosi(T) AVL<T>::insert ( const T& e ) { //将关键码e插入AVL树中
   BinNodePosi(T) & x = search ( e ); if ( x ) return x; //确认目标节点不存在
   BinNodePosi(T) xx = x = new BinNode<T> ( e, _hot ); _size++; //创建新节点x
// 此时，x的父亲_hot若增高，则其祖父有可能失衡
   for ( BinNodePosi(T) g = _hot; g; g = g->parent ) { //从x之父出发向上，逐层检查各代祖先g
      if ( !AvlBalanced ( *g ) ) { //一旦发现g失衡，则（采用“3 + 4”算法）使之复衡，并将子树
         FromParentTo ( *g ) = rotateAt ( tallerChild ( tallerChild ( g ) ) ); //重新接入原树
         break; //g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
      } else //否则（g依然平衡），只需简单地
         updateHeight ( g ); //更新其高度（注意：即便g未失衡，高度亦可能增加）
   } //至多只需一次调整；若果真做过调整，则全树高度必然复原
   return xx; //返回新节点位置
} //无论e是否存在于原树中，总有AVL::insert(e)->data == e
