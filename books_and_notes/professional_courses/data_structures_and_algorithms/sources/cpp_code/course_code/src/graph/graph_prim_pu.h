/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> struct PrimPU { //针对Prim算法的顶点优先级更新器
   virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
      if ( UNDISCOVERED == g->status ( v ) ) //对于uk每一尚未被发现的邻接顶点v
         if ( g->priority ( v ) > g->weight ( uk, v ) ) { //按Prim策略做松弛
            g->priority ( v ) = g->weight ( uk, v ); //更新优先级（数）
            g->parent ( v ) = uk; //更新父节点
         }
   }
};