/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> template <typename PU> //优先级搜索（全图）
void Graph<Tv, Te>::pfs ( int s, PU prioUpdater ) { //assert: 0 <= s < n
   reset(); int v = s; //初始化
   do //逐一检查所有顶点
      if ( UNDISCOVERED == status ( v ) ) //一旦遇到尚未发现的顶点
         PFS ( v, prioUpdater ); //即从该顶点出发启动一次PFS
   while ( s != ( v = ( ++v % n ) ) ); //按序号检查，故不漏不重
}

template <typename Tv, typename Te> template <typename PU> //顶点类型、边类型、优先级更新器
void Graph<Tv, Te>::PFS ( int s, PU prioUpdater ) { //优先级搜索（单个连通域）
   priority ( s ) = 0; status ( s ) = VISITED; parent ( s ) = -1; //初始化，起点s加至PFS树中
   while ( 1 ) { //将下一顶点和边加至PFS树中
      for ( int w = firstNbr ( s ); -1 < w; w = nextNbr ( s, w ) ) //枚举s的所有邻居w
         prioUpdater ( this, s, w ); //更新顶点w的优先级及其父顶点
      for ( int shortest = INT_MAX, w = 0; w < n; w++ )
         if ( UNDISCOVERED == status ( w ) ) //从尚未加入遍历树的顶点中
            if ( shortest > priority ( w ) ) //选出下一个
               { shortest = priority ( w ); s = w; } //优先级最高的顶点s
      if ( VISITED == status ( s ) ) break; //直至所有顶点均已加入
      status ( s ) = VISITED; type ( parent ( s ), s ) = TREE; //将s及与其父的联边加入遍历树
   }
} //通过定义具体的优先级更新策略prioUpdater，即可实现不同的算法功能
