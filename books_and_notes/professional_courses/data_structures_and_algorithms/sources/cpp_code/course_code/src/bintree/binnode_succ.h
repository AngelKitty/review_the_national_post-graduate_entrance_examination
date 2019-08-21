/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> BinNodePosi(T) BinNode<T>::succ() { //定位节点v的直接后继
   BinNodePosi(T) s = this; //记录后继的临时变量
   if ( rc ) { //若有右孩子，则直接后继必在右子树中，具体地就是
      s = rc; //右子树中
      while ( HasLChild ( *s ) ) s = s->lc; //最靠左（最小）的节点
   } else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
      while ( IsRChild ( *s ) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
      s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
   }
   return s;
}