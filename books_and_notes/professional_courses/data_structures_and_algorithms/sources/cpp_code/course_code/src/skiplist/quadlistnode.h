/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../Entry/Entry.h"
#define QlistNodePosi(T)  QuadlistNode<T>* //跳转表节点位置

template <typename T> struct QuadlistNode { //QuadlistNode模板类
   T entry; //所存词条
   QlistNodePosi(T) pred;  QlistNodePosi(T) succ; //前驱、后继
   QlistNodePosi(T) above; QlistNodePosi(T) below; //上邻、下邻
   QuadlistNode //构造器
   ( T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
     QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL )
      : entry ( e ), pred ( p ), succ ( s ), above ( a ), below ( b ) {}
   QlistNodePosi(T) insertAsSuccAbove //插入新节点，以当前节点为前驱，以节点b为下邻
   ( T const& e, QlistNodePosi(T) b = NULL );
};

#include "QuadlistNode_implementation.h"